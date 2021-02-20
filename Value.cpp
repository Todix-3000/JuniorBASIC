//
// Created by tdix on 20.02.2021.
//

#include "Value.h"

Value::Value(int intValue) {
    type=VALUE_TYPE_INT;
    value = malloc(sizeof(int));
    *((int*) value) = intValue;
}

Value::Value(double floatValue) {
    type=VALUE_TYPE_FLOAT;
    value = malloc(sizeof(int));
    *((double*) value) = floatValue;
}

Value::Value(std::string stringValue) {
    type=VALUE_TYPE_STRING;
}

Value::~Value() {
    free(value);
}