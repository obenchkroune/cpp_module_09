#include <stdint.h>

#include <cstdlib>
#include <iostream>
#include <list>
#include <vector>

#include "PmergeMe.hpp"

int main(int ac, char** av) {
    if (ac < 2) {
        std::cerr << print_usage(av[0]) << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<uint32_t> nbrs_vec = parse_numbers(ac, av);
    std::list<uint32_t>   nbrs_lst(nbrs_vec.begin(), nbrs_vec.end());

    print_container("Before", nbrs_vec);
    double vec_time = benchmark(nbrs_vec, sort_vector);
    print_container("After", nbrs_vec);
    double lst_time = benchmark(nbrs_lst, sort_list);

    print_result("std::vector", vec_time);
    print_result("std::list", lst_time);
    return EXIT_SUCCESS;
}
