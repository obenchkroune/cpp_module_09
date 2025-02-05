#include "PmergeMe.hpp"

#include <stdint.h>

#include <algorithm>
#include <cassert>
#include <ctime>
#include <iostream>
#include <list>
#include <sstream>
#include <vector>

static void sort_pairs(std::vector<std::pair<uint32_t, uint32_t> >& pairs) {
    if (pairs.size() < 2) return;

    std::vector<std::pair<uint32_t, uint32_t> >::iterator mid = pairs.begin() + pairs.size() / 2;
    std::vector<std::pair<uint32_t, uint32_t> >           left(pairs.begin(), mid);
    std::vector<std::pair<uint32_t, uint32_t> >           right(mid, pairs.end());

    sort_pairs(left);
    sort_pairs(right);

    std::vector<std::pair<uint32_t, uint32_t> > result;
    std::size_t                                 i = 0, j = 0;
    while (i < left.size() && j < right.size()) {
        if (left[i].first < right[j].first) {
            result.push_back(left[i]);
            i++;
        } else {
            result.push_back(right[j]);
            j++;
        }
    }
    result.insert(result.end(), left.begin() + i, left.end());
    result.insert(result.end(), right.begin() + j, right.end());
    pairs.swap(result);
}

static std::vector<std::size_t> jacobsthal(std::size_t n) {
    std::vector<size_t> res;

    res.push_back(3);
    res.push_back(5);

    size_t idx = 2;
    while (1) {
        size_t next = res[idx - 1] + 2 * res[idx - 2];
        if (next >= n) {
            break;
        }
        res.push_back(next);
        ++idx;
    }
    return res;
}

void sort_vector(std::vector<uint32_t>& vec) {
    if (vec.size() < 2) return;
    std::vector<std::pair<uint32_t, uint32_t> > pairs;

    bool     has_stray = (vec.size() % 2 != 0);
    uint32_t stray     = 0;
    if (has_stray) {
        stray = vec.back();
        vec.pop_back();
    }

    for (std::size_t i = 0; i < vec.size(); i += 2) {
        if (vec[i] > vec[i + 1])
            pairs.push_back(std::make_pair(vec[i], vec[i + 1]));
        else
            pairs.push_back(std::make_pair(vec[i + 1], vec[i]));
    }

    sort_pairs(pairs);

    std::vector<uint32_t> main;
    std::vector<uint32_t> pend;
    main.reserve(pairs.size() + 1);
    pend.reserve(pairs.size() + 1);

    typedef std::vector<std::pair<uint32_t, uint32_t> >::const_iterator PairConstIterator;
    for (PairConstIterator it = pairs.begin(); it != pairs.end(); ++it) {
        main.push_back(it->first);
        pend.push_back(it->second);
    }

    if (has_stray) pend.push_back(stray);

    std::vector<std::size_t> jacobsthal_indices = jacobsthal(pend.size());
    std::vector<bool>        inserted(pend.size(), false);

    size_t chain_length = 0;

    for (std::size_t i = 0; i < jacobsthal_indices.size(); ++i) {
        std::size_t idx = jacobsthal_indices[i] - 1;
        if (idx >= pend.size()) break;
        uint32_t value = pend[idx];
        inserted[idx]  = true;

        std::vector<uint32_t>::iterator start = main.begin();
        std::vector<uint32_t>::iterator end   = main.begin() + idx + chain_length;

        std::vector<uint32_t>::iterator pos = std::upper_bound(start, end, value);
        // std::cout << value << ": search space from " << *start << " to " << *end << std::endl;
        // print_container("chain", main);
        main.insert(pos, value);
        ++chain_length;
    }

    for (std::size_t idx = 0; idx < pend.size(); ++idx) {
        if (inserted[idx] == true) continue;

        uint32_t value = pend[idx];

        std::vector<uint32_t>::iterator start = main.begin();
        std::vector<uint32_t>::iterator end   = main.begin() + idx + chain_length;
        // std::cout << value << ": search space from " << *start << " to " << *end << std::endl;
        // print_container("chain", main);
        std::vector<uint32_t>::iterator pos = std::upper_bound(start, end, value);
        main.insert(pos, value);
        ++chain_length;
    }

    vec.swap(main);
}

static void sort_pairs(std::list<std::pair<uint32_t, uint32_t> >& pairs) {
    if (pairs.size() < 2) return;

    std::list<std::pair<uint32_t, uint32_t> >::iterator mid = pairs.begin();
    std::advance(mid, pairs.size() / 2);

    std::list<std::pair<uint32_t, uint32_t> > left(pairs.begin(), mid);
    std::list<std::pair<uint32_t, uint32_t> > right(mid, pairs.end());

    sort_pairs(left);
    sort_pairs(right);

    std::list<std::pair<uint32_t, uint32_t> > result;

    std::list<std::pair<uint32_t, uint32_t> >::iterator left_it  = left.begin();
    std::list<std::pair<uint32_t, uint32_t> >::iterator right_it = right.begin();

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

void sort_list(std::list<uint32_t>& vec) {
    if (vec.size() < 2) return;
    std::list<std::pair<uint32_t, uint32_t> > pairs;

    bool     has_stray = (vec.size() % 2 != 0);
    uint32_t stray     = 0;
    if (has_stray) {
        stray = vec.back();
        vec.pop_back();
    }

    typedef std::list<uint32_t>::const_iterator ListConstIterator;
    for (ListConstIterator it = vec.begin(); it != vec.end(); std::advance(it, 2)) {
        if (*it > *next(it))
            pairs.push_back(std::make_pair(*it, *next(it)));
        else
            pairs.push_back(std::make_pair(*next(it), *it));
    }

    sort_pairs(pairs);

    std::list<uint32_t> main;
    std::list<uint32_t> pend;

    typedef std::list<std::pair<uint32_t, uint32_t> >::const_iterator PairConstIterator;
    for (PairConstIterator it = pairs.begin(); it != pairs.end(); ++it) {
        main.push_back(it->first);
        pend.push_back(it->second);
    }

    if (has_stray) pend.push_back(stray);

    std::vector<std::size_t> jacobsthal_indices = jacobsthal(pend.size());
    std::vector<bool>        inserted(pend.size(), false);

    size_t chain_length = 0;
    for (std::size_t i = 0; i < jacobsthal_indices.size(); ++i) {
        std::size_t idx = jacobsthal_indices[i] - 1;
        if (idx >= pend.size()) break;
        ListConstIterator value_it = next(pend.begin(), idx);
        inserted[idx]              = true;

        std::list<uint32_t>::iterator pos =
            std::upper_bound(main.begin(), next(main.begin(), idx + chain_length), *value_it);
        // std::cout << value << ": search space from " << *start << " to " << *end << std::endl;
        // print_container("chain", main);
        main.insert(pos, *value_it);
        ++chain_length;
    }

    for (std::size_t idx = 0; idx < pend.size(); ++idx) {
        if (inserted[idx] == true) continue;

        ListConstIterator value_it = next(pend.begin(), idx);
        inserted[idx]              = true;

        std::list<uint32_t>::iterator pos =
            std::upper_bound(main.begin(), next(main.begin(), idx + chain_length), *value_it);
        // std::cout << value << ": search space from " << *start << " to " << *end << std::endl;
        // print_container("chain", main);
        main.insert(pos, *value_it);
        ++chain_length;
    }

    vec.swap(main);
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
    std::cout << "Time to process a range of 5 elements with " << container_name << "\t: " << time_taken << " µs"
              << std::endl;
}
