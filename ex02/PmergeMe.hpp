#pragma once

#include <stdint.h>
#include <sys/types.h>

#include <ctime>
#include <iostream>
#include <list>
#include <vector>

struct ListPendNode {
    uint32_t                      value;
    std::list<uint32_t>::iterator end_it;
    bool                          inserted;

    ListPendNode(uint32_t value, std::list<uint32_t>::iterator it);
};

struct VectorPendNode {
    uint32_t value;
    bool     inserted;
    size_t   idx;

    VectorPendNode(uint32_t value, size_t idx);
};

void                  sort_vector(std::vector<uint32_t>& vec);
void                  sort_list(std::list<uint32_t>& lst);
std::vector<uint32_t> parse_numbers(int ac, char** av);
std::string           print_usage(const char* progname);

// ----------------------------------------------------------
// utils
// ----------------------------------------------------------

namespace util {

void print_result(const char* container_name, double time_taken);

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
Iterator next(Iterator it, ssize_t n = 1) {
    Iterator result = it;
    std::advance(result, n);
    return result;
}

template <typename Container>
bool is_sorted(Container& container) {
    typename Container::iterator it = container.begin();
    if (it == container.end()) return true;
    typename Container::value_type prev = *it++;
    for (; it != container.end(); ++it) {
        if (prev > *it) return false;
        prev = *it;
    }
    return true;
}

};  // namespace util
