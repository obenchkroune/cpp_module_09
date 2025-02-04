#pragma once

#include <string>

class BadExpressionException : public std::exception {
public:
    const char* what() const throw();
};

int         evaluate_expression(const std::string& expr);
std::string rpn_usage(const char* progname);