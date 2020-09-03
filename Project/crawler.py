# Crawl all News from the rss feeds of diferent newspapers, saved in newspages.txt

# imports
import requests
from bs4 import BeautifulSoup
import re
from datetime import datetime

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
        article["site"]=url.split(".")[1]
        article["time"]=datetime.now().strftime("%H:%M:%S")
        article["title"] = item.title.get_text()
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
        articles.append(article)
        
    # save to hadoop
    save_to_hadoop(articles,url)

def save_to_hadoop(articles,url):
    with open("./output/"+articles[0]["site"]+url.split(".")[2].split("/")[-2]+".txt","w",encoding="utf-8") as f:
        for article in articles:
            f.write(str(article))

def run_all():
    "Run crawling for all saved newspages."

    with open("newspages.txt") as f:

        for newspage in f.readlines():
            # run crawler
            crawl_page(newspage.rstrip())

run_all()