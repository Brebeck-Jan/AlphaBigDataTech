import pymongo
import sys

#Create a MongoDB client
client = pymongo.MongoClient('mongodb://mongo-0.mongo-service') 

#Specify the database to be used
db = client.test

#Specify the collection to be used
col = db.myTestCollection

#Insert a single document
col.insert_one({'hello':'world'})

#Find the document that was previously written
x = col.find_one({'hello':'world'})

#Print the result to the screen
print(x)

#Close the connection
client.close()