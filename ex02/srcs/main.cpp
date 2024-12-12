#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>

#include "PmergeMe.hpp"

int main(int ac, char** av) {
    std::vector<uint32_t> vec;

    try {
        vec = PmergeMe::parse_args(ac, av);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Before:" << std::endl;
    for (std::vector<uint32_t>::const_iterator it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    PmergeMe::sort(vec);

    std::cout << "Comparison count: " << PmergeMe::get_comparison_count() << std::endl;

    std::cout << "After:" << std::endl;
    for (std::vector<uint32_t>::const_iterator it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << " ";
    }
    return 0;
}
