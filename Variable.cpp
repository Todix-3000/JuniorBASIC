//
// Created by tdix on 20.03.2021.
//

#include "Variable.h"


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

Variable::Variable() {
    intVars["TEST%"] = 42;
    floatVars["TEST"] = 42.5;
    stringVars["TEST$"] = "Mein Text";
}

Variable* Variable::instance = nullptr;

Variable* Variable::getContainer() {
    if (instance == nullptr) {
        instance = new Variable();
    }
    return instance;
}
