#include <iostream>
#include <string>
#include <asio.hpp>

#include <Client.h>

int main() {
	std::cout << "ytes" << std::endl;

	Client a;
	if (!a.connect(69, "127.0.0.1"))
		return 0; // failed

	sleep(1);
	std::cout << a.fetch_msg() << std::endl;
	return 0;
}