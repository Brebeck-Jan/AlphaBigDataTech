# Crawl all News from the rss feeds of diferent newspapers, saved in newspages.txt

# imports
import requests
from bs4 import BeautifulSoup
import re

def crawl_page(url):
    """ Crawl nes of given page and save to Datalake.
        Base structure from older project: https://github.com/Brebeck-Jan/alpha-tech/blob/master/code/backend/System/NewsCrawler.py
    """
    print(url)
    print(type(url))
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
    titles = data.find_all("title")
    
    # extract only title
    titles = [re.search("<title>(.*)</title>", str(element)).group(1) for element in titles]

    print(titles)
    # save to hadoop
    save_to_hadoop(titles)

def save_to_hadoop(titles):
    "Function to save titles to hadoop."
    pass

def run_all():
    "Run crawling for all saved newspages."

    with open("newspages.txt") as f:

        for newspage in f.readlines():
            
            # run crawler
            crawl_page(newspage.rstrip())

# crawl_page("https://www.spiegel.de/schlagzeilen/index.rss")
# crawl_page("http://newsfeed.zeit.de/all")

run_all()