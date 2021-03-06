//
// Created by tdix on 20.02.2021.
//

#include "Value.h"
#include <iostream>
#include "utils.h"

Value::Value() {
    type=VALUE_TYPE_INT;
    iValue = 0;
}
Value::Value(int intValue) {
    type=VALUE_TYPE_INT;
    iValue = intValue;
}

Value::Value(double floatValue) {
    type=VALUE_TYPE_FLOAT;
    fValue = floatValue;
}

Value::Value(std::string stringValue) {
    type = VALUE_TYPE_STRING;
    sValue = stringValue;
}

Value::~Value() {
}

int Value::getType() const{
    return type;
}

int Value::getInt() const {
    if (type==VALUE_TYPE_INT) {
        return iValue;
    }
    if (type==VALUE_TYPE_FLOAT) {
        return static_cast<int>(fValue);
    }
    throw Exception(EXCEPTION_TYPE_MISMATCH);
}

double Value::getFloat() const {
    if (type==VALUE_TYPE_FLOAT) {
        return fValue;
    }
    if (type==VALUE_TYPE_INT) {
        return static_cast<double>(iValue);
    }
    throw Exception(EXCEPTION_TYPE_MISMATCH);
}

std::string Value::getString() const {
    if (type==VALUE_TYPE_STRING) {
        return sValue;
    }
    throw Exception(EXCEPTION_TYPE_MISMATCH);
}

Value operator+(const Value &left, const Value &right) {
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

Value operator-(const Value &left, const Value &right) {
    if (left.getType()==VALUE_TYPE_STRING || right.getType()==VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    if (left.getType()==VALUE_TYPE_INT && right.getType()==VALUE_TYPE_INT) {
        return Value(left.getInt() - right.getInt());
    }
    return Value(left.getFloat() - right.getFloat());
}

Value operator*(const Value &left, const Value &right) {
    if (left.getType()==VALUE_TYPE_STRING || right.getType()==VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    if (left.getType()==VALUE_TYPE_INT && right.getType()==VALUE_TYPE_INT) {
        return Value(left.getInt() * right.getInt());
    }
    return Value(left.getFloat() * right.getFloat());
}

Value operator/(const Value &left, const Value &right) {
    if (left.getType()==VALUE_TYPE_STRING || right.getType()==VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    if (left.getType()==VALUE_TYPE_INT && right.getType()==VALUE_TYPE_INT) {
        return Value(left.getInt() / right.getInt());
    }
    return Value(left.getFloat() / right.getFloat());
}

Value operator-(const Value &right) {
    if (right.getType()==VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    if (right.getType()==VALUE_TYPE_INT) {
        return Value(-right.getInt());
    }
    return Value(-right.getFloat());
}

Value operator+(const Value &right) {
    if (right.getType() == VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    if (right.getType() == VALUE_TYPE_INT) {
        return Value(right.getInt());
    }
    return Value(right.getFloat());
}

Value operator%(const Value &left, const Value &right) {
    if (left.getType()==VALUE_TYPE_STRING || right.getType()==VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    return Value(left.getInt() % right.getInt());
}

std::ostream& operator<< (std::ostream &out, const Value &value) {
    if (value.getType()==VALUE_TYPE_STRING) {
        out << "s(" << value.getString() << ")";
    }
    if (value.getType()==VALUE_TYPE_INT) {
        out << "i(" << value.getInt() << ")";
    }
    if (value.getType()==VALUE_TYPE_FLOAT) {
        out << "f(" << value.getFloat() << ")";
    }
    return out;
}

Value operator<(const Value &left, const Value &right) {
    if (left.getType()==VALUE_TYPE_STRING && right.getType()==VALUE_TYPE_STRING) {
        return left.getString()<right.getString() ? Value(1) : Value(0);
    }
    if (left.getType()==VALUE_TYPE_STRING || right.getType()==VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    if (left.getType()==VALUE_TYPE_INT && right.getType()==VALUE_TYPE_INT) {
        return left.getInt()<right.getInt() ? Value(1) : Value(0);
    }
    return left.getFloat()<right.getFloat() ? Value(1) : Value(0);
}

Value operator<=(const Value &left, const Value &right) {
    if (left.getType()==VALUE_TYPE_STRING && right.getType()==VALUE_TYPE_STRING) {
        return left.getString()<=right.getString() ? Value(1) : Value(0);
    }
    if (left.getType()==VALUE_TYPE_STRING || right.getType()==VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    if (left.getType()==VALUE_TYPE_INT && right.getType()==VALUE_TYPE_INT) {
        return left.getInt()<=right.getInt() ? Value(1) : Value(0);
    }
    return left.getFloat()<=right.getFloat() ? Value(1) : Value(0);
}

Value operator>(const Value &left, const Value &right) {
    if (left.getType()==VALUE_TYPE_STRING && right.getType()==VALUE_TYPE_STRING) {
        return left.getString()>right.getString() ? Value(1) : Value(0);
    }
    if (left.getType()==VALUE_TYPE_STRING || right.getType()==VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    if (left.getType()==VALUE_TYPE_INT && right.getType()==VALUE_TYPE_INT) {
        return left.getInt()>right.getInt() ? Value(1) : Value(0);
    }
    return left.getFloat()>right.getFloat() ? Value(1) : Value(0);
}

Value operator>=(const Value &left, const Value &right) {
    if (left.getType()==VALUE_TYPE_STRING && right.getType()==VALUE_TYPE_STRING) {
        return left.getString()>=right.getString() ? Value(1) : Value(0);
    }
    if (left.getType()==VALUE_TYPE_STRING || right.getType()==VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    if (left.getType()==VALUE_TYPE_INT && right.getType()==VALUE_TYPE_INT) {
        return left.getInt()>=right.getInt() ? Value(1) : Value(0);
    }
    return left.getFloat()>=right.getFloat() ? Value(1) : Value(0);
}

Value operator==(const Value &left, const Value &right) {
    if (left.getType()==VALUE_TYPE_STRING && right.getType()==VALUE_TYPE_STRING) {
        return left.getString()==right.getString() ? Value(1) : Value(0);
    }
    if (left.getType()==VALUE_TYPE_STRING || right.getType()==VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    if (left.getType()==VALUE_TYPE_INT && right.getType()==VALUE_TYPE_INT) {
        return left.getInt()==right.getInt() ? Value(1) : Value(0);
    }
    return left.getFloat()==right.getFloat() ? Value(1) : Value(0);
}

Value operator!=(const Value &left, const Value &right) {
    if (left.getType()==VALUE_TYPE_STRING && right.getType()==VALUE_TYPE_STRING) {
        return left.getString()!=right.getString() ? Value(1) : Value(0);
    }
    if (left.getType()==VALUE_TYPE_STRING || right.getType()==VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    if (left.getType()==VALUE_TYPE_INT && right.getType()==VALUE_TYPE_INT) {
        return left.getInt()!=right.getInt() ? Value(1) : Value(0);
    }
    return left.getFloat()!=right.getFloat() ? Value(1) : Value(0);
}