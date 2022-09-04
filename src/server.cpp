#include "Server.h"

using asio::ip::tcp;

Server::Server(int iPORT, std::string iADDR, const char* logLoc) {
	this->PORT = iPORT;
	this->ADDR = iADDR;
	logFile.open(logLoc);
	this->logMsg("Server class intialised.\n");
}

void Server::listen_connections() {
	tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), this->PORT));
	this->logMsg("[SERVER] Waiting for connections.\n");
	try {
		while (true) {
			tcp::socket* socket = new tcp::socket(io_context);
			acceptor.accept(*socket); //blocking
			std::string timeString = timeSinceEpochMillisec();

			clients.insert(std::pair<std::string, tcp::socket*>(timeString, socket));
			this->logMsg("[SERVER] New connection accepted.\n");

			this->send_message(socket, timeString); // timestring is clients name.id
			sleep(0.5);
		}
	} catch (std::exception& e) {
		this->logMsg("[SERVER] Error accepting connection:\n");
		// std::cerr << "" << e.what() << std::endl;
		return;
	}
}

void Server::listen_messages(void recvMsg(std::string msg, Server *server)) {
	while (true) {
		while ( to_del_clients.size() > 0) {
			auto client = to_del_clients[0];
			this->disconnectClient(client);
			to_del_clients.erase(to_del_clients.begin());
		}

		std::vector<char> headBuf(8);
		int msgSize = 0 ;

		for (auto clienta : clients) {
			auto client = clienta.second;
			size_t bytes = client->available();

			if (bytes > 0)
				client->read_some(asio::buffer(headBuf), ec);
			else
				continue; // this clinet sending no data

			for (int i = 0; i < headBuf.size(); i++)
				if (isdigit(headBuf[i]))
					msgSize += ((int)headBuf[i] - 48) * pow(10, headBuf.size() - i - 1);

			std::vector<char> msgBuf(msgSize);
			client->read_some(asio::buffer(msgBuf), ec);
			std::string msg(msgBuf.begin(), msgBuf.end());

			recvMsg(msg, this);

			headBuf.clear();
			msgSize = 0;
		}
	}
}

// void Server::listen_messages(std::map<tcp::socket*, std::string> *msgBuffer) {
// 	while (true) {
// 		while ( to_del_clients.size() > 0) {
// 			auto client = to_del_clients[0];
// 			this->disconnectClient(client);
// 			to_del_clients.erase(to_del_clients.begin());
// 		}

// 		std::vector<char> headBuf(8);
// 		int msgSize = 0 ;

// 		for (auto clienta : clients) {
// 			auto client = clienta.second;
// 			size_t bytes = client->available();

// 			if (bytes > 0)
// 				client->read_some(asio::buffer(headBuf), ec);
// 			else
// 				continue; // this clinet sending no data

// 			for (int i = 0; i < headBuf.size(); i++)
// 				if (isdigit(headBuf[i]))
// 					msgSize += ((int)headBuf[i] - 48) * pow(10, headBuf.size() - i - 1);

// 			std::vector<char> msgBuf(msgSize);
// 			client->read_some(asio::buffer(msgBuf), ec);
// 			std::string msg(msgBuf.begin(), msgBuf.end());

// 			msgBuffer->insert({client , msg});

// 			headBuf.clear();
// 			msgSize = 0;
// 		}
// 	}
// }

bool Server::disconnectClient(tcp::socket* client) {
	this->send_message(client, "exit");
	this->rmClient(client);
	client->shutdown(client->shutdown_both, ec);

	delete client;
	this->logMsg("[SERVER] Ended a connection\n");
	return true;
}

void Server::send_message(tcp::socket* client, std::string msg) {
	std::stringstream ss;
	ss << std::setw(8) << std::setfill('0') << msg.length();
	asio::write(*client, asio::buffer(ss.str()), ec);
	asio::write(*client, asio::buffer(msg), ec);
}

void Server::logMsg(const char* logMsg) {
	logFile << logMsg ;
	logFile.flush();
}

bool Server::rmClient(tcp::socket* client) {
	auto key = this->valueToKey(client);
	clients.erase(clients.find(key));
	return true;
}

std::string Server::indexToKey(int ind) {
	auto it = this->clients.begin();
	std::advance(it, ind);
	return it->first;
}

tcp::socket* Server::keyToValue(std::string key) {
	auto indexElem = this->clients.find(key);
	return indexElem->second;
}

std::string Server::valueToKey(tcp::socket* value) {
	for (auto i : this->clients)
		if (i.second == value)
			return i.first;
	return "-";
}

int Server::keyToIndex(std::string key) {
	auto toFind = this->clients.find(key);
	return distance(this->clients.begin(), toFind );
}

std::string Server::timeSinceEpochMillisec() {
	using namespace std::chrono;
	return std::to_string(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
}

std::string Server::make_daytime_string() {
	time_t now = time(0);
	return ctime(&now);
}