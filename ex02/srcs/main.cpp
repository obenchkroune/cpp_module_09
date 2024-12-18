#include <ctime>
#include <iostream>
#include <vector>

#include "PmergeMe.hpp"

int main(int ac, char** av) {

    try {
        std::vector<uint32_t> vec = PmergeMe::parse_args(ac, av);
        std::list<uint32_t>   lst(vec.begin(), vec.end());

        std::cout << "Before : ";
        PmergeMe::print_container(vec);

        double vector_benchmark = PmergeMe::benchmark(PmergeMe::sort, vec);
        double deque_benchmark  = PmergeMe::benchmark(PmergeMe::sort, lst);

        if (!PmergeMe::is_equal(vec, lst)) {
            throw std::runtime_error("Containers are not equal");
        }
        if (!PmergeMe::is_sorted(vec)) {
            throw std::runtime_error("Container is not sorted");
        }

        std::cout << "After  : ";
        PmergeMe::print_container(vec);

        std::cout << "Time to process a range of " << vec.size()
                  << " elements with std::vector : " << vector_benchmark << " us" << std::endl;

        std::cout << "Time to process a range of " << lst.size()
                  << " elements with std::list  : " << deque_benchmark << " us" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
