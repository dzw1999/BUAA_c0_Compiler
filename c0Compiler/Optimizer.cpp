//
// Created by andyson on 18-12-28.
//

#include "Optimizer.h"

Optimizer &Optimizer::getOptimizer(Quadruple theOriginQuadruple, Quadruple theOptimizedQuadruple) {
    static Optimizer instance(theOriginQuadruple, theOptimizedQuadruple);
    return instance;
}

Optimizer::Optimizer(Quadruple theOriginQuadruple, Quadruple theOptimizedQuadruple)
        : originQuadruple(theOriginQuadruple), optimizedQuadruple(theOptimizedQuadruple) {

}