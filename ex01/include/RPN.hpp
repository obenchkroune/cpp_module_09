#pragma once

#include <string>

class RPN
{
public:

	static int evaluateExpression(const std::string &expression);
private:
	RPN();
	RPN(const RPN &other);
	RPN &operator=(const RPN &other);
	~RPN();
};
