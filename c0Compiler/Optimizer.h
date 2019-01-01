//
// Created by andyson on 18-12-28.
//

#ifndef C0COMPILER_OPTIMIZER_H
#define C0COMPILER_OPTIMIZER_H

#include "Quadruple.h"
#include <vector>

class Optimizer {
public:
    static Optimizer &getOptimizer(Quadruple &theOriginQuadruple);  // 单例设计模式
    bool constantCombinationOption;
    bool blockPublicExpressionOption;
    bool peepholeOption;

    void optimize();

private:
    Quadruple &originQuadruple;
    vector<int> functionEntry;

    void constantCombinationOptimize();
    void peepholeOptimize();
    void blockPublicExpressionOptimize();


    bool isConst(string a);
    Optimizer(Quadruple &theQuadruple);
};


#endif //C0COMPILER_OPTIMIZER_H
