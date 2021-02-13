//
// Created by tdix on 07.02.2021.
//

enum {
    ERROR_OK, ERROR_EXPRESSION_TOO_COMPLEX, ERROR_MISSING_BRACKET_OPEN, ERROR_TOO_MANY_BRACKETS_OPEN
};

class ShuntingYard {
public:
    ShuntingYard();
    void run(unsigned char* input, int &error);
};
