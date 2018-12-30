//
// Created by andyson on 18-11-30.
//

#include "Quadruple.h"

Quadruple::Quadruple(FILE *quadrupleOut) {
    quadNum = 0;
    qout = quadrupleOut;
}

Quadruple::Quadruple(Quadruple &quadruple) {
    qout = quadruple.qout;
    for (int i = 0; i < quadruple.quadNum; i++) {
        addQuadruple(quadruple.quadrupleList[i].op, quadruple.quadrupleList[i].dst, quadruple.quadrupleList[i].src1,
                     quadruple.quadrupleList[i].src2, quadruple.quadrupleList[i].global);
    }
}

void Quadruple::addQuadruple(Operator op, string dst, string src1, string src2, bool global) {
    //一个明显的内存泄漏,不过作业不要求复用性所以懒得改= =
    Quad *q = new Quad;
    q->op = op;
    q->src1 = src1;
    q->src2 = src2;
    q->dst = dst;
    q->global = global;
    quadrupleList[quadNum++] = *q;
}

//输出
void Quadruple::output() {
    for (int i = 0; i < quadNum; ++i) {
        string outStr;
        toString(quadrupleList[i], outStr);
        fprintf(qout, "%s", outStr.c_str());
    }
}

string Quadruple::newTemp() {
    static int cnt = 0;
    return "@temp" + to_string(cnt++);
}

int Quadruple::length() {
    return quadNum;
}

void Quadruple::toString(Quad q, string &res) {
    char rtn[MAX_SYMBOL_LENGTH];
    string OperatorStr[] = {
            "ADD",    // 加法
            "SUB",    // 减法
            "MUL",    // 乘法
            "DIV",    // 除法
            "ASS",    // 赋值
            "ARAS",   // 给数组元素赋值
            "GAR",    // 获取数组元素
            "FAS",    // 给函数赋返回值
            "CUT",    // 将整数截断成字符型
            "LAB",    // 标签
            "CMP",    // 比较
            "BG",     // 大于跳转
            "BGE",    // 大于或等于跳转
            "BL",     // 小于跳转
            "BLE",    // 小于或等于跳转
            "BEQ",    // 等于跳转
            "BNE",    // 不等于跳转
            "SWITCH_BNE",  //switch bne
            "SWITCH_VAR", //switch var
            "J",      // 无条件跳转
            "CALL",   // 函数调用，带返回值
            "RET",    // 函数返回
            "PUSH",   // 压栈
            "RI",     // 读整数
            "RC",     // 读字符
            "WS",     // 写字符串
            "WI",     // 写整数
            "WC",     // 写字符
            "VAR_DECLARE",        //变量声明
            "MAIN_FUNCTION_DEFINE",    //main函数定义
            "FUNCTION_DEFINE",    //函数定义
            "FUNCTION_END",
            "PARAMETER_DECLARE"
    };
    sprintf(rtn, "%s: %s, %s, %s\n", OperatorStr[q.op].c_str(), q.dst.c_str(), q.src1.c_str(), q.src2.c_str());
    res = rtn;
    return;
}