//
// Created by tdix on 07.04.2021.
//

#include "ForNextDefinition.h"
#include "utils.h"

ForNextDefinition::ForNextDefinition(VarDefinition varDef, std::vector<int> varIndex, char direction, Value counter, Value target, Value step) {
    this->varDef      = varDef;
    this->varIndex    = varIndex;
    this->direction   = direction;
    this->counter     = counter;
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
    this->counter = this->counter + this->step;
    if (direction > 0) {
        if ((this->counter > this->target).getInt() == 1) {
            return false;
        }
    } else {
        if ((this->counter < this->target).getInt() == 1) {
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

const Value &ForNextDefinition::getCounter() const {
    return counter;
}

