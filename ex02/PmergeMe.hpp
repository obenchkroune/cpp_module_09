#pragma once

#include <stdint.h>

#include <ctime>
#include <iostream>
#include <list>
#include <vector>

void                  sort_vector(std::vector<uint32_t>& vec);
void                  sort_list(std::list<uint32_t>& lst);
void                  print_result(const char* container_name, double time_taken);
std::vector<uint32_t> parse_numbers(int ac, char** av);
std::string           print_usage(const char* progname);

// ----------------------------------------------------------
// templates
// ----------------------------------------------------------
template <typename Tp>
double benchmark(Tp& nbrs, void (*sort_fn)(Tp&)) {
    timespec start, end;

    clock_gettime(CLOCK_REALTIME, &start);
    sort_fn(nbrs);
    clock_gettime(CLOCK_REALTIME, &end);
    return (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3;
}

template <typename Tp>
void print_container(const char* label, const Tp& vec) {
    std::cout << label << ":\t";
    for (typename Tp::const_iterator it = vec.begin(); it != vec.end(); ++it) {
        std::cout << *it << ", ";
    }
    std::cout << std::endl;
}

template <typename Iterator>
Iterator next(Iterator it, size_t n = 1) {
    Iterator result = it;
    std::advance(result, n);
    return result;
}
