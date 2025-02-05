#include <stdint.h>

#include <cstdlib>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>

#include "PmergeMe.hpp"

std::string ft_usage(const char* progname) {
    return "usage: " + std::string(progname) + " <numbers> [...more numbers]";
}

std::vector<uint32_t> parse_numbers(int ac, char** av) {
    std::stringstream ss;

    for (int i = 1; i < ac; ++i) {
        ss << av[i] << ' ';
    }

    std::vector<uint32_t> result;
    uint32_t              nbr;

    while (ss >> std::ws && !ss.eof()) {
        ss >> nbr;
        if (ss.fail()) throw std::runtime_error("invalid input");
        result.push_back(nbr);
    }
    return result;
}

template <typename Tp>
void print_container(const char* label, const Tp& vec) {
    std::cout << label << ":\t";
    for (typename Tp::const_iterator it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << ", ";
    }
    std::cout << std::endl;
}

int main(int ac, char** av) {
    if (ac < 2) {
        std::cerr << ft_usage(av[0]) << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<uint32_t> nbrs_vec = parse_numbers(ac, av);
    std::list<uint32_t>   nbrs_lst(nbrs_vec.begin(), nbrs_vec.end());
    timespec              start, end;

    print_container("Before", nbrs_vec);

    clock_gettime(CLOCK_REALTIME, &start);
    sort_vector(nbrs_vec);
    clock_gettime(CLOCK_REALTIME, &end);

    print_container("After", nbrs_vec);

    double vec_time_us = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3;

    clock_gettime(CLOCK_REALTIME, &start);
    sort_list(nbrs_lst);
    clock_gettime(CLOCK_REALTIME, &end);

    double lst_time_us = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3;

    std::cout << "Time to process a range of 5 elements with std::vector : " << vec_time_us << " us" << std::endl;
    std::cout << "Time to process a range of 5 elements with std::list   : " << lst_time_us << " us" << std::endl;
    return EXIT_SUCCESS;
}
