//
// Created by tdix on 07.03.2021.
//
#include "Function.h"
#include "utils.h"
#include "Variable.h"
#include <cmath>
#include <sstream>
#include <ctime>
#include <string.h>

void Function::sin(std::stack<Token>* stack) {
    auto params = getValuesFromStack(stack);
    if (params.size()!=1) {
        throw Exception(EXCEPTION_PARAMETER_COUNT);
    }
    if (params[0].getValue().getType() == VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    stack->push(Value(std::sin(params[0].getValue().getFloat())));
}

void Function::cos(std::stack<Token>* stack) {
    auto params = getValuesFromStack(stack);
    if (params.size()!=1) {
        throw Exception(EXCEPTION_PARAMETER_COUNT);
    }
    if (params[0].getValue().getType() == VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    stack->push(Value(std::cos(params[0].getValue().getFloat())));
}

void Function::tan(std::stack<Token>* stack) {
    auto params = getValuesFromStack(stack);
    if (params.size()!=1) {
        throw Exception(EXCEPTION_PARAMETER_COUNT);
    }
    if (params[0].getValue().getType() == VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    stack->push(Value(std::tan(params[0].getValue().getFloat())));
}

void Function::atn(std::stack<Token>* stack) {
    auto params = getValuesFromStack(stack);
    if (params.size()!=1) {
        throw Exception(EXCEPTION_PARAMETER_COUNT);
    }
    if (params[0].getValue().getType() == VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    stack->push(Value(std::atan(params[0].getValue().getFloat())));
}

void Function::sqr(std::stack<Token>* stack) {
    auto params = getValuesFromStack(stack);
    if (params.size()!=1) {
        throw Exception(EXCEPTION_PARAMETER_COUNT);
    }
    if (params[0].getValue().getType() == VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    stack->push(Value(std::sqrt(params[0].getValue().getFloat())));
}

void Function::exp(std::stack<Token>* stack) {
    auto params = getValuesFromStack(stack);
    if (params.size()!=1) {
        throw Exception(EXCEPTION_PARAMETER_COUNT);
    }
    if (params[0].getValue().getType() == VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    stack->push(Value(std::exp(params[0].getValue().getFloat())));
}

void Function::log(std::stack<Token>* stack) {
    auto params = getValuesFromStack(stack);
    if (params.size() != 1) {
        throw Exception(EXCEPTION_PARAMETER_COUNT);
    }
    if (params[0].getValue().getType() == VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    stack->push(Value(std::log(params[0].getValue().getFloat())));
}

void Function::xint(std::stack<Token>* stack) {
    auto params = getValuesFromStack(stack);
    if (params.size() != 1) {
        throw Exception(EXCEPTION_PARAMETER_COUNT);
    }
    if (params[0].getValue().getType() == VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    if (params[0].getValue().getType() == VALUE_TYPE_INT) {
        stack->push(Value(params[0].getValue().getInt()));
    } else {
        stack->push(Value(std::floor(params[0].getValue().getFloat())));
    }
}

void Function::sgn(std::stack<Token>* stack) {
    auto params = getValuesFromStack(stack);
    if (params.size()!=1) {
        throw Exception(EXCEPTION_PARAMETER_COUNT);
    }
    if (params[0].getValue().getType() == VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    if (params[0].getValue().getType() == VALUE_TYPE_INT) {
        stack->push(Value(params[0].getValue().getInt()>0 ? 1 : params[0].getValue().getInt()<0 ? -1 : 0));
    } else {
        stack->push(Value(params[0].getValue().getFloat() > 0 ? 1 : params[0].getValue().getFloat() < 0 ? -1 : 0));
    }
}

void Function::abs(std::stack<Token>* stack) {
    auto params = getValuesFromStack(stack);
    if (params.size()!=1) {
        throw Exception(EXCEPTION_PARAMETER_COUNT);
    }
    if (params[0].getValue().getType() == VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    if (params[0].getValue().getType() == VALUE_TYPE_INT) {
        stack->push(Value(std::abs(params[0].getValue().getInt())));
    } else {
        stack->push(Value(std::fabs(params[0].getValue().getFloat())));
    }
}

void Function::rnd(std::stack<Token>* stack) {
    static bool init = false;
    if (!init) {
        srand(time(NULL));
        rand();
        srand(rand());
        init = true;
    }
    auto params = getValuesFromStack(stack);
    if (params.size() != 0) {
        throw Exception(EXCEPTION_PARAMETER_COUNT);
    }
    stack->push(Value((double)rand()/(double)RAND_MAX));
}

void Function::chr(std::stack<Token>* stack) {
    auto params = getValuesFromStack(stack);
    if (params.size()!=1) {
        throw Exception(EXCEPTION_PARAMETER_COUNT);
    }
    if (params[0].getValue().getType() == VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    auto c = params[0].getValue().getInt();
    if (c<0 || c>255) {
        throw Exception(EXCEPTION_RANGE_ERROR);
    }

    stack->push(Value(std::string(1, c)));
}

void Function::asc(std::stack<Token>* stack) {
    auto params = getValuesFromStack(stack);
    if (params.size()!=1) {
        throw Exception(EXCEPTION_PARAMETER_COUNT);
    }
    if (params[0].getValue().getType() != VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    auto value = params[0].getValue().getString();
    if (value.length()==0) {
        stack->push(Value(0));
    } else {
        stack->push(Value((int) value[0]));
    }
}

void Function::len(std::stack<Token>* stack) {
    auto params = getValuesFromStack(stack);
    if (params.size()!=1) {
        throw Exception(EXCEPTION_PARAMETER_COUNT);
    }
    if (params[0].getValue().getType() != VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    stack->push(Value((int) params[0].getValue().getString().length()));
}

void Function::val(std::stack<Token>* stack) {
    auto params = getValuesFromStack(stack);
    int size = params.size();
    if (params.size()!=1) {
        throw Exception(EXCEPTION_PARAMETER_COUNT);
    }
    if (params[0].getValue().getType() != VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    auto value = params[0].getValue().getString();
    try {
        size_t intLength;
        size_t doubleLength;
        int intValue = std::stoi(value, &intLength);
        double doubleValue = std::stod(value , &doubleLength);
        if (doubleLength>intLength) {
            stack->push(Value(doubleValue));
            return;
        }
        stack->push(Value(intValue));
    } catch (std::invalid_argument e) {
        stack->push(Value(0));
    }
}

void Function::left(std::stack<Token>* stack) {
    auto params = getValuesFromStack(stack);
    if (params.size() != 2) {
        throw Exception(EXCEPTION_PARAMETER_COUNT);
    }
    if (params[0].getValue().getType() != VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    if (params[1].getValue().getType() == VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    stack->push(Value( params[0].getValue().getString().substr(0, params[1].getValue().getInt())));
}

void Function::right(std::stack<Token>* stack) {
    auto params = getValuesFromStack(stack);
    if (params.size() != 2) {
        throw Exception(EXCEPTION_PARAMETER_COUNT);
    }
    if (params[0].getValue().getType() != VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    if (params[1].getValue().getType() == VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    std::string value = params[0].getValue().getString();
    int len = value.length();
    int start = len-params[1].getValue().getInt();
    if (start<0) {
        start = 0;
    }
    len = len - start;
    stack->push(Value( value.substr(start, len)));
}

void Function::mid(std::stack<Token>* stack) {
    auto params = getValuesFromStack(stack);
    if (params.size()<2 || params.size()>3) {
        throw Exception(EXCEPTION_PARAMETER_COUNT);
    }
    if (params[0].getValue().getType() != VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    if (params[1].getValue().getType() == VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    if (params.size()==3 && params[2].getValue().getType() == VALUE_TYPE_STRING) {
        throw Exception(EXCEPTION_TYPE_MISMATCH);
    }
    std::string value = params[0].getValue().getString();
    int start = params[1].getValue().getInt()-1;
    if (start<0) {
        throw Exception(EXCEPTION_RANGE_ERROR);
    }
    if (start>value.length()) {
        start = value.length()-1;
    }
    int len = value.length()-start;
    if (params.size()==3) {
        len = params[2].getValue().getInt();
        if (len<0) {
            throw Exception(EXCEPTION_RANGE_ERROR);
        }
    }
    stack->push(Value( value.substr(start, len)));
}

void Function::str(std::stack<Token>* stack) {
    auto params = getValuesFromStack(stack);
    if (params.size()!=1) {
        throw Exception(EXCEPTION_PARAMETER_COUNT);
    }
    if (params[0].getValue().getType() == VALUE_TYPE_INT) {
        stack->push(Value(std::to_string(params[0].getValue().getInt())));
        return;
    }
    if (params[0].getValue().getType() == VALUE_TYPE_FLOAT) {
        std::ostringstream ss;
        ss << params[0].getValue().getFloat();
        stack->push(Value(ss.str()));
        return;
    }
    throw Exception(EXCEPTION_TYPE_MISMATCH);
}

void Function::getArrayVariableValue(std::stack<Token>* stack, std::string varName, short varType) {
    auto params = getValuesFromStack(stack);
    std::vector<int> indexes;
    for (auto &param :params) {
        indexes.push_back(param.getValue().getInt());
    }
    auto x = Variable::getContainer()->getValue(varName, varType, indexes);
    stack->push(x);
}

std::vector<Token> Function::getValuesFromStack(std::stack<Token>* stack) {
    std::vector<Token> result;
    while(!stack->empty()) {
        auto value = stack->top();
        if (value.getType() == TOKEN_TYPE_VALUE) {
            stack->pop();
            result.insert(result.begin(), value);
        } else {
            if (value.getType() == TOKEN_TYPE_BRACKETOPEN) {
                stack->pop();
            }
            return result;
        }
    }
    return result;
}

void Function::dummy(std::stack<Token> *stack) {
    auto params = getValuesFromStack(stack);
    if (params.size()!=1) {
        throw Exception(EXCEPTION_PARAMETER_COUNT);
    }

    stack->push(params[0]);
}
