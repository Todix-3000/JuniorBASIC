#include <iostream>
#include <stack>
#include "Token.h"
#include "ShuntingYard.h"



int main() {
    int result;
    std::cout << "Hello, World!" << std::endl;
    ShuntingYard* algorithm = new ShuntingYard();

    algorithm->run((unsigned char*) "5 + 6 * 8\0", result);

    return 0;
}
