import happybase

connection = happybase.Connection('localhost', 9090)
connection.tables()
try:
    table = connection.table('crawled_articles')
    table.put("testkey",{"data:site":"test","data:title":"test","data:time":"test","data:link":"test","data:text":"test"})
except:
    connection.create_table('crawled_articles', { 'data': dict() } )
    print("table created")
    
connection.close()