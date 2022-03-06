#include <iostream>

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define NO_COLOR "\033[0m"

#define ERROR(x) std::cout << RED << "[ERROR] " << NO_COLOR << x << std::endl
#define LOG(x)   std::cout << GREEN << "[LOG] " << NO_COLOR << x << std::endl