#include <stdint.h>

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "PmergeMe.hpp"

int main(int ac, char** av) {
    if (ac < 2) {
        std::cerr << print_usage(av[0]) << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<uint32_t> numbers_vector = parse_numbers(ac, av);
    std::list<uint32_t>   numbers_list(numbers_vector.begin(), numbers_vector.end());

    util::print_container("Before", numbers_vector);

    double vector_sorting_duration = util::benchmark(numbers_vector, sort_vector);
    double list_sorting_duration   = util::benchmark(numbers_list, sort_list);

    util::print_container("After", numbers_vector);

    util::print_result("std::vector", vector_sorting_duration);
    util::print_result("std::list", list_sorting_duration);

    assert(util::is_sorted(numbers_vector));
    assert(util::is_sorted(numbers_list));

    return EXIT_SUCCESS;
}
