#include "BitcoinExchange.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
	if (argc < 2) {
		std::cerr << INPUT_FILE_ERR << std::endl;
		return 1;
	}
	try {
		BitcoinExchange btc;
		btc.processInput(argv[1]);
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
}
