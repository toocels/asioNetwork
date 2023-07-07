#pragma once

#include <iostream>
#include <asio.hpp>
#include <iomanip>
#include <math.h>
#include <string>

using namespace std;

class Client {
private:
	int PORT;
	string IP;
	asio::error_code ec;
	asio::io_context io_context;
	asio::ip::tcp::socket socket = asio::ip::tcp::socket(io_context);

public:
	bool connect(int PORT, string IP);
	void sendMsg(string msg);
	void fetch_msgs(void recvMsg(string msg));
	string fetch_msg();
};
