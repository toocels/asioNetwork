#pragma once

#include <iostream>
#include <map>
#include <math.h>
#include <asio.hpp>
#include <fstream>
#include <iomanip>
#include <string>

#define CONNECTIONS_LISTEN_DELAY_US 100'000 // limits speed at which connections are accepted for "listen_messages()"

using asio::ip::tcp;
using namespace std;

class Server {
private:
	int PORT;
	string IP;
	asio::error_code ec;
	vector<tcp::socket*> clients;
	vector<tcp::socket*> to_del_clients;
	bool disconnectClient(tcp::socket* client);
	void logMsg(const string& logMsg);

public:
	Server(int PORT, string IP);
	void listen_connections();
	void send_message(tcp::socket* client, string msg);
	vector<tcp::socket*> getClients();
	int getIndex(tcp::socket* K) ;

	void listen_messages(void recvMsg(string msg, tcp::socket* client));
	pair<string, tcp::socket*> listen_message();
	void addToDelClients(tcp::socket* client);
	// map<string, tcp::socket*> getClients();

	int keyToIndex(string key);
	string indexToKey(int ind);
	tcp::socket* keyToValue(string key);
	string valueToKey(tcp::socket* value);
	int valueToIndex(tcp::socket* value);
	tcp::socket* indexToValue(int ind);
};
