#include "RPN.hpp"

#include <iostream>
#include <sstream>
#include <stack>

static int calculate_operation(std::stack<int>& nbrs, char op) {
    if (nbrs.size() < 2)
        throw std::runtime_error("Error");
    int b = nbrs.top();
    nbrs.pop();
    int a = nbrs.top();
    nbrs.pop();

    switch (op) {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        if (b == 0)
            throw std::runtime_error("division by zero");
        return a / b;
    default:
        throw std::runtime_error("Error");
    }
}

int RPN(const std::string& expression) {
    std::istringstream iss(expression);
    std::stack<int>    nbrs;
    int                nbr;

    while (iss >> std::ws && !iss.eof()) {
        if (std::isdigit(iss.peek())) {
            iss >> nbr;
            if (iss.fail())
                throw std::runtime_error("Error");
            nbrs.push(nbr);
            continue;
        }
        nbrs.push(calculate_operation(nbrs, iss.get()));
    }
    if (nbrs.size() != 1)
        throw std::runtime_error("Error");
    return nbrs.top();
}

std::string collect_input(int ac, char** av) {
    std::string input;

    for (int i = 1; i < ac; i++) {
        input += av[i];
        input += " ";
    }
    return input;
}
