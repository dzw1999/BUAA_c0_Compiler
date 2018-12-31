//
// Created by andyson on 18-12-28.
//

#include "Optimizer.h"

Optimizer &Optimizer::getOptimizer(Quadruple theOriginQuadruple, Quadruple theOptimizedQuadruple) {
    static Optimizer instance(theOriginQuadruple, theOptimizedQuadruple);
    return instance;
}

//void Optimizer::optimize() {
//    if (constantCombinationOption) {
//        //constantCombinationOptimize();
//    }
//    if (globalRegisterOption) {
//        //globalRegisterOptimize();
//    }
//    if (blockPublicExpressionOption) {
//        //blockPublicExpressionOptimize();
//    }
//
//}
//
//void Optimizer::constantCombinationOptimize() {
//    for (int i = 0; i < functionEntry.size(); i++) {
//        for (int j = functionEntry[i];
//             j < (i == functionEntry.size() - 1 ? originQuadruple.length() : functionEntry[i + 1] - 1); ++j) {
//            if ((originQuadruple.quadrupleList[j]->op == ADD ||
//                 originQuadruple.quadrupleList[j]->op == SUB ||
//                 originQuadruple.quadrupleList[j]->op == MUL ||
//                 originQuadruple.quadrupleList[j]->op == DIV) &&
//                isdigit(originQuadruple.quadrupleList[j]->src1[0]) &&
//                isdigit(originQuadruple.quadrupleList[j]->src2[0])) {
//                string replace = originQuadruple.quadrupleList[j]->dst;
//                int num1 = atoi(originQuadruple.quadrupleList[j]->src1.c_str());
//                int num2 = atoi(originQuadruple.quadrupleList[j]->src2.c_str());
//                int resConst = originQuadruple.quadrupleList[j]->op == ADD ? num1 + num2 :
//                               originQuadruple.quadrupleList[j]->op == SUB ? num1 - num2 :
//                               originQuadruple.quadrupleList[j]->op == MUL ? num1 * num2 :
//                               originQuadruple.quadrupleList[j]->op == DIV ? num1 / num2 : 0;
//                for (int k = ;
//                     k < (i == functionEntry.size() - 1 ? originQuadruple.length() : functionEntry[i + 1] - 1); ++k) {
//
//                }
//            }
//        }
//    }
//
//}
//
//void Optimizer::globalRegisterOptimize() {
//
//}
//
//void Optimizer::blockPublicExpressionOptimize() {
//
//}
//
//void Optimizer::splitIntoFunction() {
//    for (int i = 0; i < originQuadruple.length(); i++) {
//        if (originQuadruple.quadrupleList[i]->op == FUNCTION_DEFINE ||
//            originQuadruple.quadrupleList[i]->op == MAIN_FUNCTION_DEFINE) {
//            functionEntry.push_back(i);
//        }
//    }
//}
//
//void Optimizer::splitIntoBasicBlock() {
//    for (int i = 0; i < originQuadruple.length(); i++) {
//        if (originQuadruple.quadrupleList[i]->op == J ||
//            originQuadruple.quadrupleList[i]->op == BNE ||
//            originQuadruple.quadrupleList[i]->op == BLE ||
//            originQuadruple.quadrupleList[i]->op == BL ||
//            originQuadruple.quadrupleList[i]->op == BGE ||
//            originQuadruple.quadrupleList[i]->op == BG ||
//            originQuadruple.quadrupleList[i]->op == SWITCH_BNE ||
//            originQuadruple.quadrupleList[i]->op == RET ||
//            originQuadruple.quadrupleList[i]->op == ||)
//    }
//}

Optimizer::Optimizer(Quadruple theOriginQuadruple, Quadruple theOptimizedQuadruple)
        : originQuadruple(theOriginQuadruple), optimizedQuadruple(theOptimizedQuadruple) {
    currentEntry = 1;
    currentExit = 1;
    constantCombinationOption = true;
    globalRegisterOption = true;
    blockPublicExpressionOption = true;
}