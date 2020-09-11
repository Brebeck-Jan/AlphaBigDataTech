kubectl delete svc/app-service
kubectl delete svc/mongo-service
kubectl delete svc/memcached-service
kubectl delete svc/headless-service

kubectl delete deployment/web-app-deployment
kubectl delete deployment/memcache-deployment
kubectl delete deployment/mongo-deployment

kubectl delete statefulset/mongo

kubectl delete horizontalpodautoscaler.autoscaling/app-deployment
kubectl delete horizontalpodautoscaler.autoscaling/memcache-deployment

kubectl delete -f crawler.yaml
kubectl delete -f datalake.yaml

minikube delete