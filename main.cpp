#include <iostream>
#include <stack>
#include "Token.h"
#include "Value.h"
#include "ShuntingYard.h"
#include "utils.h"


int main() {

    ShuntingYard *algorithm = new ShuntingYard();
    try {
        //algorithm->run((unsigned char*) "3*25\0", result);
        std::cout << algorithm->run((unsigned char *) "SIN(0.5)\0") << std::endl;
        //std::cout << algorithm->run((unsigned char *) "\"Hallo\" = \" \" + \"Hallo\"\0") << std::endl;
       // std::cout << algorithm->run((unsigned char*) "-(3*-26./5+6*-1)\0") << std::endl;
       // std::cout << algorithm->run((unsigned char*) "25*6-100\0") << std::endl;
       // std::cout << algorithm->run((unsigned char *) "\"Hallo\" + \" \" + \"Welt\"\0") << std::endl;
       // std::cout << algorithm->run((unsigned char*) "\"\" && \"45\"\0") << std::endl;
    } catch (Exception exception) {
        std::cout << "EXCEPTION: " << exception.getCode();
    }
    return 0;
}
