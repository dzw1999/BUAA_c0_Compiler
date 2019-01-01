//
// Created by andyson on 18-12-1.
//

#ifndef C0COMPILER_MIPSGENERATOR_H
#define C0COMPILER_MIPSGENERATOR_H

#include "Quadruple.h"
#include "SymbolTable.h"
#include "StackManager.h"
#include "ExceptionHandler.h"
#include "Type.h"
#include "GlobalRegisterAllocation.h"

class MIPSGenerator {
public:
    static MIPSGenerator &getMIPSGenerator(SymbolTable &theSymbolTable, StackManager &theStackManager,
                                           GlobalRegisterAllocation &theGlobalRegisterAllocation,
                                           ExceptionHandler &theExceptionHandler,
                                           FILE *theMIPSFile
    );

    void generateMIPS(Quadruple quadruple);

private:
    SymbolTable &symbolTable;
    StackManager &stackManager;
    ExceptionHandler &exceptionHandler;
    GlobalRegisterAllocation &globalRegisterAllocation;
    FILE *MIPSFile;
    string MIPSTextCode[MAX_MIPS_TEXT_LENGTH], MIPSDataCode[MAX_MIPS_DATA_LENGTH];
    int MIPSTextLine, MIPSDataLine;
    bool debug;
    char buffer[MAX_MIPS_CODE_LENGTH];

    bool isNum(string a);

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

    void SAVE_SCENEToMIPS(Quad quad);

    void CALLToMIPS(Quad quad);

    void RETToMIPS(Quad quad);

    void PUSHToMIPS(Quad quad);

    void RIToMIPS(Quad quad);

    void RCToMIPS(Quad quad);

    void WSToMIPS(Quad quad);

    void WIToMIPS(Quad quad);

    void WCToMIPS(Quad quad);

    void VAR_DECLAREToMIPS(Quad quad);

    void MAIN_FUNCTION_DEFINEToMIPS(Quad quad);

    void FUNCTION_DEFINEToMIPS(Quad quad);

    void PARAMETERToMIPS(Quad quad);

    void FUNCTION_ENDToMIPS(Quad quad);

    void getAddrToMIPS(string src, int getSrc, bool &global);

    string getSrcToMIPS(string src, int quadSrc, bool intoReg = false);

    void writeDst(string dst, string reg = "$s1");

    void getLocal(int offset, int quadSrc);

    void getGlobal(int offset, int quadSrc);

    void getParameter(int paraNum, int paraOrder, int quadSrc);

    void getConst(string src, int quadSrc);

    void getConst(symTableEntry ste, int quadSrc);

    void writeLocal(int offset, string reg = "$s1");

    void writeGlobal(int offset, string reg = "$s1");

    void writeParameter(int paraNum, int paraOrder, string reg = "$s1");

    MIPSGenerator(SymbolTable &theSymbolTable, StackManager &theStackManager,
                  GlobalRegisterAllocation &theGlobalRegisterAllocation, ExceptionHandler &theExceptionHandler,
                  FILE *theMIPSFile);
};


#endif //C0COMPILER_MIPSGENERATOR_H
