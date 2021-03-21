#include <iostream>
#include <stack>
#include "Token.h"
#include "Value.h"
#include "ShuntingYard.h"
#include "utils.h"
#include "Variable.h"


int main() {

    ShuntingYard *algorithm = new ShuntingYard();
    try {
        auto v = Variable::getContainer();
        v->setValue("TEST%", Value(42));
        v->setValue("TEST%", Value(42.5));
        v->setValue("TEST$", Value("HALLO I BIMS"));

        v->dim("TEST%", VALUE_TYPE_INT, {5,5});
        v->setValue("TEST%", {2,3}, Value(23));

        std::cout << algorithm->run((unsigned char*) "1+(2*3)-8+TEST%(2,5-2)\0") << std::endl;
        std::cout << algorithm->run((unsigned char *) "3+VAL(\"45K\")\0") << std::endl;
        std::cout << algorithm->run((unsigned char *) "MID$(TEST$, 3,7)\0") << std::endl;

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
