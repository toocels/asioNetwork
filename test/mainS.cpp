#include <iostream>
#include <thread>

#include "Server.h"

using std::cout, std::endl;
using std::cout, std::endl, std::cin;

void recvMsg(std::string msg, tcp::socket* client){
	cout << "Msg from client: " << msg  << " " << client << endl;
}

int main() {

	Server server(69, "127.0.0.1");
	
	cout << ":Listening for connections:" << endl;
	std::thread listen_connections(&Server::listen_connections, &server);

	while(true){
		std::string cmd;
		cout << "Cmd: ";
		cin >> cmd;
		if(cmd == "exit")
			break;
		else if(cmd == "cons"){
			for(auto client : server.getClients()){
				cout << client.first << ' ' << client.second << endl;
			}
		}
		else if(cmd == "msgs"){
			auto a = server.listen_message();
			cout << "Msg: " << a.first << ' ' << a.second << endl;
		}
		else if(cmd == "rm"){
			std::string inp;
			cout << "By index/ key: ";
			cin >> inp;
			if(inp == "ind"){
				int ind;
				cout << "Ind = ";
				cin >> ind;
				server.addToDelClients(server.keyToValue(server.indexToKey(ind)));
			}
			else if(inp == "key"){
				std::string key;
				cout << "Key = ";
				cin >> key;
				server.addToDelClients(server.keyToValue(key));	
			}
		}
	}

	return 0;
}