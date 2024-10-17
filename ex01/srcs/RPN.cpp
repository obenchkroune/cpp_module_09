#include "RPN.hpp"
#include <iostream>
#include <stack>
#include <stdexcept>
#include <sstream>

RPN::RPN()
{
}

RPN::RPN(const RPN &other)
{
	(void)other;
}

RPN &RPN::operator=(const RPN &other)
{
	(void)other;
	return *this;
}

RPN::~RPN()
{
}

int RPN::evaluateExpression(const std::string &expr)
{
	std::stack<int> nbrs;
	std::istringstream iss(expr);

	while (!(iss >> std::ws).eof())
	{
		int	nbr;
		char op;
		std::streampos pos = iss.tellg();

		if (iss >> nbr)
		{
			nbrs.push(nbr);
			continue ;
		}
		iss.clear();
		iss.seekg(pos);
		
		if (nbrs.size() < 2)
			throw std::runtime_error("invalid expression");
		int b = nbrs.top(); nbrs.pop();
		int a = nbrs.top(); nbrs.pop();

		iss >> std::ws >> op;
		switch (op)
		{
			case '+':
				nbrs.push(a + b);
				break;
			case '-':
				nbrs.push(a - b);
				break;
			case '*':
				nbrs.push(a * b);
				break;
			case '/':
				nbrs.push(a / b);
				break;
			default:
				throw std::runtime_error("invalid expression");
		}
	}
	
	if (nbrs.size() != 1)
		throw std::runtime_error("invalid expression");
	return nbrs.top();
}
