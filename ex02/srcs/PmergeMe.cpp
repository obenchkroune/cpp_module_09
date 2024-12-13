#include "PmergeMe.hpp"
#include <iostream>
#include <math.h>

namespace PmergeMe {

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
        if (!std::isdigit(ss.peek())) {
            throw std::runtime_error(INVALID_INPUT);
        }
        uint32_t n;
        ss >> n;
        vec.push_back(n);
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
    }
    vec.insert(vec.begin() + left, n);
}

void jacobsthal_insert(std::deque<uint32_t>& deq, uint32_t n) {
    if (deq.size() == 0) {
        deq.push_back(n);
        return;
    }

    std::size_t left = 0, right = deq.size();
    while (left < right) {
        std::size_t mid = left + jacobsthal(static_cast<int>(log2(right - left)));

        if (mid >= right) {
            mid = right - 1;
        }

        if (deq[mid] < n) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    deq.insert(deq.begin() + left, n);
}

void sort(std::vector<uint32_t>& vec) {
    if (vec.size() < 2) {
        return;
    }

    bool     is_odd = vec.size() % 2;
    uint32_t odd_nbr;

    if (is_odd) {
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

void sort(std::deque<uint32_t>& lst) {
    if (lst.size() < 2) {
        return;
    }

    bool     is_odd = lst.size() % 2;
    uint32_t odd_nbr;

    if (is_odd) {
        odd_nbr = lst.back();
        lst.pop_back();
    }

    std::deque<uint32_t> smaller_values;
    std::deque<uint32_t> larger_values;

    for (std::size_t i = 0; i < lst.size(); i += 2) {
        if (lst[i] < lst[i + 1]) {
            larger_values.push_back(lst[i + 1]);
            smaller_values.push_back(lst[i]);
        } else {
            larger_values.push_back(lst[i]);
            smaller_values.push_back(lst[i + 1]);
        }
    }
    sort(larger_values);
    lst.swap(larger_values);

    for (std::size_t i = 0; i < smaller_values.size(); i++) {
        jacobsthal_insert(lst, smaller_values[i]);
    }

    if (is_odd) {
        jacobsthal_insert(lst, odd_nbr);
    }
}

bool is_equal(const std::vector<uint32_t>& vec, const std::deque<uint32_t>& deq) {
    if (vec.size() != deq.size()) {
        return false;
    }

    for (std::size_t i = 0; i < vec.size(); i++) {
        if (vec[i] != deq[i]) {
            return false;
        }
    }
    return true;
}

bool is_sorted(const std::vector<uint32_t>& vec) {
    for (std::size_t i = 0; i < vec.size() - 1; i++) {
        if (vec[i] > vec[i + 1]) {
            return false;
        }
    }
    return true;
}

}; // namespace PmergeMe
