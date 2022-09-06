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
		else if(cmd == "listen"){
			server.listen_messages(recvMsg);
		}
	}

	return 0;
}