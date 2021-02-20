#include <iostream>
#include <stack>
#include "Token.h"
#include "ShuntingYard.h"



int main() {
    int result;
    std::cout << "Hello, World!" << std::endl;
    ShuntingYard* algorithm = new ShuntingYard();

    algorithm->run((unsigned char*) "3*25\0", result);

    algorithm->run((unsigned char*) "-(3*-25/5+6*-1)\0", result);

    return 0;
}
