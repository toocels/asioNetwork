#pragma once

class Client
{
public:
	int PORT;
	std::string ADDR;
	asio::error_code ec;
	asio::io_context io_context;
	asio::ip::tcp::socket socket = asio::ip::tcp::socket(io_context);

	bool connect(int lPORT, std::string lADDR);
	std::string fetch_msg();
	void fetch_msgs(void recvMsg(std::string msg));
	// void fetch_msgs(std::vector<std::string> *msgBuffer);
	void sendMsg(std::string msg);
};
