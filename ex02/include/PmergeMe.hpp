#pragma once

#include <list>
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
void                  jacobsthal_insert(std::list<uint32_t>& lst, uint32_t n);
void                  sort(std::vector<uint32_t>& vec);
void                  sort(std::list<uint32_t>& lst);
size_t                get_comparison_count();

}; // namespace PmergeMe
