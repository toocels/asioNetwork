#include "cstm_libs/Client.h"
#include <iostream>


int main() {

	Client client;
	if (!client.connect(69, "127.0.0.1")) {
		cout << "Failed to connect" << endl;
		return 0; // failed to connect
	}

	while (true) {
		cout << "Enter msg: ";
		string msg; // send msg
		getline(cin, msg);
		client.sendMsg(msg);

		string tmp = client.fetch_msg(); // recv msg
		while (tmp != "") {
			cout << "[ SEVER ] " << tmp << endl;
			tmp = client.fetch_msg();
		}
	}

	return 0;
}