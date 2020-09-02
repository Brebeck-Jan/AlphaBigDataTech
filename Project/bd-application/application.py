######################################################
# Text preprocessing - replace umlauts 
######################################################

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

def preprocessing(filename):
	with open(filename, encoding='utf-8') as file:
		text = file.read()
	text = umlauts(text)
	text = ' '.join((text.strip('\n').split()))

	text_file = open(filename, "w")
	n = text_file.write(text)
	text_file.close()


######################################################
# BigData - Anwendung 
######################################################

import findspark
findspark.init()
from pyspark.sql import SparkSession
from pyspark.conf import SparkConf

# if we would use kubernetes then run this instead of next cell
'''conf=SparkConf()\
        .setMaster(local[*])\
        .setappName("WordCount")\
        .setExecutorEnv("spark.executor.memory","4g")\
        .setExecutorEnv("spark.driver.memory","4g")
  spark=SparkSession.builder\
        .config(conf=conf)\
        .getOrCreate()'''

# spark init
spark=SparkSession.builder\
    .master("local[*]")\
    .appName("BigDataApplication")\
    .getOrCreate()
sc=spark.sparkContext

# exclude punctuation
def lower_clean_str(x):
	punc='!"#$%&\'()*+,./:;<=>?@[\\]^_`{|}~-'
	lowercased_str = x.lower()
	for ch in punc:
		lowercased_str = lowercased_str.replace(ch, '')
	return lowercased_str

def BigDataApplication(df):

	#remove punktuation and transform to lowercase
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

	# exclude stopwords
	## if normal importing doesn't work:   ##
	## python -m nltk.downloader stopwords ##

	#import nltk
	#nltk.download('stopwords')

	from nltk.corpus import stopwords
	stopwords =stopwords.words('german')

	german_stopwords = []
	for word in stopwords:
		german_stopwords.append(umlauts(word))

	countRBK = countRBK.filter(lambda x: x[1] not in german_stopwords)
	
	return countRBK.take(5)



# atach Database here
# for testing we use test.txt file
file = '/Users/Syman/Documents/Studij/Semester04/BigData/AlphaBigDataTech/Project/bd-application/test.txt'
file = sc.textFile(file)


print(BigDataApplication(file))