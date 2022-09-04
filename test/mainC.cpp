#include <iostream>

#include "Client.h"

using std::cout, std::endl;

int main() {

	Client client;
	if (!client.connect(69, "127.0.0.1"))
		return 0; // failed to connect

	client.sendMsg("Msg from cleint :)");
	std::cout << client.fetch_msg() << std::endl;

	return 0;
}