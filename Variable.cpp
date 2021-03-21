//
// Created by tdix on 20.03.2021.
//

#include "Variable.h"
#include "utils.h"


Value Variable::getValue(std::string varName, int varType) {
    switch (varType) {
        case VALUE_TYPE_INT:
            return Value(intVars[varName]);
        case VALUE_TYPE_STRING:
            return Value(stringVars[varName]);
        case VALUE_TYPE_FLOAT:
            return Value(floatVars[varName]);
    }
}

Value Variable::getValue(std::string varName, short varType, std::vector<int> indexes) {
    int dims = indexes.size();
    if (dimensions.find(varName) == dimensions.end()) {
        std::vector<int> createDimensions;
        for (int i=0; i<dims; i++) {
            createDimensions.push_back(10);
        }
        dim(varName, varType, createDimensions);
    }
    std::vector<int> givenIndexes = dimensions[varName];
    if (dims != givenIndexes.size()) {
        throw Exception(EXCEPTION_RANGE_ERROR);
    }
    int multiplyer = 1;
    int linearIndex;
    for (int i=0; i<dims; i++) {
        if (indexes[i] > givenIndexes[i]) {
            throw Exception(EXCEPTION_RANGE_ERROR);
        }
        linearIndex += multiplyer * indexes[i];
        multiplyer *= givenIndexes[i]+1;
    }
    switch (varType) {
        case VALUE_TYPE_INT:
            return Value(intVars[varName]);
        case VALUE_TYPE_STRING:
            return Value(stringVars[varName]);
        case VALUE_TYPE_FLOAT:
            return Value(floatVars[varName]);
    }
}

void Variable::dim(std::string varName, short varType, std::vector<int> indexes) {
    if (dimensions.find(varName) != dimensions.end()) {
        throw Exception(EXCEPTION_ARRAY_DIMED);
    }
    dimensions[varName] = indexes;
}

Variable::Variable() {
    intVars["TEST%"] = 42;
    floatVars["TEST"] = 42.5;
    stringVars["TEST$"] = "Mein Text";
    std::vector<int> v = {5,5};
    dim("TEST%", VALUE_TYPE_INT, v);
}

Variable* Variable::instance = nullptr;

Variable* Variable::getContainer() {
    if (instance == nullptr) {
        instance = new Variable();
    }
    return instance;
}
