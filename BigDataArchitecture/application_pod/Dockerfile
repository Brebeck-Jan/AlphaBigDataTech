FROM python:3.7

Run apt-get update
Run apt-get install default-jdk -y
Run apt-get install git
Run curl https://mirror.synyx.de/apache/spark/spark-3.0.1/spark-3.0.1-bin-hadoop2.7.tgz --output spark.tgz
Run tar xvf spark.tgz
Run ls

Add . /
RUN pip install -r requirements.txt
Env  SPARK_HOME spark-3.0.1-bin-hadoop2.7

CMD ["python3","application.py"]    
