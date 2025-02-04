#include <cstdlib>
#include <iostream>

#include "BitcoinExchange.hpp"

int main(int ac, char** av) {
    if (ac != 2) {
        std::cerr << FILE_ERROR << std::endl;
        return EXIT_SUCCESS;
    }

    BitcoinExchange btc;

    if (!btc.loadDatabase()) {
        std::cerr << DB_ERROR << std::endl;
        return EXIT_FAILURE;
    }

    if (!btc.processFile(av[1])) {
        std::cerr << FILE_ERROR << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
