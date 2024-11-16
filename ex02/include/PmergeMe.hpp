#pragma once

#include <list>
#include <vector>

struct Pair
{
    Pair(int small, int large) : small(small), large(large)
    {
    }
    ~Pair()
    {
    }

    int small;
    int large;
};

class PmergeMe
{
public:
    static void sortVector(std::vector<int> &vec);
    static void sortList(std::list<int> &lst);

private:
    PmergeMe();
    PmergeMe(const PmergeMe &other);
    PmergeMe &operator=(const PmergeMe &other);
    ~PmergeMe();

    static std::vector<Pair> makeVectorPairs(std::vector<int> &vec);
    static std::list<Pair>   makeListPairs(std::list<int> &lst);
};
