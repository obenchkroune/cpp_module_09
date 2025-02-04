#include "RPN.hpp"

#include <cstring>
#include <deque>
#include <stdexcept>
#include <string>

const char* BadExpressionException::what() const throw() {
    return "invalid expression";
}

int evaluate_expression(const std::string& expr) {
    static const char* operators = "+-/*";
    std::deque<int>    result;

    for (std::string::size_type i = 0; i < expr.size(); ++i) {
        while (::isspace(expr[i])) ++i;

        if (expr[i] == '\0') break;

        if (std::strchr(operators, expr[i])) {
            if (result.size() < 2) throw BadExpressionException();

            int b = result.back();
            result.pop_back();
            int a = result.back();
            result.pop_back();

            switch (expr[i]) {
                case '+':
                    result.push_back(a + b);
                    break;
                case '-':
                    result.push_back(a - b);
                    break;
                case '*':
                    result.push_back(a * b);
                    break;
                case '/':
                    if (b == 0) throw std::runtime_error("division by 0");
                    result.push_back(a / b);
                    break;
            }

            continue;
        }

        if (!std::isdigit(expr[i])) throw BadExpressionException();

        result.push_back(expr[i] - '0');
    }

    if (result.size() != 1) throw BadExpressionException();
    return result.back();
}

std::string rpn_usage(const char* progname) {
    return "usage: " + std::string(progname) + " <expression> [...expression]";
}
