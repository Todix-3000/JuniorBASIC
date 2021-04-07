//
// Created by tdix on 07.04.2021.
//

class  {
private:
    VarDefinition varDef;
    std::vector<int> varIndex;
    bool isAscending;
    Value counter;
    Value target;
    Value step;
public:
ForNextDefinition::ForNextDefinition(VarDefinition varDef, std::vector<int> varIndex, bool isAscending, Value counter, Value target, Value step) {
    this->varDef      = varDef;
    this->varIndex    = varIndex;
    this->isAscending = isAscending;
    this->counter     = counter;
    this->target      = target;
    this->step        = step;
}

bool ForNextDefinition::checkNextStep(VarDefinition varDef, std::vector<int> varIndex) {
    if (varDef != this->varDef || varIndex.count() != this->varIndex.count()) {
        throw ForNextDefinition(EXCEPTION_NEXT_WITHOUT_FOR);
    }
    return ForNextDefinition::checkNextStep();
}

bool ForNextDefinition::checkNextStep() {
    // Check Stack
    // count counter
    // compare
    // if false => remove from stack
    // if true => jump to stack-adress
}

