#include <iostream>
#include <stack>
#include "Token.h"
#include "ShuntingYard.h"



int main() {
    int result;
    std::cout << "Hello, World!" << std::endl;
    ShuntingYard* algorithm = new ShuntingYard();

    algorithm->run((unsigned char*) "9-5*2+8/2\0", result);

    return 0;
}
