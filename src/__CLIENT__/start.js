//console.log(process.env);
const bodyParser = require('body-parser')

const START_ROUTINES = require('./start_routines.js');

var __DBCLIENT__ = require('./../__ENTITIES__/DBClient.js');
var __DBREQUEST__ = require('./../__ENTITIES__/DBRequest.js');
var SOCKETS = require('./../__ENTITIES__/Socket.js');

//es7 async/await`
const snooze = ms => new Promise(resolve => setTimeout(resolve, ms));

//===============
'use strict';
//===============

//=======================================================
let CONFIGS = START_ROUTINES.READCONFIGS('system_configs');
let RETURN_VALUES = START_ROUTINES.READCONFIGS('return_values');
if(typeof CONFIGS["__DEFAULT__"] == "undefined") {
	console.error("=> CONFIGS NOT PROPERLY LOADED");
	return;
}
const __TCP_HOST_NAME__ = CONFIGS["SERVER_HOST"];
const __TCP_HOST_PORT__ = CONFIGS["SERVER_PORT"];
const __CLIENT_TOKEN__ = CONFIGS["TOKEN"];
const __CLIENT_UUID__ = CONFIGS["UUID"];
const __APP_TYPE__ = CONFIGS["APP_TYPE"].split(',')
//TODO: Check all this variables before starting

//TODO: Checks ( this should not be empty )
console.log("=> __TCP_HOST_NAME__: %s", __TCP_HOST_NAME__);
console.log("=> __TCP_HOST_PORT__: %s", __TCP_HOST_PORT__);
console.log("=> __CLIENT_TOKEN__: %s", __CLIENT_TOKEN__);
console.log("=> __CLIENT_UUID__: %s", __CLIENT_UUID__);
console.log("=> __APP_TYPE__: %s", __APP_TYPE__);
//=======================================================

//================================================
var __MYSQL_CONNECTION__;
var __SOCKET_COLLECTION__;
SOCKETS = new SOCKETS;

(async ()=>{
	try{
		__MYSQL_CONNECTION__ = await START_ROUTINES.GET_MYSQL_CONNECTION();
		console.log("=> MYSQL CONNECTION ESTABLISHED");
	}
	catch(error) {
		console.log(error);
		return;
	}
})();

(async ()=>{
	try {
		__SOCKET_COLLECTION__ = await SOCKETS.startSockets();
		console.log("=> SOCKETS ESTABLISHED");
	}
	catch( error ) {
		console.log(error);
		return;
	}
})();

(async ()=>{
	let startSocketConnection = async ()=>{
		try {
			let socketConnectionPromise = await SOCKETS.connect(__TCP_HOST_NAME__, __TCP_HOST_PORT__);
			console.log("=> SERVER CONNECTION ESTABLISHED");
		}
		catch (error) {
			console.error("=> CONNECTION ERROR:", error);
			console.error("=> FAILED CONNECTION TO SERVER");

			let reconnectionTimeout = 5000;
			console.log("=> PENDING RECONNECTION - T MINUS 5 SECONDS")

			await snooze( reconnectionTimeout );
			await startSocketConnection();
			return false;
		}
	} 
	startSocketConnection();
})();
