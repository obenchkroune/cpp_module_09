#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>

#include "PmergeMe.hpp"

#define USAGE "Usage: PmergeMe <numbers> [numbers ...]"

inline bool parse_args(int ac, char **av, std::vector<int> &vec)
{
    std::string input;
    for (int i = 1; i < ac; ++i)
    {
        input += av[i];
        input += ' ';
    }

    std::istringstream iss(input);
    int                nbr;

    while (iss >> std::ws && !iss.eof())
    {
        iss >> nbr;
        if (iss.fail() || nbr < 0)
            return false;
        vec.push_back(nbr);
    }
    return true;
}

inline double get_time_ms(const timespec &start, const timespec &end)
{
    return (end.tv_sec - start.tv_sec) * 1000000 +
           static_cast<double>(end.tv_nsec - start.tv_nsec) / 1000;
}

inline void print_vector(const std::vector<int> &vec)
{
    for (size_t i = 0; i < vec.size(); i++)
        std::cout << vec[i] << ' ';
    std::cout << std::endl;
}

int main(int ac, char **av)
{
    if (ac < 2)
    {
        std::cout << USAGE << std::endl;
        return 1;
    }

    std::vector<int> vec;
    if (!parse_args(ac, av, vec))
        return 1;

    std::cout << "Before : ";
    print_vector(vec);

    timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    PmergeMe::sortVector(vec);
    clock_gettime(CLOCK_MONOTONIC, &end);

    double         vec_time = get_time_ms(start, end);
    std::list<int> lst(vec.begin(), vec.end());

    clock_gettime(CLOCK_MONOTONIC, &start);
    PmergeMe::sortList(lst);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double lst_time = get_time_ms(start, end);

    std::cout << "After : ";
    print_vector(vec);

    std::cout << "Time to process a range of " << vec.size()
              << " elements with std::vector : " << vec_time << " us" << std::endl;
    std::cout << "Time to process a range of " << vec.size()
              << " elements with std::list   : " << lst_time << " us" << std::endl;
}
