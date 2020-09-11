FROM node:slim
EXPOSE 8080
WORKDIR /app
COPY package.json /app/
RUN npm install
RUN npm install mongodb
COPY server.js /app/
CMD node server.js