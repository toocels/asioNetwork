#include "cstm_libs/Client.h"

bool Client::connect(int PORT, string IP) {
	this->PORT = PORT;
	this->IP = IP;

	try {
		asio::ip::tcp::endpoint endpoints(asio::ip::make_address(IP, ec), PORT);
		socket.connect(endpoints);
		return true;
	} catch (...) {
		return false;
	}
}

string Client::fetch_msg() {
	vector<char> headBuf(8);
	size_t bytes = socket.available();

	if (bytes > 0)
		socket.read_some(asio::buffer(headBuf), ec);
	else
		return ""; // nothign to read

	int msgSize = 0 ;
	for (int i = 0; i < headBuf.size(); i++)
		if (isdigit(headBuf[i]))
			msgSize += ((int)headBuf[i] - 48) * pow(10, headBuf.size() - i - 1);

	vector<char> msgBuf(msgSize);
	socket.read_some(asio::buffer(msgBuf), ec);
	string msg(msgBuf.begin(), msgBuf.end());

	return msg;
}

void Client::fetch_msgs(void recvMsg(string msg)) {
	while (true) {
		vector<char> headBuf(8);
		int msgSize = 0 ;

		size_t bytes = socket.available();

		if (bytes > 0)
			socket.read_some(asio::buffer(headBuf), ec);
		else
			continue; // this clinet sending no data

		for (int i = 0; i < headBuf.size(); i++)
			if (isdigit(headBuf[i]))
				msgSize += ((int)headBuf[i] - 48) * pow(10, headBuf.size() - i - 1);

		vector<char> msgBuf(msgSize);
		socket.read_some(asio::buffer(msgBuf), ec);
		string msg(msgBuf.begin(), msgBuf.end());

		recvMsg(msg);
	}
}

void Client::sendMsg(string msg) {
	stringstream ss;
	ss << setw(8) << setfill('0') << msg.length();
	asio::write(socket, asio::buffer(ss.str()), ec);
	asio::write(socket, asio::buffer(msg), ec);
}
