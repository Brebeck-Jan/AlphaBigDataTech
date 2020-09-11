import happybase
# local setup:
connection = happybase.Connection('hello')
# dockersetup
# connection = happybase.Connection('hello', 9090)
connection.tables()
table = connection.table('crawled_articles')

counter = 0
for k, data in table.scan():
    counter+=1
    print(k, data)

print(f"{counter}. entries")
connection.close()