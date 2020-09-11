const dns = require('dns').promises;
const os = require('os')
const express = require('express')
const { addAsync } = require('@awaitjs/express');
const app = addAsync(express());
const MemcachePlus = require('memcache-plus');
const mongo = require('mongodb');
const MongoClient = mongo.MongoClient;

//Connect to the memcached instances
let memcached = null
let memcachedServers = []

const dbConfig = 'mongodb://mongo-connection:27017'

async function getMemcachedServersFromDns() {
	let queryResult = await dns.lookup('memcached-service', { all: true })
	let servers = queryResult.map(el => el.address + ":11211")

	//Only create a new object if the server list has changed
	if (memcachedServers.sort().toString() !== servers.sort().toString()) {
		console.log("Updated memcached server list to ", servers)
		memcachedServers = servers
		//Disconnect an existing client
		if (memcached)
			await memcached.disconnect()
		memcached = new MemcachePlus(memcachedServers);
	}
}

//Initially try to connect to the memcached servers, then each 5s update the list
getMemcachedServersFromDns()
setInterval(() => getMemcachedServersFromDns(), 5000)

//Get data from cache if a cache exists yet
async function getFromCache(key) {
	if (!memcached) {
		console.log(`No memcached instance available, memcachedServers = ${memcachedServers}`)
		return null;
	}
	return await memcached.get(key);
}

//Get data from database
// function getFromDatabase() {
// 	let output = []
// 	// let result1

// 	// let result2
// 	// result2 = MongoClient.connect(dbConfig, function(err, db) {
// 	//   if (err) throw err;
// 	//   var dbo = db.db("news");
// 	//   result1 = dbo.collection("newscollection").find().toArray(function(err, result) {
// 	// 	if (err) throw err;
// 	// 	console.log(result);
// 	// 	db.close();
// 	// 	return result
// 	//   });
// 	// return result1});


// MongoClient.connect(dbConfig, function(err, dbconnection) {
// 	var db = dbconnection.db("news")
//     var cursor = db.collection('newscollection').find();
//     cursor.each(function(err, doc) {
// 		console.log(doc);
// 		output.push(doc)
//     });
// });
// return output
// }

async function getAThing() {
    let db = await MongoClient.connect('mongodb://mongo-connection:27017/news');
        let thing = await db.collection("newscollection").findOne();
        await db.close();
        return thing;
}
async function send_response(response, data) {
	response.send(data)
}

app.get('/', function (request, response) {
	console.log("TEstong")
	response.writeHead(302, { 'Location': 'person/l.mlb.com-p.7491' })
	response.end();
})

app.getAsync('/person/:id', async function (request, response) {
	let key = 'user_'
	let cachedata = await getFromCache(key)

	if (cachedata) {
		console.log(`Cache hit for key=${key}, cachedata = ${cachedata}`)
		send_response(response, cachedata);
	} else {
		console.log(`Cache miss for key=${key}, querying database`)
		let data = await getAThing()
		console.log(data)
		if (data) {
			console.log(`Got data=${data}, storing in cache`)
			if (memcached)
				await memcached.set(key, data, 30 /* seconds */);
			send_response(response, data);
		} else {
			send_response(response, "No data found");
		}
	}
})

app.set('port', (process.env.PORT || 8080))

app.listen(app.get('port'), function () {
	console.log("Node app is running at localhost:" + app.get('port'))
})
