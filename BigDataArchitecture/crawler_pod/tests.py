import happybase

connection = happybase.Connection('localhost', 9090)
connection.tables()
table = connection.table('crawled_articles')

for k, data in table.scan():
    print(k, data)

connection.close()