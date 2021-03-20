//
// Created by tdix on 20.03.2021.
//

#ifndef JUNIORBASIC_VARIABLE_H
#define JUNIORBASIC_VARIABLE_H
#include <map>
#include "Value.h"

class Variable {
private:
    Variable();
    static Variable *instance;
    std::map<std::string,int>         intVars;
    std::map<std::string,double>      floatVars;
    std::map<std::string,std::string> stringVars;
public:
    static Variable* getContainer();
    Value getValue(std::string varName, int varType);
};
#endif //JUNIORBASIC_VARIABLE_H
