//
// Created by andyson on 18-11-30.
//

#ifndef C0COMPILER_SEMANTICANALYZER_H
#define C0COMPILER_SEMANTICANALYZER_H

#include "Type.h"
#include "Quadruple.h"

class SemanticAnalyzer {
public:
    static SemanticAnalyzer &getSemanticAnalyzer(Quadruple &theQuadruple);  // 单例设计模式

    const string newLabel(const string &label);
    //生成四元式的语义动作函数
    void varDefine(symbolType type, const string ident, int length, bool global);
    void mainFunctionDefine(valueType vType, const string ident);
    void functionDefine(valueType vType, const string ident);
    void parameter(symbolType type, const string ident);
    void functionEnd();

    void valueParameter(const string expr);
    void functionCall(const string ident, string &dst);
    void returnStatement(const string expr);

    void add(const string &src1, const string &src2, string &dst);
    void sub(const string &src1, const string &src2, string &dst);
    void mult(const string &src1, const string &src2, string &dst);
    void div(const string &src1, const string &src2, string &dst);

    void bz(symbolType comparator, const string &label, const string &src1, const string &src2);
    void bnz(symbolType comparator, const string &label, const string &src1, const string &src2);
    void switchBz(symbolType comparator, const string &label, const string &src2);
    void switchVar(const string &dst);
    void j(const string &label);

    void read(const string &dst, valueType type);
    void write(valueType type, const string &src1);

    void assign(const string &ident,
                const string &value,
                valueType identType,
                valueType valueType);
    void assignToArray(const string &arrayName,
                       const string &index,
                       const string &value,
                       valueType arrayType,
                       valueType valueType);
    void getFromArray(const string &arrname, const string &index, string &dst);
    void assignFromFunction();

    const string &label(const string &lab);
private:
    Quadruple &quadruple;
    int labelCnt;
    int functionCallCnt;
    SemanticAnalyzer(Quadruple &theQuadruple);
};


#endif //C0COMPILER_SEMANTICANALYZER_H
