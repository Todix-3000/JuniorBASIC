//
// Created by tdix on 20.02.2021.
//

#include "utils.h"

Exception::Exception(int code) {
    this->code = code;
}

int Exception::getCode() {
    return code;
}

std::string Exception::getMessage() {
    switch (code) {
        case EXCEPTION_NOT_IMPLEMENTED:
            return "NOT IMPLEMENTED";
        case EXCEPTION_ILLEGAL_EXPRESSION:
            return "SYNTAX";
        case EXCEPTION_TYPE_MISMATCH:
            return "TYPE MISMATCH";
        case EXCEPTION_PARAMETER_COUNT:
            return "PARAMETER COUNT";
        case EXCEPTION_RANGE_ERROR:
            return "RANGE";
        case EXCEPTION_ARRAY_DIMED:
            return "ARRAY DIMED";
        case EXCEPTION_DIVISION_BY_ZERO:
            return "DIVISION BY ZERO";
        case EXCEPTION_UNKNOWN_LINE:
            return "UNKNOWN LINE";
        case EXCEPTION_FILE_READ:
            return "FILE READ";
        case EXCEPTION_FILE_WRITE:
            return "FILE WRITE";
        case EXCEPTION_RETURN_WITHOUT_GOSUB:
            return "RETURN WITHOUT GOSUB";
        case EXCEPTION_NEXT_WITHOUT_FOR:
            return "NEXT WITHOUT FOR";
        case EXCEPTION_OUT_OF_DATA:
            return "OUT OF DATA";
        case EXCEPTION_FILE_OPEN:
            return "FILE IS OPEN";
        case EXCEPTION_FILE_NOT_OPEN:
            return "FILE IS NOT OPEN";
        case EXCEPTION_FILESYSTEM:
            return "FILESYSTEM";
        case EXCEPTION_RUN_MODE:
            return "RUN MODE";
        default:
            return "UNKNOWN";

    }
    return std::__cxx11::string();
}
