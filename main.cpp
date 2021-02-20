#include <iostream>
#include <stack>
#include "Token.h"
#include "Value.h"
#include "ShuntingYard.h"



int main() {
    Value v1 = Value{"AA"};
    Value v2 = Value{"SS"};

    Value v3 = v1+v2;

    std::cout << v3.getString();

    return 0;
    int result;
    std::cout << "Hello, World!" << std::endl;
    ShuntingYard* algorithm = new ShuntingYard();

    algorithm->run((unsigned char*) "3*25\0", result);

    algorithm->run((unsigned char*) "-(3*-25/5+6*-1)\0", result);

    return 0;
}
