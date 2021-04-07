//
// Created by tdix on 07.04.2021.
//

#ifndef JUNIORBASIC_FORNEXTDEFINITION_H
#define JUNIORBASIC_FORNEXTDEFINITION_H

class ForNextDefinition {
private:
    VarDefinition varDef;
    std::vector<int> varIndex;
    bool isAscending;
    Value counter;
    Value target;
    Value step;
public:
   ForNextDefinition(VarDefinition varDef, std::vector<int> varIndex, bool isAscending, Value counter, Value target, Value step);
   bool checkNextStep(VarDefinition varDef, std::vector<int> varIndex);
   bool checkNextStep();
}

#endif //JUNIORBASIC_FORNEXTDEFINITION_H