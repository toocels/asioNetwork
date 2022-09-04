#include <iostream>
#include <string>
#include <asio.hpp>


#include "Server.h"

int main() {

	// auto a = new Client();
	Server a(69, "127.0.0.1");
	a.listen_connections();
	return 0;
}