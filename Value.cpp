//
// Created by tdix on 20.02.2021.
//

#include "Value.h"
#include "utils.h"

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
    type = VALUE_TYPE_STRING;
    sValue = stringValue;
}

Value::~Value() {
    if (type!=VALUE_TYPE_STRING) {
        free(value);
    }
}

int Value::getType() {
    return type;
}

int Value::getInt() {
    if (type==VALUE_TYPE_INT) {
        return *((int *) value);
    }
    if (type==VALUE_TYPE_FLOAT) {
        return static_cast<int>(*((double *) value));
    }
    throw Exception(EXCEPTION_TYPE_MISMATCH);
}

double Value::getFloat() {
    if (type==VALUE_TYPE_FLOAT) {
        return *((double *) value);
    }
    if (type==VALUE_TYPE_INT) {
        return static_cast<double>(*((int *) value));
    }
    throw Exception(EXCEPTION_TYPE_MISMATCH);
}

std::string Value::getString() {
    if (type==VALUE_TYPE_STRING) {
        return sValue;
    }
    throw Exception(EXCEPTION_TYPE_MISMATCH);
}


Value operator+(Value &left, Value &right) {
    if (left.getType()==VALUE_TYPE_STRING && right.getType()==VALUE_TYPE_STRING) {
        return Value(left.getString()+right.getString());
    }
    if (left.getType()==VALUE_TYPE_STRING || right.getType()==VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    if (left.getType()==VALUE_TYPE_INT && right.getType()==VALUE_TYPE_INT) {
        return Value(left.getInt() + right.getInt());
    }
    return Value(left.getFloat() + right.getFloat());
}