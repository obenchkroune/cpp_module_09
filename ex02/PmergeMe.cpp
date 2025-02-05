#include "PmergeMe.hpp"

#include <stdint.h>

#include <cassert>
#include <ctime>
#include <iostream>
#include <list>
#include <sstream>
#include <vector>

static void sort_pairs(std::vector<std::pair<uint32_t, uint32_t> >& pairs) {
    if (pairs.size() < 2) return;

    std::vector<std::pair<uint32_t, uint32_t> >::iterator mid = pairs.begin() + pairs.size() / 2;

    std::vector<std::pair<uint32_t, uint32_t> > left(pairs.begin(), mid);
    std::vector<std::pair<uint32_t, uint32_t> > right(mid, pairs.end());

    sort_pairs(left);
    sort_pairs(right);

    std::vector<std::pair<uint32_t, uint32_t> > result;

    std::size_t i = 0, j = 0;
    while (i < left.size() && j < right.size()) {
        if (left[i].first < right[j].first) {
            result.push_back(left[i++]);
        } else {
            result.push_back(right[j++]);
        }
    }
    result.insert(result.end(), left.begin() + i, left.end());
    result.insert(result.end(), right.begin() + j, right.end());
    pairs.swap(result);
}

static void sort_winners(std::list<std::pair<uint32_t, uint32_t> >& pairs) {
    if (pairs.size() < 2) return;

    std::list<std::pair<uint32_t, uint32_t> >::iterator mid = pairs.begin();
    std::advance(mid, pairs.size() / 2);

    std::list<std::pair<uint32_t, uint32_t> > left(pairs.begin(), mid);
    std::list<std::pair<uint32_t, uint32_t> > right(mid, pairs.end());

    sort_winners(left);
    sort_winners(right);

    std::list<std::pair<uint32_t, uint32_t> > result;

    std::list<std::pair<uint32_t, uint32_t> >::iterator left_it  = left.begin(),
                                                        right_it = right.begin();

    while (left_it != left.end() && right_it != right.end()) {
        if (left_it->first < right_it->first) {
            result.push_back(*left_it++);
        } else {
            result.push_back(*right_it++);
        }
    }
    result.insert(result.end(), left_it, left.end());
    result.insert(result.end(), right_it, right.end());
    pairs.swap(result);
}

std::vector<std::size_t> jacobsthal(int n) {
    // J(n)=J(n−1)+2J(n−2)
    std::vector<std::size_t> numbers;
    numbers.reserve(n);
    numbers.push_back(3);
    numbers.push_back(3);
    for (int i = 5; i < n; ++i) {
        numbers.push_back(numbers[i - 1] + 2 * numbers[i - 2]);
    }
    return numbers;
}

void sort_vector(std::vector<uint32_t>& vec) {
    if (vec.size() < 2) return;
    std::vector<std::pair<uint32_t, uint32_t> > pairs;

    bool     has_stray = vec.size() % 2;
    uint32_t stray;

    if (has_stray) {
        stray = vec.back();
        vec.pop_back();
    }

    for (std::size_t i = 0; i < vec.size(); i += 2) {
        if (vec[i] > vec[i + 1]) {
            pairs.push_back(std::make_pair(vec[i], vec[i + 1]));
        } else {
            pairs.push_back(std::make_pair(vec[i + 1], vec[i]));
        }
    }

    sort_pairs(pairs);

    std::vector<uint32_t> main;
    std::vector<uint32_t> pend;

    main.reserve(pairs.size() * 2 + has_stray);
    pend.reserve(pairs.size() + has_stray);

    std::vector<std::pair<uint32_t, uint32_t> >::iterator pair_it = pairs.begin();
    for (; pair_it != pairs.end(); ++pair_it) {
        main.push_back(pair_it->first);
        pend.push_back(pair_it->second);
    }

    if (has_stray) pend.push_back(stray);

    std::vector<std::size_t> jacobsthal_indices = jacobsthal(pend.size());

    for (std::size_t i = 0; i < jacobsthal_indices.size(); ++i) {
        std::size_t index = jacobsthal_indices[i];
        if (index >= pend.size()) break;

        uint32_t value = pend[index];

        std::vector<uint32_t>::iterator pos = std::lower_bound(main.begin(), main.end(), value);

        main.insert(pos, value);
        pend.erase(pend.begin() + index);
    }

    for (std::size_t i = 0; i < pend.size(); ++i) {
        uint32_t value = pend[i];

        std::vector<uint32_t>::iterator pos = std::lower_bound(main.begin(), main.end(), value);

        main.insert(pos, value);
    }

    vec.swap(main);
}

void sort_list(std::list<uint32_t>& lst) {
    if (lst.size() < 2) return;
    std::list<std::pair<uint32_t, uint32_t> > pairs;

    bool     has_stray = lst.size() % 2;
    uint32_t stray;

    if (has_stray) {
        stray = lst.back();
        lst.pop_back();
    }

    for (std::list<uint32_t>::iterator it = lst.begin(); it != lst.end(); ++it) {
        uint32_t prev = *it++;
        if (prev > *it) {
            pairs.push_back(std::make_pair(prev, *it));
        } else {
            pairs.push_back(std::make_pair(*it, prev));
        }
    }

    sort_winners(pairs);

    std::list<uint32_t> main;
    std::list<uint32_t> pend;

    std::list<std::pair<uint32_t, uint32_t> >::iterator pair_it = pairs.begin();
    for (; pair_it != pairs.end(); ++pair_it) {
        main.push_back(pair_it->first);
        pend.push_back(pair_it->second);
    }
    if (has_stray) pend.push_back(stray);

    std::vector<std::size_t> jacobsthal_indices = jacobsthal(pend.size());

    for (std::size_t i = 0; i < jacobsthal_indices.size(); ++i) {
        std::size_t index = jacobsthal_indices[i];
        if (index >= pend.size()) break;

        std::list<uint32_t>::iterator it = pend.begin();
        std::advance(it, index);
        uint32_t value = *it;

        std::list<uint32_t>::iterator pos = std::lower_bound(main.begin(), main.end(), value);

        main.insert(pos, value);
        pend.erase(it);
    }

    for (std::list<uint32_t>::iterator it = pend.begin(); it != pend.end(); ++it) {
        uint32_t value = *it;

        std::list<uint32_t>::iterator pos = std::lower_bound(main.begin(), main.end(), value);

        main.insert(pos, value);
    }

    lst.swap(main);
}

std::string print_usage(const char* progname) {
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

void print_result(const char* container_name, double time_taken) {
    std::cout << "Time to process a range of 5 elements with " << container_name
              << "\t: " << time_taken << " us" << std::endl;
}