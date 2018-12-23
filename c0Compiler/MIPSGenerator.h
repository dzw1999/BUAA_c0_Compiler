//
// Created by andyson on 18-12-1.
//

#ifndef C0COMPILER_MIPSGENERATOR_H
#define C0COMPILER_MIPSGENERATOR_H

#include "Quadruple.h"
#include "SymbolTable.h"
#include "StackManager.h"
#include "ExceptionHandler.h"

#define MAX_MIPS_LENGTH 5000
#define MAX_MIPS_DATA_CODE 500
#define MAX_MIPS_CODE_LENGTH 200

class MIPSGenerator {
public:
    static MIPSGenerator &getMIPSGenerator(Quadruple &theQuadruple, SymbolTable &theSymbolTable, StackManager &theStackManager, ExceptionHandler &theExceptionHandler, FILE *theMIPSFile);

    void generateMIPS();
private:
    Quadruple quadruple;
    SymbolTable symbolTable;
    StackManager stackManager;
    ExceptionHandler exceptionHandler;
    FILE *MIPSFile;
    char MIPSTextCode[MAX_MIPS_LENGTH][MAX_MIPS_CODE_LENGTH], MIPSDataCode[MAX_MIPS_DATA_CODE][MAX_MIPS_CODE_LENGTH];
    int MIPSTextLine, MIPSDataLine;

    void generateMIPSOfQuad(Quad quad);

    void ADDToMIPS(Quad quad);
    void SUBToMIPS(Quad quad);
    void MULToMIPS(Quad quad);
    void DIVToMIPS(Quad quad);
    void ASSToMIPS(Quad quad);
    void ARASToMIPS(Quad quad);
    void GARToMIPS(Quad quad);
    void LABToMIPS(Quad quad);
    void BGToMIPS(Quad quad);
    void BGEToMIPS(Quad quad);
    void BLToMIPS(Quad quad);
    void BLEToMIPS(Quad quad);
    void BEQToMIPS(Quad quad);
    void BNEToMIPS(Quad quad);
    void SWITCH_BNEToMIPS(Quad quad);
    void SWITCH_VARToMIPS(Quad quad);
    void JToMIPS(Quad quad);
    void CALLToMIPS(Quad quad);
    void RETToMIPS(Quad quad);
    void PUSHToMIPS(Quad quad);
    void RIToMIPS(Quad quad);
    void RCToMIPS(Quad quad);
    void WSToMIPS(Quad quad);
    void WIToMIPS(Quad quad);
    void WCToMIPS(Quad quad);
    void VAR_DECLAREToMIPS(Quad quad);
    void CONST_DECLAREToMIPS(Quad quad);
    void MAIN_FUNCTION_DEFINEToMIPS(Quad quad);
    void FUNCTION_DEFINEToMIPS(Quad quad);
    void PARAMETERToMIPS(Quad quad);
    void FUNCTION_ENDToMIPS(Quad quad);

    void getAddrtoMIPS(string src, int getSrc, bool &global);
    void getSrcToMIPS(string src, int quadSrc);
    void writeDst(string dst);
    void getLocal(int offset, int quadSrc);
    void getGlobal(int offset, int quadSrc);
    void getParameter(int paraNum,int paraOrder, int quadSrc);
    void getConst(string src, int quadSrc);
    void getString(string src, int quadSrc);
    void writeLocal(int offset);
    void writeGlobal(int offset);
    void writeParameter(int paraNum,int paraOrder);

    MIPSGenerator(Quadruple &theQuadruple, SymbolTable &theSymbolTable, StackManager &theStackManager, ExceptionHandler &theExceptionHandler, FILE *theMIPSFile);
};


#endif //C0COMPILER_MIPSGENERATOR_H
