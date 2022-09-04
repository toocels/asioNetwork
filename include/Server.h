#pragma once

#include <iostream>
#include <map>
#include <math.h>
#include <asio.hpp>
#include <fstream>
#include <iomanip>
#include <string>

using asio::ip::tcp;

class Server
{
public:
	int PORT;
	std::string ADDR;
	asio::error_code ec;
	asio::io_context io_context;
	std::map<std::string, tcp::socket*> clients;
	std::vector<tcp::socket*> to_del_clients;
	std::ofstream logFile;

	Server(int iPORT, std::string iADDR, const char* logLoc);
	void listen_connections();
	void listen_messages(void recvMsg(std::string msg, Server *server));
	// void listen_messages(std::map<tcp::socket*, std::string> *msgBuffer);
	bool disconnectClient(tcp::socket* client);
	void send_message(tcp::socket* client, std::string msg);
	void logMsg(const char* logMsg);
	bool rmClient(tcp::socket* client);
	std::string indexToKey(int ind);
	tcp::socket* keyToValue(std::string key);
	std::string valueToKey(tcp::socket* value);
	int keyToIndex(std::string key);
	std::string timeSinceEpochMillisec();
	std::string make_daytime_string();
};
