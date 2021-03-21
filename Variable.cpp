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
    throw Exception(EXCEPTION_TYPE_MISMATCH);
}

Value Variable::getValue(std::string varName, short varType, std::vector<int> indexes) {
    int linearIndex = getLinearIndex(varName, varType, indexes);

    switch (varType) {
        case VALUE_TYPE_INT:
            return Value(intArrays[varName][linearIndex]);
        case VALUE_TYPE_STRING:
            return Value(stringArrays[varName][linearIndex]);
        case VALUE_TYPE_FLOAT:
            return Value(floatArrays[varName][linearIndex]);
    }
    throw Exception(EXCEPTION_TYPE_MISMATCH);
}

void Variable::setValue(std::string varName, Value value) {
    switch (value.getType()) {
        case VALUE_TYPE_INT:
            intVars[varName] = value.getInt();
            break;
        case VALUE_TYPE_STRING:
            stringVars[varName] = value.getString();
            break;
        case VALUE_TYPE_FLOAT:
            floatVars[varName] = value.getFloat();
            break;
    }
}

void Variable::setValue(std::string varName, std::vector<int> indexes, Value value) {
    int linearIndex = getLinearIndex(varName, value.getType(), indexes);

    switch (value.getType()) {
        case VALUE_TYPE_INT:
            intArrays[varName][linearIndex] = value.getInt();
            break;
        case VALUE_TYPE_STRING:
            stringArrays[varName][linearIndex] = value.getString();
            break;
        case VALUE_TYPE_FLOAT:
            floatArrays[varName][linearIndex] = value.getFloat();
            break;
    }
}

int Variable::getLinearIndex(std::string varName, short varType, std::vector<int> indexes) {
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
    return linearIndex;
}

void Variable::dim(std::string varName, short varType, std::vector<int> indexes) {
    if (dimensions.find(varName) != dimensions.end()) {
        throw Exception(EXCEPTION_ARRAY_DIMED);
    }
    dimensions[varName] = indexes;
}

Variable::Variable() {
}

Variable* Variable::instance = nullptr;

Variable* Variable::getContainer() {
    if (instance == nullptr) {
        instance = new Variable();
    }
    return instance;
}
