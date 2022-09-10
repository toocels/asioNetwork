#include "Server.h"
	

Server::Server(int iPORT, std::string iADDR, const char* logLoc, bool idoLog) 
 :PORT(iPORT), ADDR(iADDR), doLog(idoLog){
 	if(doLog)
		logFile.open(logLoc);
	logMsg("Server class intialised.\n");
}

Server::~Server(){
	for(auto client : clients){

	}
	std::cout << "Server object deleted." << std::endl;
}

void Server::listen_connections() {
	tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), PORT));
	logMsg("[SERVER] Waiting for connections.\n");
	try {
		while (true) {
			tcp::socket* socket = new tcp::socket(io_context);
			acceptor.accept(*socket); //blocking
			std::string timeString = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());

			clients.insert(std::pair<std::string, tcp::socket*>(timeString, socket));
			logMsg("[SERVER] New connection accepted.\n");

			send_message(socket, timeString); // timestring is clients name.id
			sleep(CONNECTION_LISTEN_DELAY);
		}
	} catch (std::exception& e) {
		logMsg("[SERVER] Error accepting connection:\n");
		// std::cerr << "" << e.what() << std::endl;
		return;
	}
}

std::pair<std::string, tcp::socket*> Server::listen_message(){
	while ( to_del_clients.size() > 0) {
		auto client = to_del_clients[0];
		disconnectClient(client);
		to_del_clients.erase(to_del_clients.begin());
	}


	std::vector<char> headBuf(8);
	int msgSize = 0 ;
	std::pair<std::string, tcp::socket*> ret;
	ret.first = "";
	ret.second = nullptr;

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

		ret.first = msg;
		ret.second = client;
		break;
	}

	return ret;
}

void Server::listen_messages(void recvMsg(std::string msg, tcp::socket* client)) {
	while (true) {
		while ( to_del_clients.size() > 0) {
			auto client = to_del_clients[0];
			disconnectClient(client);
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

			recvMsg(msg, client);

			headBuf.clear();
			msgSize = 0;
		}
	}
}

bool Server::disconnectClient(tcp::socket* client) {
	send_message(client, "exit");
	auto key = valueToKey(client);
	clients.erase(clients.find(key));

	client->shutdown(client->shutdown_both, ec);

	delete client;
	logMsg("[SERVER] Ended a connection\n");
	return true;
}

void Server::addToDelClients(tcp::socket* client) {
	to_del_clients.push_back(client);
}

void Server::send_message(tcp::socket* client, std::string msg) {
	std::stringstream ss;
	ss << std::setw(8) << std::setfill('0') << msg.length();
	asio::write(*client, asio::buffer(ss.str()), ec);
	asio::write(*client, asio::buffer(msg), ec);
}

void Server::logMsg(const char* logMsg) {
	if(!doLog) return;

	logFile << logMsg ;
	logFile.flush();
}

std::map<std::string, tcp::socket*> Server::getClients(){
	return clients;
}

std::string Server::indexToKey(int ind) {
	auto it = clients.begin();
	std::advance(it, ind);
	return it->first;
}

tcp::socket* Server::keyToValue(std::string key) {
	auto indexElem = clients.find(key);
	return indexElem->second;
}

std::string Server::valueToKey(tcp::socket* value) {
	for (auto i : clients)
		if (i.second == value)
			return i.first;
	return "-";
}

int Server::keyToIndex(std::string key) {
	auto toFind = clients.find(key);
	return distance(clients.begin(), toFind );
}

int Server::valueToIndex(tcp::socket* value){
	int ind=0;
	for(auto i: clients){
		if (i.second == value){
			return ind;
		}
		ind +=1;
	}
	return -1;
}