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