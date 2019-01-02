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
#include "GlobalRegisterAllocation.h"

int main(int argc, char *argv[]) {
    string fileName;
    bool optimizationOption = true;
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
    FILE *optimizedQuadrupleOut;
    if(optimizationOption)
        optimizedQuadrupleOut = fopen("Optimized Quadruple.txt", "w");
    else
        remove("Optimized Quadruple.txt");
    FILE *MIPSOut = fopen("MIPS code.asm", "w");

    printf("Start compiling...\n");

    ExceptionHandler exceptionHandler = ExceptionHandler::getExceptionHandler();

    LexicalAnalyzer lexicalAnalyzer = LexicalAnalyzer::getLexicalAnalyzer(fin, exceptionHandler);

    SymbolTable symbolTable = SymbolTable::getSymbolTable();

    Quadruple quadruple;

    SemanticAnalyzer semanticAnalyzer = SemanticAnalyzer::getSemanticAnalyzer(quadruple);

    StackManager stackManager;

    GrammarAnalyzer grammarAnalyzer(lexicalAnalyzer, exceptionHandler, symbolTable, semanticAnalyzer, grammarOut);

    GlobalRegisterAllocation globalRegisterAllocation(quadruple);

    Optimizer optimizer = Optimizer::getOptimizer(quadruple);

    MIPSGenerator mipsGenerator = MIPSGenerator::getMIPSGenerator(symbolTable, stackManager, globalRegisterAllocation, exceptionHandler, MIPSOut);


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
    quadruple.output(originQuadrupleOut);
    fclose(originQuadrupleOut);



    //优化
    if(optimizationOption) {
        optimizer.optimize();
        printf("Optimization succeeded.\n");
        //输出优化后四元式
        quadruple.output(optimizedQuadrupleOut);
        //全局寄存器分配
        globalRegisterAllocation.allocate();
        fclose(optimizedQuadrupleOut);
    }

    //生成MIPS
    mipsGenerator.generateMIPS(quadruple, optimizationOption);
    fclose(MIPSOut);

    fclose(fin);
    return 0;
}
