//
// Created by tdix on 07.04.2021.
//

#ifndef JUNIORBASIC_FORNEXTDEFINITION_H
#define JUNIORBASIC_FORNEXTDEFINITION_H

#include "Token.h"

class ForNextDefinition {
private:
    VarDefinition varDef;
    std::vector<int> varIndex;
    char direction;
    Value target;
    Value step;

public:
    const VarDefinition &getVarDef() const;
    const std::vector<int> &getVarIndex() const;
    ForNextDefinition(VarDefinition varDef, std::vector<int> varIndex, char direction, Value target, Value step);
    bool checkNextStep(VarDefinition varDef, std::vector<int> varIndex);
    bool checkNextStep();
};

#endif //JUNIORBASIC_FORNEXTDEFINITION_H