#include "PmergeMe.hpp"

#include <stdint.h>

#include <cassert>
#include <list>
#include <vector>

static void sort_winners(std::vector<std::pair<uint32_t, uint32_t> >& pairs) {
    if (pairs.size() < 2) return;

    std::vector<std::pair<uint32_t, uint32_t> >::iterator mid = pairs.begin() + pairs.size() / 2;

    std::vector<std::pair<uint32_t, uint32_t> > left(pairs.begin(), mid);
    std::vector<std::pair<uint32_t, uint32_t> > right(mid, pairs.end());

    sort_winners(left);
    sort_winners(right);

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

    std::list<std::pair<uint32_t, uint32_t> >::iterator left_it = left.begin(), right_it = right.begin();

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
    numbers.push_back(0);
    numbers.push_back(1);
    for (int i = 2; i < n; ++i) {
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

    sort_winners(pairs);

    std::vector<uint32_t> main_chain;
    std::vector<uint32_t> secondary_chain;

    main_chain.reserve(pairs.size() * 2 + has_stray);
    secondary_chain.reserve(pairs.size() + has_stray);

    for (std::vector<std::pair<uint32_t, uint32_t> >::iterator it = pairs.begin(); it != pairs.end(); ++it) {
        main_chain.push_back(it->first);
        secondary_chain.push_back(it->second);
    }
    if (has_stray) secondary_chain.push_back(stray);

    std::vector<std::size_t> jacobsthal_indices = jacobsthal(secondary_chain.size());

    for (std::size_t i = 0; i < jacobsthal_indices.size(); ++i) {
        std::size_t index = jacobsthal_indices[i];
        if (index >= secondary_chain.size()) break;

        uint32_t value = secondary_chain[index];

        std::vector<uint32_t>::iterator pos = std::lower_bound(main_chain.begin(), main_chain.end(), value);

        main_chain.insert(pos, value);
        secondary_chain.erase(secondary_chain.begin() + index);
    }

    for (std::size_t i = 0; i < secondary_chain.size(); ++i) {
        uint32_t value = secondary_chain[i];

        std::vector<uint32_t>::iterator pos = std::lower_bound(main_chain.begin(), main_chain.end(), value);

        main_chain.insert(pos, value);
    }

    vec.swap(main_chain);
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

    std::list<uint32_t> main_chain;
    std::list<uint32_t> secondary_chain;

    for (std::list<std::pair<uint32_t, uint32_t> >::iterator it = pairs.begin(); it != pairs.end(); ++it) {
        main_chain.push_back(it->first);
        secondary_chain.push_back(it->second);
    }
    if (has_stray) secondary_chain.push_back(stray);

    std::vector<std::size_t> jacobsthal_indices = jacobsthal(secondary_chain.size());

    for (std::size_t i = 0; i < jacobsthal_indices.size(); ++i) {
        std::size_t index = jacobsthal_indices[i];
        if (index >= secondary_chain.size()) break;

        std::list<uint32_t>::iterator it = secondary_chain.begin();
        std::advance(it, index);
        uint32_t value = *it;

        std::list<uint32_t>::iterator pos = std::lower_bound(main_chain.begin(), main_chain.end(), value);

        main_chain.insert(pos, value);
        secondary_chain.erase(it);
    }

    for (std::list<uint32_t>::iterator it = secondary_chain.begin(); it != secondary_chain.end(); ++it) {
        uint32_t value = *it;

        std::list<uint32_t>::iterator pos = std::lower_bound(main_chain.begin(), main_chain.end(), value);

        main_chain.insert(pos, value);
    }

    lst.swap(main_chain);
}
