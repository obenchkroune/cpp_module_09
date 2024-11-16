#include "PmergeMe.hpp"

#include <iostream>

PmergeMe::PmergeMe()
{
}

PmergeMe::~PmergeMe()
{
}

std::vector<Pair> PmergeMe::makeVectorPairs(std::vector<int> &vec)
{
    std::vector<Pair> result;

    result.reserve(vec.size() / 2);
    std::vector<int>::iterator it = vec.begin();
    for (; it != vec.end(); it += 2)
    {
        if (*it < *(it + 1))
            result.push_back(Pair(*it, *(it + 1)));
        else
            result.push_back(Pair(*(it + 1), *it));
    }
    return result;
}

void printPairs(const std::vector<Pair> &pairs)
{
    for (std::vector<Pair>::const_iterator it = pairs.begin(); it != pairs.end(); it++)
        std::cout << it->large << " | " << it->small << std::endl;
}

void PmergeMe::sortVector(std::vector<int> &vec)
{
    if (vec.size() <= 1)
        return;

    bool has_odd     = vec.size() % 2 != 0;
    int  odd_element = -1;

    if (has_odd)
    {
        odd_element = vec.back();
        vec.pop_back();
    }

    std::vector<Pair> pairs = makeVectorPairs(vec);
    std::vector<int>  larger;

    larger.reserve(pairs.size());
    for (std::vector<Pair>::iterator it = pairs.begin(); it != pairs.end(); it++)
        larger.push_back(it->large);
    sortVector(larger);

    vec.clear();
    vec = larger;
    for (std::vector<Pair>::iterator it = pairs.begin(); it != pairs.end(); it++)
    {
        std::vector<int>::iterator pos = std::lower_bound(vec.begin(), vec.end(), it->small);
        vec.insert(pos, it->small);
    }
    if (has_odd)
    {
        std::vector<int>::iterator pos = std::lower_bound(vec.begin(), vec.end(), odd_element);
        vec.insert(pos, odd_element);
    }
}

std::list<Pair> PmergeMe::makeListPairs(std::list<int> &lst)
{
    std::list<Pair> result;

    std::list<int>::iterator it = lst.begin();
    while (it != lst.end())
    {
        std::list<int>::iterator next = it;
        std::advance(next, 1);
        if (*it < *next)
            result.push_back(Pair(*it, *next));
        else
            result.push_back(Pair(*next, *it));
        std::advance(it, 2);
    }
    return result;
}

void PmergeMe::sortList(std::list<int> &lst)
{
    if (lst.size() <= 1)
        return;

    bool has_odd     = lst.size() % 2 != 0;
    int  odd_element = -1;

    if (has_odd)
    {
        odd_element = lst.back();
        lst.pop_back();
    }

    std::list<Pair> pairs = makeListPairs(lst);
    std::list<int>  larger;

    for (std::list<Pair>::iterator it = pairs.begin(); it != pairs.end(); it++)
        larger.push_back(it->large);
    sortList(larger);

    lst.clear();
    lst = larger;
    for (std::list<Pair>::iterator it = pairs.begin(); it != pairs.end(); it++)
    {
        std::list<int>::iterator pos = std::lower_bound(lst.begin(), lst.end(), it->small);
        lst.insert(pos, it->small);
    }
    if (has_odd)
    {
        std::list<int>::iterator pos = std::lower_bound(lst.begin(), lst.end(), odd_element);
        lst.insert(pos, odd_element);
    }
}
