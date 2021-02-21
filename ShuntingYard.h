//
// Created by tdix on 07.02.2021.
//

#include "Value.h"

enum {
    ERROR_OK, ERROR_EXPRESSION_TOO_COMPLEX, ERROR_MISSING_BRACKET_OPEN, ERROR_TOO_MANY_BRACKETS_OPEN
};

class ShuntingYard {
public:
    ShuntingYard();
    static Value run(unsigned char* input);
};
