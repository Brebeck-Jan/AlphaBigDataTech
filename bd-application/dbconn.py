import happybase

connection = happybase.Connection(host='localhost', port=9090, autoconnect=True)

table = connection.table('crawled_articles')

news = []
for k, data in table.scan():
    #news.append(data[b'data:title'])
    news.append(data[b'data:title'].decode('utf-8'))

print(news)

connection.close()