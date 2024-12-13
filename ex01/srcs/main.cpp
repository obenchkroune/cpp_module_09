#include <iostream>

#include "RPN.hpp"

#define USAGE "Usage: RPN <expr> [expr ...]"

int main(int ac, char** av) {
    if (ac < 2) {
        std::cerr << USAGE << std::endl;
        return 1;
    }

    std::string input = collect_input(ac, av);

    try {
        std::cout << RPN(input) << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
