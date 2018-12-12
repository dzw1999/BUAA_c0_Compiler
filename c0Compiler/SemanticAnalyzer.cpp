//
// Created by andyson on 18-11-30.
//

#include "SemanticAnalyzer.h"
#include <sstream>
#include <cstring>

SemanticAnalyzer &SemanticAnalyzer::getSemanticAnalyzer(Quadruple &theQuadruple) {
    static SemanticAnalyzer instance(theQuadruple);
    return instance;
}


SemanticAnalyzer::SemanticAnalyzer(Quadruple &theQuadruple) : quadruple(theQuadruple) {
    quadruple = theQuadruple;
    labelCnt = 0;
    functionCallCnt = 0;
}

const string SemanticAnalyzer::newLabel(const string &label) {
    static string newLab;
    newLab.clear();
    newLab.append("_label_");
    newLab.append(to_string(labelCnt++));
    return newLab;
}

void SemanticAnalyzer::constDefine(symbolType type, string ident, const string &value, bool global) {
    Operator op = CONST_DECLARE;
    string src1 = type == INT ? "int" : "char";
    string src2 = value;
    quadruple.addQuadruple(op, ident, src1, src2, global);
}

void SemanticAnalyzer::varDefine(symbolType type, string ident, int length, bool global) {
    Operator op = VAR_DECLARE;
    string src1 = type == INT ? "int" : "char";
    string src2 = to_string(length == 0 ? 1 : length);
    quadruple.addQuadruple(op, ident, src1, src2, global);
}

void SemanticAnalyzer::mainFunctionDefine(valueType vType, const string ident) {
    Operator op = MAIN_FUNCTION_DEFINE;
    string src1 = "void";
    quadruple.addQuadruple(op, ident, src1, "");
}

void SemanticAnalyzer::functionDefine(valueType vType, string ident) {
    Operator op = FUNCTION_DEFINE;
    string src1 = vType == INT_TYPE ? "int" :
                  vType == CHAR_TYPE ? "char" :
                  "void";
    quadruple.addQuadruple(op, ident, src1, "");
}

void SemanticAnalyzer::parameter(symbolType type, string ident) {
    Operator op = PARAMETER_DECLARE;
    string src1 = type == INT ? "int" : "char";
    quadruple.addQuadruple(op, ident, src1, "");
}

void SemanticAnalyzer::functionEnd() {
    Operator op = FUNCTION_END;
    quadruple.addQuadruple(op, "", "", "");
}

void SemanticAnalyzer::valueParameter(const string expr) {
    Operator op = PUSH;
    string src1 = expr;
    quadruple.addQuadruple(op, "", src1, "");
}

void SemanticAnalyzer::functionCall(const string ident, string &dst) {
    Operator op = CALL;
    string src1 = ident;
    dst = "@RET";
    dst.append(to_string(functionCallCnt++));
    quadruple.addQuadruple(op, dst, src1, "");
}

void SemanticAnalyzer::returnStatement(const string expr) {
    Operator op = RET;
    string src1 = expr;
    quadruple.addQuadruple(op, "", src1, "");
}

void SemanticAnalyzer::add(const string &src1, const string &src2, string &dst) {
    dst = quadruple.newTemp();
    quadruple.addQuadruple(ADD, dst, src1, src2);
}

void SemanticAnalyzer::sub(const string &src1, const string &src2, string &dst) {
    dst = quadruple.newTemp();
    quadruple.addQuadruple(SUB, dst, src1, src2);
}

void SemanticAnalyzer::mult(const string &src1, const string &src2, string &dst) {
    dst = quadruple.newTemp();
    quadruple.addQuadruple(MUL, dst, src1, src2);
}

void SemanticAnalyzer::div(const string &src1, const string &src2, string &dst) {
    dst = quadruple.newTemp();
    quadruple.addQuadruple(DIV, dst, src1, src2);
}

void SemanticAnalyzer::bz(symbolType comparator, const string &label, const string &src1, const string &src2) {
    Operator jmpOp;
    switch (comparator) {
        case EQUAL:
            jmpOp = BNE;
            break;
        case LESS_EQUAL:
            jmpOp = BG;
            break;
        case GREATER_EQUAL:
            jmpOp = BL;
            break;
        case LESS:
            jmpOp = BGE;
            break;
        case GREATER:
            jmpOp = BLE;
            break;
        case UNEQUAL:
            jmpOp = BEQ;
            break;
        default:
            throw 52;
    }
    quadruple.addQuadruple(jmpOp, label, src1, src2);
}

void SemanticAnalyzer::bnz(symbolType comparator, const string &label, const string &src1, const string &src2) {
    Operator jmpOp;
    switch (comparator) {
        case EQUAL:
            jmpOp = BEQ;
            break;
        case LESS_EQUAL:
            jmpOp = BLE;
            break;
        case GREATER_EQUAL:
            jmpOp = BGE;
            break;
        case LESS:
            jmpOp = BL;
            break;
        case GREATER:
            jmpOp = BG;
            break;
        case UNEQUAL:
            jmpOp = BNE;
            break;
        default:
            throw 52;
    }
    quadruple.addQuadruple(jmpOp, label, src1, src2);
}

void SemanticAnalyzer::switchBz(symbolType comparator, const string &label, const string &src1, const string &src2) {
    quadruple.addQuadruple(SWITCH_BNE, label, src1, src2);
}

void SemanticAnalyzer::switchVar(const string &dst) {
    quadruple.addQuadruple(SWITCH_VAR, dst, "", "");
}

void SemanticAnalyzer::j(const string &label) {
    quadruple.addQuadruple(J, "", label, "");
}

void SemanticAnalyzer::read(const string &dst, valueType type) {
    Operator op = type == INT_TYPE ? RI : RC;
    quadruple.addQuadruple(op, dst, "", "");
}

void SemanticAnalyzer::write(valueType type, const string &src1) {
    Operator op = (type == STRING_TYPE) ? WS :
                  (type == CHAR_TYPE || type == CHAR_ARRAY_TYPE) ? WC :
                  WI;
    quadruple.addQuadruple(op, "", src1, "");
}

void SemanticAnalyzer::assign(const string &ident, const string &value, valueType identType, valueType valueType) {
    quadruple.addQuadruple(ASS, ident, value, "");
}

void SemanticAnalyzer::assignToArray(const string &arrayName, const string &index, const string &value,
                                     valueType arrayType, valueType valueType) {
    quadruple.addQuadruple(ARAS, arrayName, value, index);
}

void SemanticAnalyzer::getFromArray(const string &arrname, const string &index, string &dst) {
    quadruple.addQuadruple(GAR, dst, arrname, index);
}

const string &SemanticAnalyzer::label(const string &lab) {
    quadruple.addQuadruple(LAB, "", lab, "");
}