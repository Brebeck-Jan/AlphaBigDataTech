minikube start

minikube addons enable ingress

eval $(minikube docker-env)

docker build -t web-app .
docker build -t crawler ./crawler_pod

for name in *.yaml; do kubectl apply -f "$name"; done