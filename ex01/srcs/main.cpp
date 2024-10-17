#include "RPN.hpp"
#include <iostream>

int main(int ac, char **av)
{
	std::string input;

	if (ac < 2)
		return 1;

	for (int i = 1; i < ac; ++i)
		input += av[i];

	try
	{
		std::cout << RPN::evaluateExpression(input) << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error" << std::endl;
		return 1;
	}
	return 0;
}
