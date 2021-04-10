//
// Created by tdix on 20.02.2021.
//

#ifndef JUNIORBASIC_UTILS_H
#define JUNIORBASIC_UTILS_H

#include <cstdlib>
#include <type_traits>
#include <exception>
#include <string>

template<typename Base, typename T>
inline bool instanceof(const T*) {
    return std::is_base_of<Base, T>::value;
}

enum {
    EXCEPTION_NOT_IMPLEMENTED, EXCEPTION_ILLEGAL_EXPRESSION, EXCEPTION_TYPE_MISMATCH,
    EXCEPTION_PARAMETER_COUNT, EXCEPTION_RANGE_ERROR, EXCEPTION_ARRAY_DIMED,
    EXCEPTION_DIVISION_BY_ZERO, EXCEPTION_UNKNOWN_LINE, EXCEPTION_FILE_READ,
    EXCEPTION_FILE_WRITE, EXCEPTION_RETURN_WITHOUT_GOSUB, EXCEPTION_NEXT_WITHOUT_FOR,
    EXCEPTION_OUT_OF_DATA, EXCEPTION_FILE_OPEN, EXCEPTION_FILE_NOT_OPEN,
    EXCEPTION_FILESYSTEM
};

class NotFount: public std::exception {};
class Break: public std::exception {};
class Exception: public std::exception {
public:
    Exception(int code);
    int getCode();
    std::string getMessage();
private:
    int code;
};
#endif //JUNIORBASIC_UTILS_H
