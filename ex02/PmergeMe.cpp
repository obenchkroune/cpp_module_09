#include "PmergeMe.hpp"

#include <stdint.h>

#include <algorithm>
#include <cassert>
#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>

ListPendNode::ListPendNode(uint32_t value, std::list<uint32_t>::iterator it)
    : value(value), end_it(it), inserted(false) {}

VectorPendNode::VectorPendNode(uint32_t value, size_t idx) : value(value), inserted(false), idx(idx) {}

static void sort_pairs(std::vector<std::pair<uint32_t, uint32_t> >& pairs) {
    if (pairs.size() < 2) return;

    std::vector<std::pair<uint32_t, uint32_t> >::iterator mid = pairs.begin() + pairs.size() / 2;
    std::vector<std::pair<uint32_t, uint32_t> >           left(pairs.begin(), mid);
    std::vector<std::pair<uint32_t, uint32_t> >           right(mid, pairs.end());

    sort_pairs(left);
    sort_pairs(right);

    std::vector<std::pair<uint32_t, uint32_t> > result;
    size_t                                      i = 0, j = 0;
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

static std::vector<size_t> jacobsthal(size_t n) {
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

    for (size_t i = 0; i < vec.size(); i += 2) {
        if (vec[i] > vec[i + 1])
            pairs.push_back(std::make_pair(vec[i], vec[i + 1]));
        else
            pairs.push_back(std::make_pair(vec[i + 1], vec[i]));
    }

    sort_pairs(pairs);

    std::vector<uint32_t>       main;
    std::vector<VectorPendNode> pend;
    main.reserve(pairs.size() + 1);
    pend.reserve(pairs.size() + 1);

    typedef std::vector<std::pair<uint32_t, uint32_t> >::iterator PairIterator;

    PairIterator start = pairs.begin();

    main.push_back(start->second);
    main.push_back(start->first);

    size_t idx = 1;
    for (PairIterator it = start + 1; it != pairs.end(); ++it) {
        main.push_back(it->first);
        pend.push_back(VectorPendNode(it->second, idx++));
    }

    if (has_stray) {
        pend.push_back(VectorPendNode(stray, idx));
    }

    std::vector<size_t> squence = jacobsthal(pend.size());

    size_t chain_length = 1;
    for (size_t i = 0; i < squence.size(); ++i) {
        size_t idx = squence[i] - 1;
        if (idx >= pend.size()) break;

        std::vector<VectorPendNode>::iterator pend_it = util::next(pend.begin(), idx);

        std::vector<uint32_t>::iterator pos =
            std::upper_bound(main.begin(), util::next(main.begin(), chain_length + pend_it->idx), pend_it->value);

        main.insert(pos, pend_it->value);
        pend_it->inserted = true;
        ++chain_length;
    }

    for (std::vector<VectorPendNode>::iterator pend_it = pend.begin(); pend_it != pend.end(); ++pend_it) {
        if (pend_it->inserted) continue;

        std::vector<uint32_t>::iterator pos =
            std::upper_bound(main.begin(), util::next(main.begin(), chain_length + pend_it->idx), pend_it->value);
        main.insert(pos, pend_it->value);

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

void sort_list(std::list<uint32_t>& lst) {
    if (lst.size() < 2) return;
    std::list<std::pair<uint32_t, uint32_t> > pairs;

    bool     has_stray = (lst.size() % 2 != 0);
    uint32_t stray     = 0;
    if (has_stray) {
        stray = lst.back();
        lst.pop_back();
    }

    for (std::list<uint32_t>::iterator it = lst.begin(); it != lst.end(); std::advance(it, 2)) {
        if (*it > *util::next(it))
            pairs.push_back(std::make_pair(*it, *util::next(it)));
        else
            pairs.push_back(std::make_pair(*util::next(it), *it));
    }

    sort_pairs(pairs);

    std::list<uint32_t>     main;
    std::list<ListPendNode> pend;

    std::list<std::pair<uint32_t, uint32_t> >::iterator start = pairs.begin();
    main.push_back(start->second);
    main.push_back(start->first);

    for (std::list<std::pair<uint32_t, uint32_t> >::iterator it = util::next(pairs.begin()); it != pairs.end(); ++it) {
        main.push_back(it->first);
        pend.push_back(ListPendNode(it->second, util::next(main.end(), -1)));
    }

    if (has_stray) {
        pend.push_back(ListPendNode(stray, util::next(main.end(), -1)));
    }

    std::vector<size_t> jacobsthal_indices = jacobsthal(pend.size());

    std::list<ListPendNode>::iterator value_it = pend.begin();
    size_t                            last_idx = 0;

    for (size_t i = 0; i < jacobsthal_indices.size(); ++i) {
        size_t idx = jacobsthal_indices[i] - 1;
        if (idx >= pend.size()) break;

        std::advance(value_it, idx - last_idx);
        last_idx                          = idx;
        std::list<uint32_t>::iterator pos = std::upper_bound(main.begin(), value_it->end_it, value_it->value);
        // std::cout << value << ": search space from " << *start << " to " << *end << std::endl;
        // print_container("chain", main);
        main.insert(pos, value_it->value);
        value_it->inserted = true;
    }

    for (std::list<ListPendNode>::iterator it = pend.begin(); it != pend.end(); ++it) {
        if (it->inserted) continue;

        std::list<uint32_t>::iterator pos = std::upper_bound(main.begin(), it->end_it, it->value);
        // std::cout << it->value << ": search space from " << *main.begin() << " to " << *it->end_it << std::endl;
        // util::print_container("chain", main);
        main.insert(pos, it->value);
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

void util::print_result(const char* container_name, double time_taken) {
    std::cout << "Time to process a range of 5 elements with " << container_name << "\t: " << time_taken << " µs"
              << std::endl;
}
