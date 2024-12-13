#include <iostream>

#include "BitcoinExchange.hpp"

#define USAGE         "Usage: btc <filename>"
#define DATABASE_FILE "data.csv"

int main(int ac, char** av) {
    if (ac != 2) {
        std::cerr << USAGE << std::endl;
        return 1;
    }

    try {
        BitcoinExchange::loadDatabase(DATABASE_FILE);
        BitcoinExchange::processInput(av[1]);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
