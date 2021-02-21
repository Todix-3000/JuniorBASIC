#include <iostream>
#include <stack>
#include "Token.h"
#include "Value.h"
#include "ShuntingYard.h"



int main() {

    std::cout << "Hello, World!" << std::endl;
    ShuntingYard* algorithm = new ShuntingYard();

    //algorithm->run((unsigned char*) "3*25\0", result);

    //std::cout << algorithm->run((unsigned char*) "-(3*-26./5+6*-1)\0") << std::endl;

    // std::cout << algorithm->run((unsigned char*) "25*6-100\0") << std::endl;
    std::cout << algorithm->run((unsigned char*) "\"Hallo\"+\"d\"\0") << std::endl;
    // std::cout << algorithm->run((unsigned char*) "25\0") << std::endl;

    return 0;
}
