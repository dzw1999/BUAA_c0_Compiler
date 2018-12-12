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


int main(int argc, char *argv[]) {
    string fileName;
    if (argc > 1)
        fileName = argv[1];
    else
        cin >> fileName;
    FILE *fin = fopen((fileName).c_str(), "r");
    if (fin == nullptr) {
        printf("File does not exist.\n");
        return 0;
    }
    FILE *grammarOut = fopen("Grammar Analysis.txt", "w");
    FILE *quadrupleOut = fopen("Quadruple.txt", "w");
    FILE *MIPSOut = fopen("MIPS code.asm", "w");

    LexicalAnalyzer lexicalAnalyzer = LexicalAnalyzer::getLexicalAnalyzer(fin);

    ExceptionHandler exceptionHandler = ExceptionHandler::getExceptionHandler();

    SymbolTable symbolTable = SymbolTable::getSymbolTable();

    Quadruple quadruple = Quadruple::getQuadruple(quadrupleOut);

    SemanticAnalyzer semanticAnalyzer = SemanticAnalyzer::getSemanticAnalyzer(quadruple);

    StackManager stackManager;

    GrammarAnalyzer grammarAnalyzer = GrammarAnalyzer(lexicalAnalyzer, exceptionHandler, symbolTable, semanticAnalyzer,
                                                      grammarOut);

    if (grammarAnalyzer.grammarAnalyze() == 1) {
        exceptionHandler.error(37, 0);
    }

    MIPSGenerator mipsGenerator = MIPSGenerator::getMIPSGenerator(quadruple, symbolTable, stackManager,
                                                                  exceptionHandler, MIPSOut);

    quadruple.output();

    mipsGenerator.generateMIPS();

    fclose(fin);
    fclose(grammarOut);
    fclose(quadrupleOut);
    fclose(MIPSOut);
    return 0;
}
