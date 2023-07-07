#include "cstm_libs/Server.h"


Server::Server(int PORT, string IP) {
	this->PORT = PORT;
	this->IP = IP;
	logMsg("Server class intialised.");
}

void Server::listen_connections() {
	asio::io_context io_context;
	tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), PORT));
	logMsg("[ SERVER ] Waiting for connections.");
	try {
		while (true) {
			tcp::socket* socket = new tcp::socket(io_context);
			acceptor.accept(*socket); //blocking

			string timeString = to_string(chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count());
			clients.push_back(socket);
			logMsg("[ SERVER ] New connection accepted.");

			send_message(socket, timeString); // timestring is clients name.id// now more like ack
			usleep(CONNECTIONS_LISTEN_DELAY_US);
		}
	} catch (exception& e) {
		logMsg("[ SERVER ] Error accepting connection:");
		// cerr << "" << e.what() << endl;
		return;
	}
}

void Server::logMsg(const string& logMsg) {
	cout << logMsg << endl;
}

void Server::send_message(tcp::socket* client, string msg) {
	stringstream ss;
	asio::error_code ec;
	ss << setw(8) << setfill('0') << msg.length();
	asio::write(*client, asio::buffer(ss.str()), ec);
	asio::write(*client, asio::buffer(msg), ec);
}

pair<string, tcp::socket*> Server::listen_message() {
	while ( to_del_clients.size() > 0) {
		auto client = to_del_clients[0];
		disconnectClient(client);
		to_del_clients.erase(to_del_clients.begin());
	}

	int msgSize = 0 ;
	vector<char> headBuf(8);
	pair<string, tcp::socket*> ret;
	ret.first = "";
	ret.second = nullptr;

	for (auto client : clients) {
		size_t bytes = client->available();

		if (bytes > 0)
			client->read_some(asio::buffer(headBuf), ec);
		else
			continue; // this clinet sending no data

		for (int i = 0; i < headBuf.size(); i++)
			if (isdigit(headBuf[i]))
				msgSize += ((int)headBuf[i] - 48) * pow(10, headBuf.size() - i - 1);

		vector<char> msgBuf(msgSize);
		client->read_some(asio::buffer(msgBuf), ec);
		string msg(msgBuf.begin(), msgBuf.end());

		ret.first = msg;
		ret.second = client;
		break;
	}

	return ret;
}

vector<tcp::socket*> Server::getClients() {
	return clients;
}

int Server::getIndex(tcp::socket* K) {
	auto it = find(clients.begin(), clients.end(), K);
	if (it != clients.end()) return it - clients.begin();
	else return -1;
}

void Server::listen_messages(void recvMsg(string msg, tcp::socket* client)) {
	while (true) {
		while ( to_del_clients.size() > 0) {
			auto client = to_del_clients[0];
			disconnectClient(client);
			to_del_clients.erase(to_del_clients.begin());
		}

		vector<char> headBuf(8);
		int msgSize = 0 ;

		for (auto client : clients) {
			size_t bytes = client->available();

			if (bytes > 0)
				client->read_some(asio::buffer(headBuf), ec);
			else
				continue; // this clinet sending no data

			for (int i = 0; i < headBuf.size(); i++)
				if (isdigit(headBuf[i]))
					msgSize += ((int)headBuf[i] - 48) * pow(10, headBuf.size() - i - 1);

			vector<char> msgBuf(msgSize);
			client->read_some(asio::buffer(msgBuf), ec);
			string msg(msgBuf.begin(), msgBuf.end());

			recvMsg(msg, client);

			headBuf.clear();
			msgSize = 0;
		}
	}
}

bool Server::disconnectClient(tcp::socket* client) {
	send_message(client, "close");
	clients.erase(find(clients.begin(), clients.end(), client));
	client->shutdown(client->shutdown_both, ec);
	delete client;

	logMsg("[ SERVER ] Ended a connection");
	return true;
}

// void Server::addToDelClients(tcp::socket* client) {
// 	to_del_clients.push_back(client);
// }

// map<string, tcp::socket*> Server::getClients() {
// 	return clients;
// }

// string Server::indexToKey(int ind) {
// 	auto it = clients.begin();
// 	advance(it, ind);
// 	return it->first;
// }

// tcp::socket* Server::keyToValue(string key) {
// 	auto indexElem = clients.find(key);
// 	return indexElem->second;
// }

// int Server::valueToIndex(tcp::socket* value) {
// 	int ind = 0;
// 	for (auto i : clients) {
// 		if (i.second == value) {
// 			return ind;
// 		}
// 		ind += 1;
// 	}
// 	return -1;
// }



// string Server::valueToKey(tcp::socket* value) {
// 	auto ind = valueToIndex(value);
// 	auto key = indexToKey(ind);
// 	return key;
// }

// int Server::keyToIndex(string key) {
// 	auto value = keyToValue(key);
// 	auto index = valueToIndex(value);
// 	return index;
// }

// tcp::socket* Server::indexToValue(int ind) {
// 	auto key = indexToKey(ind);
// 	auto value = keyToValue(key);
// 	return value;
// }