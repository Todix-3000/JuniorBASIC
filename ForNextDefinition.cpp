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
    this->varDef = varDef;
    this->varIndex = varIndex;
    this->isAscending = isAscending;
    this->counter = counter;
    this->target  = target;
    this->step    = step;
}

bool ForNextDefinition::checkNextStep(VarDefinition varDef, std::vector<int> varIndex) {

}

bool ForNextDefinition::checkNextStep() {

}

