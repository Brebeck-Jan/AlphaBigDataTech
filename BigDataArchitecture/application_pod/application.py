##########################################################################################
##########################################################################################
# BigData - Application 						                                         #
##########################################################################################
##########################################################################################


##########################################################################################
# import libraries   						                                             #
##########################################################################################

import findspark
findspark.init()
from pyspark.sql import SparkSession
from pyspark.conf import SparkConf
import happybase
from nltk.corpus import stopwords
import pandas as pd

##########################################################################################
# init spark        						                                             #
##########################################################################################

# if we would use kubernetes then run this instead of initspark
'''conf=SparkConf()\
        .setMaster(local[*])\
        .setappName("WordCount")\
        .setExecutorEnv("spark.executor.memory","4g")\
        .setExecutorEnv("spark.driver.memory","4g")
  spark=SparkSession.builder\
        .config(conf=conf)\
        .getOrCreate()'''

spark=SparkSession.builder\
	.master("local[*]")\
	.appName("BigDataApplication")\
	.getOrCreate()
sc=spark.sparkContext

##########################################################################################
# prerequisites      						                                             #
##########################################################################################

# delete umlauts
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

# exclude punctuation
def lower_clean_str(x):
	punc='!"#$%&\'()*+,./:;<=>?@[\\]^_`{|}~-„“'
	lowercased_str = x.lower()
	for ch in punc:
		lowercased_str = lowercased_str.replace(ch, ' ')
	return lowercased_str

##########################################################################################
# Application       						                                             #
##########################################################################################

def BigDataApplication(news):

	# create Pipelined RDD
	df = sc.parallelize(news)

	# remove punktuation and transform to lowercase
	df = df.map(lower_clean_str) 

	#split sentences into list of words
	df = df.flatMap(lambda satir: satir.split(" ")) 

	# exclude whitespaces
	df = df.filter(lambda x:x!='') 

	# count how many times each word occurs
	count = df.map(lambda word:(word,1))
	countRBK = count.reduceByKey(lambda x,y:(x+y)).sortByKey()

	# rank words
	countRBK = countRBK.map(lambda x:(x[1],x[0]))
	countRBK = countRBK.sortByKey(False)

	# get german stopwords and change their umlauts
	stopwords =stopwords.words('german')
	german_stopwords = []
	for word in stopwords:
		german_stopwords.append(umlauts(word))

	# delete stopwords
	countRBK = countRBK.filter(lambda x: x[1] not in german_stopwords)

	# write result into pandas dataframe and export
	export = pd.DataFrame(columns=['trend-word'])
	for i in range(5):
		export = export.append({'trend-word': countRBK.take(5)[i][1]}, ignore_index=True)
	
	return export


##########################################################################################
# attaching database 						                                             #
##########################################################################################

def attach_database():

	connection = happybase.Connection(host='localhost', port=9090, autoconnect=True)
	table = connection.table('crawled_articles')

	news = []
	for k, data in table.scan():
		news.append(data[b'data:title'].decode('utf-8'))
	
	connection.close()
	return news


##########################################################################################
# Run Application with Data					                                             #
##########################################################################################

BigDataApplication(attach_database())

##########################################################################################
# write to MongoDB      					                                             #
##########################################################################################