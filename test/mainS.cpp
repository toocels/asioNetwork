#include <iostream>
#include <thread>

#include "cstm_libs/Server.h"

int main() {
	Server server(69, "127.0.0.1");

	cout << "Listening for connections..." << endl;
	thread listen_connections(&Server::listen_connections, &server);

	while (true) {
		cout << "Enter msg: ";
		string msg; // send msg
		getline(cin, msg);
		for (auto client : server.getClients())
			server.send_message(client, msg);

		auto msgRcv = server.listen_message();
		while (msgRcv.second != NULL) {
			cout << server.getIndex(msgRcv.second) << ' ' << msgRcv.first << endl;
			msgRcv = server.listen_message();
		}
	}

	return 0;
}