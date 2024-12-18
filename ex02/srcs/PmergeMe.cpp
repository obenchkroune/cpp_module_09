#include "PmergeMe.hpp"

#include <iostream>
#include <math.h>
#include <sstream>
#include <stdexcept>

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
        std::size_t mid = left + jacobsthal(log2(right - left));

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

void jacobsthal_insert(std::list<uint32_t>& lst, uint32_t n) {
    if (lst.size() == 0) {
        lst.push_back(n);
        return;
    }
    std::size_t left = 0, right = lst.size();
    while (left < right) {
        std::size_t mid = left + jacobsthal(log2(right - left));

        if (mid >= right) {
            mid = right - 1;
        }

        std::list<uint32_t>::iterator it = lst.begin();
        std::advance(it, mid);

        if (*it < n) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    std::list<uint32_t>::iterator it = lst.begin();
    std::advance(it, left);
    lst.insert(it, n);
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

void sort(std::list<uint32_t>& lst) {
    if (lst.size() < 2) {
        return;
    }

    bool     is_odd = lst.size() % 2;
    uint32_t odd_nbr;

    if (is_odd) {
        odd_nbr = lst.back();
        lst.pop_back();
    }

    std::list<uint32_t> smaller_values;
    std::list<uint32_t> larger_values;

    std::list<uint32_t>::iterator it = lst.begin();
    while (it != lst.end()) {
        std::list<uint32_t>::iterator next = it;
        ++next;

        if (*it < *next) {
            larger_values.push_back(*next);
            smaller_values.push_back(*it);
        } else {
            larger_values.push_back(*it);
            smaller_values.push_back(*next);
        }
        ++it;
        ++it;
    }
    sort(larger_values);
    lst.swap(larger_values);

    it = smaller_values.begin();
    for (; it != smaller_values.end(); ++it) {
        jacobsthal_insert(lst, *it);
    }

    if (is_odd) {
        jacobsthal_insert(lst, odd_nbr);
    }
}

bool is_equal(const std::vector<uint32_t>& vec, const std::list<uint32_t>& lst) {
    if (vec.size() != lst.size()) {
        return false;
    }

    std::list<uint32_t>::const_iterator   lst_it = lst.begin();
    std::vector<uint32_t>::const_iterator vec_it = vec.begin();

    while (lst_it != lst.end() && vec_it != vec.end()) {
        if (*lst_it != *vec_it) {
            return false;
        }
        ++lst_it;
        ++vec_it;
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
