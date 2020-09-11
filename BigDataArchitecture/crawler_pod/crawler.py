
# Crawl all News from the rss feeds of diferent newspapers, saved in newspages.txt
import requests
from bs4 import BeautifulSoup
import re
from datetime import datetime
import time
import csv
import happybase

# local setup:
# connection = happybase.Connection('localhost', 9090)
# dockersetup
connectionstring = 'localhost'
print(connectionstring)
connection = happybase.Connection(connectionstring, 9090)
connection.tables()
try:
    table = connection.table('crawled_articles')
    table.put("testkey",{"data:site":"test","data:title":"test","data:time":"test","data:link":"test","data:text":"test"})
except:
    connection.create_table('crawled_articles', { 'data': dict() } )
    print("table created")
    
connection.close()

def connect_to_hbase():
    batch_size=1000 
    # local setup:
    # connection = happybase.Connection('localhost', 9090)
    # dockersetup
    connection = happybase.Connection(connectionstring, 9090)
    connection.tables()
    table = connection.table('crawled_articles')
    batch = table.batch(batch_size = batch_size)
    return connection, table, batch

def umlauts(word):
    """
    Replace umlauts for a given text
    
    :param word: text as string
    :return: manipulated text as str
    """
    
    tempVar = word  
    
    tempVar = tempVar.replace('ä', 'ae')
    tempVar = tempVar.replace('ö', 'oe')
    tempVar = tempVar.replace('ü', 'ue')
    tempVar = tempVar.replace('Ä', 'Ae')
    tempVar = tempVar.replace('Ö', 'Oe')
    tempVar = tempVar.replace('Ü', 'Ue')
    tempVar = tempVar.replace('ß', 'ss')
    
    return tempVar

def preprocessing(title):
    text = umlauts(title)
    text = ' '.join((text.strip('\n').split()))
    return text

def crawl_page(url):
    """ Crawl news of given page and save to Datalake.
        Base structure from older project: https://github.com/Brebeck-Jan/alpha-tech/blob/master/code/backend/System/NewsCrawler.py
    """
    # set headers and cookies to prevent to not get blacklistet
    headers = {
            'user-agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36',
            'referrer': 'https://google.com'}

    cookies = {
            'prov': '91dc564-8757-c8f8'}
    
    # get data
    response = requests.request("GET", url, headers = headers, cookies = cookies)

    # parse data
    data = BeautifulSoup(response.content, "html.parser")
    # get titles of data
    items = data.find_all("item")
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
        
        
    # save to hadoop
    save_to_hadoop(articles,url)

def run_all():
    "Run crawling for all saved newspages."

    with open("newspages.txt") as f:

        for newspage in f.readlines():
            # run crawler
            crawl_page(newspage.rstrip())
    return(newspage)

def save_to_hadoop(articles,url=""):
    for article in articles:
        try:
            batch.put(str(timestamp)+" "+article["site"]+" "+article["title"],{"data:site":article["site"],"data:title":article["title"],"data:time":article["time"],"data:link":article["link"],"data:text":article["text"]})
        except Exception as e:
            print(f"{article['title']} from {article['site']} was skipped")
            print(e)
infinityloop = True
connection, table, batch = connect_to_hbase()
while(infinityloop):
    timestamp = datetime.now()
    run_all()
    print("finished")
    batch.send()
    connection.close()
    infinityloop = False
    time.sleep(18)

# local setup:
connection = happybase.Connection(connectionstring, 9090)
# dockersetup
# connection = happybase.Connection('hbase-compose', 9090)
connection.tables()
table = connection.table('crawled_articles')

counter = 0
for k, data in table.scan():
    counter+=1
#     print(k, data)

connection.close()
print(f"{counter}. entries")