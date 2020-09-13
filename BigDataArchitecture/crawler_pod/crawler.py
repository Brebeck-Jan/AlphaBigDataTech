# Crawl all News from the rss feeds of diferent newspapers and save them as json-files to the datalake
import requests
from bs4 import BeautifulSoup
import re
from datetime import datetime
import time
import csv
import happybase
import time

# Create the crawld_articles table, if it's not in the datalake
def init_hbase_table(pod_adress,table_name):
    # Connection to the datalake-pod
    connection = happybase.Connection(pod_adress, 9090)
    tables = connection.tables()
    table_name = bytes(table_name, 'utf-8')
    # Check if tabel already exists, if not create it
    if table_name not in tables:
        try:
            connection.create_table(table_name, { 'data': dict() } )
        except:
            print(f"Error while creating {table_name}")
        finally:
            connection.close()

# Create connection to the required datalake table
def connect_to_hbase(pod_adress,table_name):
    try:
        connection = happybase.Connection(pod_adress, 9090)
        table = connection.table(table_name)
        batch = table.batch(batch_size = 1000)
        return connection, table, batch
    except:
        print("Error: Creating connection to datalake")
        
# removes all umlauts for a given word
def umlauts(word):
    tempVar = word  
    
    tempVar = tempVar.replace('ä', 'ae')
    tempVar = tempVar.replace('ö', 'oe')
    tempVar = tempVar.replace('ü', 'ue')
    tempVar = tempVar.replace('Ä', 'Ae')
    tempVar = tempVar.replace('Ö', 'Oe')
    tempVar = tempVar.replace('Ü', 'Ue')
    tempVar = tempVar.replace('ß', 'ss')
    
    return tempVar

# delets invisible commands
def preprocessing(title):
    text = umlauts(title)
    text = ' '.join((text.strip('\n').split()))
    return text

# crawls all article from a given rss feed
def crawl_page(url,batch):
    # set headers and cookies to prevent to get blacklistet
    headers = {
            'user-agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36',
            'referrer': 'https://google.com'}
    cookies = {
            'prov': '91dc564-8757-c8f8'}
    
    # get data from rss-feed and parse it
    response = requests.request("GET", url, headers = headers, cookies = cookies)
    data = BeautifulSoup(response.content, "html.parser")
    # get items of html_data
    items = data.find_all("item")
    
    # create array with jsons for each article with all attributes
    articles = []
    for item in items:
        article = {}
        article["site"] = url.split(".")[1]
        article["time"] = datetime.now().strftime("%H:%M:%S")
        article["title"] = preprocessing(item.title.get_text())
        article["link"] = item.guid.get_text()
        article["text"] = ""
        article_side =  BeautifulSoup(requests.request("GET", article["link"], headers = headers, cookies = cookies).content, "html.parser")
        if "spiegel" in url:
            selector = 'div[class*="RichText"]'
            article["authors"] = [author.get_text() for author in article_side.select('div[class="font-sansUI lg:text-base md:text-base sm:text-s text-shade-dark"]')]
            article["tags"] = [cat.get_text() for cat in item.find_all("category")]
        elif "faz" in url:
            selector = 'p[class*="TextParagraph"]'
            article["authors"] = [author.get_text() for author in article_side.select('li[class="atc-MetaItem atc-MetaItem-author"]')]
            article["tags"] = [url.split("/")[-2]]
        for paragraph in article_side.select(selector):
            article["text"] =  article["text"] + paragraph.get_text()
        article["text"]=preprocessing(article["text"])
        articles.append(article)
        
    # save to datalake
    save_to_datalake(articles,batch)
    
# saves each article of given array to the datalake
def save_to_datalake(articles,batch):
    try:
        for article in articles:
            batch.put(bytes(article["site"]+" "+article["title"], 'utf-8'),{"data:site":article["site"],"data:title":article["title"],"data:time":article["time"],"data:link":article["link"],"data:text":article["text"]})
        batch.send()
    except:
        print(f"Error saving articles to datalake")

# Run crawling for all newspages of newspages.txt
def run_all(batch):
    with open("newspages.txt") as f:
        for newspage in f.readlines():
            crawl_page(newspage.rstrip(),batch)

### RUN ###
# settings:
hbase_pod_servicename = "lake-connection"
table_name='crawled_articles'

try:
    init_hbase_table(hbase_pod_servicename,table_name)
    connection, table, batch = connect_to_hbase(hbase_pod_servicename,table_name)
    run_all(batch)
except Exception as e:
    print(f"Error running the crawler: ")
    print(e)
finally:
    connection.close()

# time sleep, that the pod gets rebuild after completion
time.sleep(1000)