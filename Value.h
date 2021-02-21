//
// Created by tdix on 20.02.2021.
//

#ifndef JUNIORBASIC_VALUE_H
#define JUNIORBASIC_VALUE_H

#include <string>

enum {VALUE_TYPE_INT, VALUE_TYPE_FLOAT, VALUE_TYPE_STRING};
class Value {
public:
    Value();
    Value(int value);
    Value(double value);
    Value(std::string value);
    ~Value();
    int getType() const;
    int getInt() const;
    double getFloat() const;
    std::string getString() const;
private:
    int type;
    int iValue;
    double fValue;
    std::string sValue;
};

Value operator+(const Value &left, const Value &right);
Value operator-(const Value &left, const Value &right);
Value operator*(const Value &left, const Value &right);
Value operator/(const Value &left, const Value &right);
Value operator-(const Value &right);
Value operator+(const Value &right);

std::ostream& operator<<(std::ostream &out, const Value &value);

#endif //JUNIORBASIC_VALUE_H
