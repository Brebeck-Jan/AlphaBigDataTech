for name in *.yaml; do kubectl delete -f "$name"; done

minikube delete