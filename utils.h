//
// Created by tdix on 20.02.2021.
//

#ifndef JUNIORBASIC_UTILS_H
#define JUNIORBASIC_UTILS_H

#include <cstdlib>
#include <type_traits>
#include <exception>

template<typename Base, typename T>
inline bool instanceof(const T*) {
    return std::is_base_of<Base, T>::value;
}

enum {
    EXCEPTION_NOT_IMPLEMENTED, EXCEPTION_ILLEGAL_EXPRESSION
};

class Exception: public std::exception {
public:
    Exception(int code);
    int getCode();
private:
    int code;
};
#endif //JUNIORBASIC_UTILS_H