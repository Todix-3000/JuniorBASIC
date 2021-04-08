//
// Created by tdix on 07.04.2021.
//

#include "ForNextDefinition.h"
#include "utils.h"
#include "Variable.h"

ForNextDefinition::ForNextDefinition(VarDefinition varDef, std::vector<int> varIndex, char direction, Value target, Value step) {
    this->varDef      = varDef;
    this->varIndex    = varIndex;
    this->direction   = direction;
    this->target      = target;
    this->step        = step;
}

bool ForNextDefinition::checkNextStep(VarDefinition varDef, std::vector<int> varIndex) {
    if (varDef.varName != this->varDef.varName || varIndex != this->varIndex) {
        throw Exception(EXCEPTION_NEXT_WITHOUT_FOR);
    }
    return ForNextDefinition::checkNextStep();
}

bool ForNextDefinition::checkNextStep() {
    Value counter = Variable::getContainer()->getValue(varDef.varName, varDef.varType, varIndex);
    counter = counter + this->step;
    Variable::getContainer()->setValue(varDef.varName, varIndex, counter);

    if (direction > 0) {
        if ((counter > this->target).getInt() == 1) {
            return false;
        }
    } else {
        if ((counter < this->target).getInt() == 1) {
            return false;
        }
    }

    return true;
}

const VarDefinition &ForNextDefinition::getVarDef() const {
    return varDef;
}

const std::vector<int> &ForNextDefinition::getVarIndex() const {
    return varIndex;
}


