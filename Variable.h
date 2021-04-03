//
// Created by tdix on 20.03.2021.
//

#ifndef JUNIORBASIC_VARIABLE_H
#define JUNIORBASIC_VARIABLE_H
#include <map>
#include <vector>
#include "Value.h"

class Variable {
private:
    Variable();
    static Variable *instance;
    std::map<std::string,int>                      intVars;
    std::map<std::string,double>                   floatVars;
    std::map<std::string,std::string>              stringVars;

    std::map<std::string,std::map<int,int>>         intArrays;
    std::map<std::string,std::map<int,double>>      floatArrays;
    std::map<std::string,std::map<int,std::string>> stringArrays;

    std::map<std::string, std::vector<int>>        dimensions;
    int getLinearIndex(std::string varName, short varType, std::vector<int> indexes);


public:
    static Variable* getContainer();
    Value getValue(std::string varName, int varType);
    Value getValue(std::string varName, short varType, std::vector<int> indexes);
    void setValue(std::string varName, Value value);
    void setValue(std::string varName, std::vector<int> indexes, Value value);
    void dim(std::string varName, short varType, std::vector<int> indexes);
    void clearAll();
};
#endif //JUNIORBASIC_VARIABLE_H
