#include <iostream>
#include <thread>

#include "Server.h"

using std::cout, std::endl;

void recvMsg(std::string msg, Server *server){
	cout << "Msg from client: " << msg << endl;
}

int main() {

	Server server(69, "127.0.0.1", "./out/log.txt");
	
	cout << ":Listening for connections:" << endl;
	std::thread thread_obj(&Server::listen_connections, &server);

	server.listen_messages(recvMsg);

	return 0;
}