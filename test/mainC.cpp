#include <iostream>

#include "Client.h"

using std::cout, std::endl, std::cin;

int main() {

	Client client;
	if (!client.connect(69, "127.0.0.1"))
		return 0; // failed to connect

	std::string cmd;
	while(true){
		cout << "Cmd: ";
		cin >> cmd;
		if(cmd == "exit")
			break;
		else if(cmd == "send"){
			std::string msg;
			cout << "Msg: ";
			cin >> msg;
			cout << endl;
			client.sendMsg(msg);
		}
		else if(cmd == "recv"){
			std::cout << client.fetch_msg() << std::endl;
		}
	}

	return 0;
}