#pragma once

#include <iostream>
#include <map>
#include <math.h>
#include <asio.hpp>
#include <fstream>
#include <iomanip>
#include <string>

#define CONNECTION_LISTEN_DELAY 1 // limits speed at which connections are accepted

using asio::ip::tcp;

class Server
{
private:
	int PORT;
	std::string ADDR;
	asio::error_code ec;
	asio::io_context io_context;
	std::map<std::string, tcp::socket*> clients;
	std::vector<tcp::socket*> to_del_clients;
	std::ofstream logFile;
	bool doLog = false;

public:
	Server(int iPORT, std::string iADDR, const char* logLoc = "./log.txt", bool doLog = false);
	~Server();
	void listen_connections();
	void listen_messages(void recvMsg(std::string msg, tcp::socket* client));
	std::pair<std::string, tcp::socket*> listen_message();
	void send_message(tcp::socket* client, std::string msg);
	void logMsg(const char* logMsg);
	
	bool disconnectClient(tcp::socket* client);
	void rmClient(tcp::socket* client);

	std::map<std::string, tcp::socket*> getClients();
	
	int keyToIndex(std::string key);
	std::string indexToKey(int ind);
	tcp::socket* keyToValue(std::string key);
	std::string valueToKey(tcp::socket* value);
};
