minikube start

minikube addons enable ingress

eval $(minikube docker-env)

docker build -t web-app .

for name in *.yaml; do kubectl apply -f "$name"; done