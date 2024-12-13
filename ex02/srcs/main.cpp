#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>

#include "PmergeMe.hpp"

int main(int ac, char** av) {

    try {
        std::vector<uint32_t> vec = PmergeMe::parse_args(ac, av);
        std::cout << "Before : ";
        PmergeMe::print_container(vec);

        double vector_benchmark = PmergeMe::benchmark(PmergeMe::sort, vec);

        std::cout << "After  : ";
        PmergeMe::print_container(vec);

        std::cout << "Time to process a range of " << vec.size()
                  << " elements with std::vector : " << vector_benchmark << " us" << std::endl;

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
