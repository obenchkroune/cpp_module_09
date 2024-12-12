#include "PmergeMe.hpp"
#include <math.h>

namespace PmergeMe {

static size_t comparison_count = 0;

std::vector<uint32_t> parse_args(int ac, char** av) {
    if (ac < 2) {
        throw std::runtime_error(USAGE);
    }

    std::stringstream ss;
    for (int i = 1; i < ac; ++i) {
        ss << av[i] << " ";
    }

    std::vector<uint32_t> vec;
    vec.reserve(ac);

    while (ss >> std::ws && !ss.eof()) {
        uint32_t n;
        if (ss >> n) {
            vec.push_back(n);
        } else {
            break;
        }
    }

    if (ss.fail()) {
        throw std::runtime_error(INVALID_INPUT);
    }
    return vec;
}

int jacobsthal(int n) {
    if (n == 0 || n == 1)
        return n;
    int a = 0, b = 1;
    for (int i = 2; i <= n; i++) {
        int c = b;
        b += 2 * a;
        a = c;
    }
    return b;
}

void jacobsthal_insert(std::vector<uint32_t>& vec, uint32_t n) {
    if (vec.size() == 0) {
        vec.push_back(n);
        return;
    }

    std::size_t left = 0, right = vec.size();
    while (left < right) {
        std::size_t mid = left + jacobsthal(floor(log2(right - left)));

        if (mid >= right) {
            mid = right - 1;
        }

        if (vec[mid] < n) {
            left = mid + 1;
        } else {
            right = mid;
        }
        comparison_count++;
    }
    vec.insert(vec.begin() + left, n);
}

void jacobsthal_insert(std::list<uint32_t>& lst, uint32_t n) {
    throw std::runtime_error("Not implemented");
}

void sort(std::vector<uint32_t>& vec) {
    if (vec.size() < 2) {
        return;
    }

    std::vector<uint32_t> sorted;
    bool                  is_odd = vec.size() % 2;
    uint32_t              odd_nbr;

    if (is_odd) {
        sorted.push_back(vec.back());
        odd_nbr = vec.back();
        vec.pop_back();
    }

    std::vector<uint32_t> smaller_values;
    std::vector<uint32_t> larger_values;
    smaller_values.reserve(vec.size() / 2);
    larger_values.reserve(vec.size() / 2);

    for (std::size_t i = 0; i < vec.size(); i += 2) {
        if (vec[i] < vec[i + 1]) {
            larger_values.push_back(vec[i + 1]);
            smaller_values.push_back(vec[i]);
        } else {
            larger_values.push_back(vec[i]);
            smaller_values.push_back(vec[i + 1]);
        }
        comparison_count++;
    }
    sort(larger_values);
    vec.swap(larger_values);

    for (std::size_t i = 0; i < smaller_values.size(); i++) {
        jacobsthal_insert(vec, smaller_values[i]);
    }

    if (is_odd) {
        jacobsthal_insert(vec, odd_nbr);
    }
}

void sort(std::list<uint32_t>& lst) {
    throw std::runtime_error("Not implemented");
}

size_t get_comparison_count() {
    return comparison_count;
}

void reset_comparison_count() {
    comparison_count = 0;
}

}; // namespace PmergeMe
