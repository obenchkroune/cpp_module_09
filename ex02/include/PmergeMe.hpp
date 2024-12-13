#pragma once

#include <ctime>
#include <deque>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <stdint.h>
#include <vector>

#define USAGE         "Usage: PmergeMe <numbers> [numbers ...]"
#define INVALID_INPUT "invalid input"

namespace PmergeMe {

std::vector<uint32_t> parse_args(int ac, char** av);
int                   jacobsthal(int n);
void                  jacobsthal_insert(std::vector<uint32_t>& vec, uint32_t n);
void                  jacobsthal_insert(std::deque<uint32_t>& deq, uint32_t n);
void                  sort(std::vector<uint32_t>& vec);
void                  sort(std::deque<uint32_t>& deq);
bool                  is_equal(const std::vector<uint32_t>& vec, const std::deque<uint32_t>& deq);
bool                  is_sorted(const std::vector<uint32_t>& vec);

template <typename Container>
void print_container(const Container& data) {
    for (typename Container::const_iterator it = data.begin(); it != data.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

template <typename Container>
double benchmark(void (*sort)(Container&), Container& data) {
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    sort(data);
    clock_gettime(CLOCK_MONOTONIC, &end);
    return ((end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1e3);
}

}; // namespace PmergeMe
