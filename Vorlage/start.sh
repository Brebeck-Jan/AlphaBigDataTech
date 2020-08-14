docker build -t my-super-web-app .

for name in *.yaml; do kubectl apply -f "$name"; done