#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include "LexicalAnalyzer.h"
#include "GrammarAnalyzer.h"
#include "ExceptionHandler.h"
#include "SymbolTable.h"
#include "SemanticAnalyzer.h"
#include "Quadruple.h"
#include "MIPSGenerator.h"
#include "Optimizer.h"


int main(int argc, char *argv[]) {
    string fileName;
    bool optimizationOption = false;
    if (argc > 1)
        fileName = argv[1];
    else
        printf("Please input a file path:\n");
    cin >> fileName;
    FILE *fin = fopen((fileName).c_str(), "r");
    if (fin == NULL) {
        printf("File does not exist.\n");
        return 0;
    }
    FILE *grammarOut = fopen("Grammar Analysis.txt", "w");
    FILE *originQuadrupleOut = fopen("Origin Quadruple.txt", "w");
    FILE *optimizedQuadrupleOut = fopen("Optimized Quadruple.txt", "w");
    FILE *MIPSOut = fopen("MIPS code.asm", "w");

    printf("Start compiling...\n");

    ExceptionHandler exceptionHandler = ExceptionHandler::getExceptionHandler();

    LexicalAnalyzer lexicalAnalyzer = LexicalAnalyzer::getLexicalAnalyzer(fin, exceptionHandler);

    SymbolTable symbolTable = SymbolTable::getSymbolTable();

    Quadruple originQuadruple(originQuadrupleOut);

    Quadruple optimizedQuadruple(optimizedQuadrupleOut);

    SemanticAnalyzer semanticAnalyzer = SemanticAnalyzer::getSemanticAnalyzer(originQuadruple);

    StackManager stackManager;

    GrammarAnalyzer grammarAnalyzer(lexicalAnalyzer, exceptionHandler, symbolTable, semanticAnalyzer, grammarOut);

    Optimizer optimizer = Optimizer::getOptimizer(originQuadruple, optimizedQuadruple);

    MIPSGenerator mipsGenerator = MIPSGenerator::getMIPSGenerator(symbolTable, stackManager, exceptionHandler, MIPSOut);

    // 词法分析
    lexicalAnalyzer.lexicalAnalyze();
    printf("Lexical analysis succeeded.\n");

    // 语法分析(语义分析伴随语法分析)
    if (grammarAnalyzer.grammarAnalyze() == 1)
        printf("Exception occurred in grammar analysis.\n");
    else
        printf("Grammar and semantic analysis succeeded.\n");
    fclose(grammarOut);

    //输出原始四元式
    originQuadruple.output();
    fclose(originQuadrupleOut);

    //优化
    //optimizer.optimize();
    printf("Optimization succeeded.\n");

    //输出优化后四元式
    optimizedQuadruple.output();
    fclose(optimizedQuadrupleOut);

    //生成MIPS
    if (optimizationOption)
        mipsGenerator.generateMIPS(optimizedQuadruple);
    else
        mipsGenerator.generateMIPS(originQuadruple);
    fclose(MIPSOut);

    fclose(fin);
    return 0;
}
