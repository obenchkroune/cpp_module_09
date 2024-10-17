#include "RPN.hpp"
#include <stack>
#include <stdexcept>

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

	for (std::size_t i = 0; i < expr.size(); i++)
	{
		while (std::isspace(expr[i]) && i < expr.size())
			i++;
		if (i >= expr.size())
			break ;

		if (std::isdigit(expr[i]))
		{
			nbrs.push(expr[i] - '0');
			continue ;
		}

		if (nbrs.size() < 2)
			throw std::runtime_error("invalid expression");
		int b = nbrs.top(); nbrs.pop();
		int a = nbrs.top(); nbrs.pop();

		switch (expr[i])
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
