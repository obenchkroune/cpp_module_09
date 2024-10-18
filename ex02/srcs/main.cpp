#include <iostream>
#include "include/PmergeMe.hpp"
#include <sstream>
#include <ctime>

bool parse_input(int ac, char **av, std::vector<int> &result)
{
	std::string input;

	if (ac < 2)
		return false;
	for (int i = 1; i < ac; i++)
	{
		input += av[i];
		input += ' ';
	}
	
	std::istringstream iss(input); 
	int nbr;

	while (!(iss >> std::ws).eof())
	{
		iss >> nbr;
		if (iss.fail())
			return false;
		result.push_back(nbr);
	}
	return true;
}

int main(int ac, char **av)
{
	std::vector<int> vec;
	double vec_time, lst_time;
	

	if (!parse_input(ac, av, vec))
	{
		std::cerr << "Error" << std::endl;
		return 1;
	}

	std::list<int> lst(vec.begin(), vec.end());

	printData("Before", vec);

	std::clock_t start = std::clock();
	vec = PmergeMe::sort(vec);
	std::clock_t end = std::clock();

	vec_time = double(end - start) / CLOCKS_PER_SEC;

	start = std::clock();
	lst = PmergeMe::sort(lst);
	end = std::clock();

	lst_time = double(end - start) / CLOCKS_PER_SEC;

	printData("After", vec);
	std::cout << std::fixed
		<< "Time to process a range of " << vec.size()
		<< " elements with std::vector : " << vec_time << " us\n"
		<< "Time to process a range of " << lst.size()
		<< " elements with std::lst : " << lst_time << " us\n";

	return 0;
}
