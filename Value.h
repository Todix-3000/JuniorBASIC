//
// Created by tdix on 20.02.2021.
//

#ifndef JUNIORBASIC_VALUE_H
#define JUNIORBASIC_VALUE_H

#include <string>

enum {VALUE_TYPE_INT, VALUE_TYPE_FLOAT, VALUE_TYPE_STRING};
class Value {
public:
    Value(int value);
    Value(double value);
    Value(std::string value);
    ~Value();
    int getType();
    int getInt();
    double getFloat();
    std::string getString();
private:
    int type;
    void* value;
    std::string sValue;
};

Value operator+(Value &left, Value &right);

#endif //JUNIORBASIC_VALUE_H
