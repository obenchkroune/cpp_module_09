#pragma once

#include <string>

std::string collect_input(int ac, char** av);
int         RPN(const std::string& expression);
