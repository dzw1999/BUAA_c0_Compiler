//
// Created by andyson on 18-11-20.
//

#ifndef GRAMMARANALYZER_GRAMMARANALYZER_H
#define GRAMMARANALYZER_GRAMMARANALYZER_H

#include <string>

#include "LexicalAnalyzer.h"
#include "ExceptionHandler.h"
#include "SymbolTable.h"
#include "SemanticAnalyzer.h"

class GrammarAnalyzer {
public:
    GrammarAnalyzer(
            LexicalAnalyzer &theLexicalAnalyzer,
            ExceptionHandler &theExceptionHandler,
            SymbolTable &theSymbolTable,
            SemanticAnalyzer &theSemanticAnalyzer,
            FILE *out);

    int grammarAnalyze();      //语法分析

private:
    LexicalAnalyzer &lexicalAnalyzer;
    ExceptionHandler &exceptionHandler;
    SymbolTable &symbolTable;
    SemanticAnalyzer &semanticAnalyzer;

    FILE *fout;     //输出文件

    int symbolCnt;
    int saveSymbolCnt;
    string currentFunction;

    void printmsg(string format, int line);

    //各种语法成分递归分析
    void program();
    void constDeclare();
    void varDeclare();
    void nonVoidFunctionDefine();
    void voidFunctionDefine();
    void mainFunction();
    void constDefine();
    void declareHead(string &functionName, valueType *vType);
    void varDefine();
    void compoundStatement();
    int parameterList();
    void parameter(int cnt);
    void expression(string &rtn_expr, valueType &type, bool &variable, int &staticValue, bool isSwitch = false);
    void term(string &rtn_term, valueType &type, bool &variable, int &staticValue);
    void factor(string &rtn_factor, valueType &type, bool &variable, int &staticValue);
    void statement();
    void assignStatement();
    void ifStatement();
    void condition(symbolType &comparator, string &src1, string &src2);
    void whileStatement();
    void switchStatement();
    void caseList(const string &src1, const valueType &type, const string &switchEndLabel);
    void caseStatement(const string &src1, const valueType &type, const string &switchEndLabel);
    void defaultStatement(const string &switchEndLabel);
    void functionCall(string &rtn_func, valueType &type);
    void valueParameterList(string functionName);
    void statementList();
    void scanfStatement();
    void printfStatement();
    void returnStatement();
    int integer();



};


#endif //GRAMMARANALYZER_GRAMMARANALYZER_H
