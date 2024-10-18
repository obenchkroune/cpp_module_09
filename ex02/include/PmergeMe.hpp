#pragma once

#include <vector>
#include <list>
#include <iostream>

template<typename Tp>
void printData(const std::string &title, const Tp &data)
{
	std::cout << title << ": ";
	for (typename Tp::const_iterator it = data.begin(); it != data.end(); it++)
	{
		std::cout << *it << ' ';
	}
	std::cout << std::endl;
}

class PmergeMe
{
public:
	static std::vector<int> sort(std::vector<int> &vec);
	static std::list<int> sort(std::list<int> &lst);

private:
	PmergeMe();
	PmergeMe(const PmergeMe &other);
	PmergeMe &operator=(const PmergeMe &other);
	~PmergeMe();

	static std::vector<int> merge(const std::vector<int> &left, const std::vector<int> &right);
	static std::list<int> merge(const std::list<int> &left, const std::list<int> &right);
};
