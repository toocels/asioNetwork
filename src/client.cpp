#include "Client.h"

bool Client::connect(int lPORT, std::string lADDR) {
	this->PORT = lPORT;
	this->ADDR = lADDR;

	// asio::ip::tcp::resolver resolver(io_context);
	try {
		asio::ip::tcp::endpoint endpoints(asio::ip::make_address(ADDR, ec), PORT);
		socket.connect(endpoints);
		return true;
	}
	catch (...) {
		return false;
	}
}

std::string Client::fetch_msg() {
	std::vector<char> headBuf(8);
	size_t bytes = socket.available();

	if (bytes > 0)
		socket.read_some(asio::buffer(headBuf), ec);
	else
		return ""; // nothign to read

	int msgSize = 0 ;
	for (int i = 0; i < headBuf.size(); i++)
		if (isdigit(headBuf[i]))
			msgSize += ((int)headBuf[i] - 48) * pow(10, headBuf.size() - i - 1);

	std::vector<char> msgBuf(msgSize);
	socket.read_some(asio::buffer(msgBuf), ec);
	std::string msg(msgBuf.begin(), msgBuf.end());

	return msg;
}

void Client::fetch_msgs(void recvMsg(std::string msg)) {
	while (true) {
		std::vector<char> headBuf(8);
		int msgSize = 0 ;

		size_t bytes = socket.available();

		if (bytes > 0)
			socket.read_some(asio::buffer(headBuf), ec);
		else
			continue; // this clinet sending no data

		for (int i = 0; i < headBuf.size(); i++)
			if (isdigit(headBuf[i]))
				msgSize += ((int)headBuf[i] - 48) * pow(10, headBuf.size() - i - 1);

		std::vector<char> msgBuf(msgSize);
		socket.read_some(asio::buffer(msgBuf), ec);
		std::string msg(msgBuf.begin(), msgBuf.end());

		recvMsg(msg);
	}
}

// void Client::fetch_msgs(std::vector<std::string> *msgBuffer) {
// 	while (true) {
// 		std::vector<char> headBuf(8);
// 		int msgSize = 0 ;

// 		size_t bytes = socket.available();

// 		if (bytes > 0)
// 			socket.read_some(asio::buffer(headBuf), ec);
// 		else
// 			continue; // this clinet sending no data

// 		for (int i = 0; i < headBuf.size(); i++)
// 			if (isdigit(headBuf[i]))
// 				msgSize += ((int)headBuf[i] - 48) * pow(10, headBuf.size() - i - 1);

// 		std::vector<char> msgBuf(msgSize);
// 		socket.read_some(asio::buffer(msgBuf), ec);
// 		std::string msg(msgBuf.begin(), msgBuf.end());

// 		msgBuffer->push_back(msg);
// 	}
// }

void Client::sendMsg(std::string msg) {
	std::stringstream ss;
	ss << std::setw(8) << std::setfill('0') << msg.length();
	asio::write(socket, asio::buffer(ss.str()), ec);
	asio::write(socket, asio::buffer(msg), ec);
}
