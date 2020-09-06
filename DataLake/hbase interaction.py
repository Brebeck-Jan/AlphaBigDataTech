import csv
import happybase
import time

batch_size = 1000
host = "0.0.0.0"
file_path = "test.csv"
namespace = "newspaper_article"
row_count = 0
start_time = time.time()
table_name = "bgt"


def connect_to_hbase():
    """ Connect to HBase server.
    This will use the host, namespace, table name, and batch size as defined in
    the global variables above.
    """
    conn = happybase.Connection(host = host,
        table_prefix = namespace,
        table_prefix_separator = ":")
    conn.open()
    table = conn.table(table_name)
    batch = table.batch(batch_size = batch_size)
    return conn, batch


def insert_row(batch, row):
    #Insert a row into HBase.
 
    batch.put(row[0], { "data:site": row[1], "data:time": row[2], "data:title": row[3],
        "data:link": row[4], "data:tags": row[5], "data:author": row[6],
        "data:text": row[7]})


def read_csv():
    csvfile = open(file_path, "r")
    csvreader = csv.reader(csvfile)
    return csvreader, csvfile

conn, batch = connect_to_hbase()
csvreader, csvfile = read_csv()

try:
    for row in csvreader:
        row_count += 1
        if row_count == 1:
            pass
        else:
            insert_row(batch, row)

    batch.send()
finally:
    csvfile.close()
    conn.close()

duration = time.time() - start_time
print ("Done. row count: %i, duration: %.3f s" % (row_count, duration))