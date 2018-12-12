//
// Created by andyson on 18-11-13.
//

#ifndef LEXICALANALYZER_LEXICALANALYZER_H
#define LEXICALANALYZER_LEXICALANALYZER_H

#include <cstdio>
#include <map>
#include "Type.h"

using namespace std;

class LexicalAnalyzer {
public:
    static LexicalAnalyzer &getLexicalAnalyzer(FILE *fin);  // 单例设计模式

    string symbolList[MAX_SYMBOL_NUM];          // 存储symbol的列表
    symbolType symbolTypeList[MAX_SYMBOL_NUM];  // 存储symbol的类型的列表
    int symbolLineList[MAX_SYMBOL_NUM];         // 存储symbol的行数

    char symGot[MAX_SYMBOL_LENGTH];             // 当前symbol
    symbolType symType;                         // 当前symbol类型

    int symbolNum;                              // 当前symbol总数
    map<string, symbolType> keywords;           // 关键字表

    int lineCnt;                                // 行计数器
    int columnCnt;                              // 列计数器

    void output(FILE *fout);                    // 输出词法分析结果函数

    void lexicalAnalyze();                      // 分析所有词,入list

private:
    FILE *fin;                          // 输入文件
    char charGot;                       // 当前字符

    LexicalAnalyzer(FILE *input);       // 真的的构造函数,实现单例

    void nextChar();                    // 读下一个字符

    void nextSymbol();                  // 读下一Symbol

    string enumToStr(symbolType st);    // 输出枚举类型的函数
};


#endif //LEXICALANALYZER_LEXICALANALYZER_H
