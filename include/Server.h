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
	Server(int iPORT, std::string iADDR, const char* logLoc = "./out/log.txt", bool doLog = false);
	void listen_connections();
	void listen_messages(void recvMsg(std::string msg, Server *server));
	void send_message(tcp::socket* client, std::string msg);
	void logMsg(const char* logMsg);
	
	bool disconnectClient(tcp::socket* client);
	bool rmClient(tcp::socket* client);
	
	int keyToIndex(std::string key);
	std::string indexToKey(int ind);
	tcp::socket* keyToValue(std::string key);
	std::string valueToKey(tcp::socket* value);

	std::string timeSinceEpochMillisec();
	std::string make_daytime_string();
};
