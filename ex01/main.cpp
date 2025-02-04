#include <cstdlib>
#include <iostream>

#include "RPN.hpp"

int main(int ac, char** av) {
    if (ac < 2) {
        std::cerr << rpn_usage(av[0]) << std::endl;
        return EXIT_FAILURE;
    }

    std::string input;
    for (int i = 1; i < ac; ++i) {
        input += av[i];
        input += ' ';
    }

    try {
        std::cout << evaluate_expression(input) << std::endl;
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}