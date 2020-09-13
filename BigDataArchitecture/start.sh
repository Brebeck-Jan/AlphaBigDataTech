minikube start

minikube addons enable ingress

eval $(minikube docker-env)

docker build -t data-lake ./../DataLake/hbase-docker-master
docker build -t web-app .
docker build -t crawler ./crawler_pod
docker build -t application ./application_pod

for name in *.yaml; do kubectl apply -f "$name"; done
