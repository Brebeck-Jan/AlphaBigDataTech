import happybase

connection = happybase.Connection('localhost', 9090)
connection.tables()
table = connection.table('crawled_articles2')

counter = 0
for k, data in table.scan():
    counter+=1
    print(k, data)

print(f"{counter}. entries")
connection.close()