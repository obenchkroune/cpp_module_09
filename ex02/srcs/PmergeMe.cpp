#include "PmergeMe.hpp"
#include <iostream>

std::vector<int> PmergeMe::sort(std::vector<int> &vec)
{
    if (vec.size() == 1) return vec;
	std::vector<int> left, right;
    std::vector<int>::iterator middle = vec.begin();
    std::advance(middle, vec.size() / 2);

    left.insert(left.end(), vec.begin(), middle);
    right.insert(right.end(), middle, vec.end());

    left = PmergeMe::sort(left);
    right = PmergeMe::sort(right);
    
    return PmergeMe::merge(left, right);
}

std::list<int> PmergeMe::sort(std::list<int> &lst)
{
	if (lst.size() == 1) return lst;
	std::list<int> left, right;
    std::list<int>::iterator middle = lst.begin();
    std::advance(middle, lst.size() / 2);

    left.insert(left.end(), lst.begin(), middle);
    right.insert(right.end(), middle, lst.end());

    left = PmergeMe::sort(left);
    right = PmergeMe::sort(right);
    
    return PmergeMe::merge(left, right);
}

std::vector<int> PmergeMe::merge(const std::vector<int> &left, const std::vector<int> &right)
{
	std::vector<int> result;

    std::vector<int>::const_iterator l_it = left.begin();
    std::vector<int>::const_iterator r_it = right.begin();

    while (l_it != left.end() && r_it != right.end())
    {
        if (*l_it < *r_it)
            result.push_back(*l_it++);
        else
            result.push_back(*r_it++);
    }
    result.insert(result.end(), r_it, right.end());
    result.insert(result.end(), l_it, left.end());
    return result;
}

std::list<int> PmergeMe::merge(const std::list<int> &left, const std::list<int> &right)
{
	std::list<int> result;

    std::list<int>::const_iterator l_it = left.begin();
    std::list<int>::const_iterator r_it = right.begin();

    while (l_it != left.end() && r_it != right.end())
    {
        if (*l_it < *r_it)
            result.push_back(*l_it++);
        else
            result.push_back(*r_it++);
    }
    result.insert(result.end(), r_it, right.end());
    result.insert(result.end(), l_it, left.end());
    return result;
}

PmergeMe::PmergeMe()
{
}

PmergeMe::PmergeMe(const PmergeMe &other)
{
	*this = other;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
	if (this != &other)
	{
	}
	return *this;
}

PmergeMe::~PmergeMe()
{
}
