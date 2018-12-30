//
// Created by andyson on 18-12-28.
//

#include "Optimizer.h"

Optimizer &Optimizer::getOptimizer(Quadruple theOriginQuadruple, Quadruple theOptimizedQuadruple) {
    static Optimizer instance(theOriginQuadruple, theOptimizedQuadruple);
    return instance;
}

void Optimizer::optimize() {
    if(constantCombinationOption){
        constantCombinationOptimize();
    }
    if(globalRegisterOption){
        //globalRegisterOptimize();
    }
    if(blockPublicExpressionOption){
        //blockPublicExpressionOptimize();
    }

}

void Optimizer::constantCombinationOptimize() {

}

void Optimizer::globalRegisterOptimize() {

}

void Optimizer::blockPublicExpressionOptimize() {

}


void Optimizer::splitIntoBasicBlock() {
    for(int i=0;i<originQuadruple.length();i++){

    }
}

Optimizer::Optimizer(Quadruple theOriginQuadruple, Quadruple theOptimizedQuadruple)
        : originQuadruple(theOriginQuadruple), optimizedQuadruple(theOptimizedQuadruple) {
    currentEntry = 1;
    currentExit = 1;
    constantCombinationOption = true;
    globalRegisterOption = true;
    blockPublicExpressionOption = true;
}