//
// Created by andyson on 18-11-20.
//

#include "ExceptionHandler.h"
#include <cstdio>

ExceptionHandler &ExceptionHandler::getExceptionHandler() {
    static ExceptionHandler instance;
    return instance;
}

void ExceptionHandler::error(int errorCode, int line, string symbol) {
    static string errorMsg[100] = {
            "No error",                                                             //0
            "Expected \"const\" at the beginning of constant declaration.",         //1
            "Expected ';' at the end of statement",
            "Expected '('",
            "Expected ')'",
            "Expected '{'",                                                         //5
            "Expected '}'",
            "Expected \"void\" at the beginning of void function definition.",
            "Expected an identifier",
            "No main function in this program",
            "Expected a type symbol",                                               //10
            "Expected a '='",
            "Expected a character",
            "Expected '['",
            "Expected ']'",
            "Expected a number",                                                    //15
            "Cannot find a proper symbol match the previous '{'",
            "Cannot find a proper symbol match the previous identifier",
            "Illegal first symbol for statement",
            "Illegal first symbol for assign statement",
            "Expected \"if\"",                                                      //20
            "Expected \"while\"",
            "Expected \"switch\"",
            "Expected \"case\"",
            "Expected an integer",
            "Expected ':'",                                                         //25
            "Expected \"default\"",
            "Expected \"scanf\"",
            "Expected \"printf\"",
            "Cannot find a proper symbol match the previous string",
            "Cannot find a proper symbol match the previous expression",            //30
            "Illegal first symbol for the parameter of printf statement",
            "Expected \"return\"",
            "Illegal first symbol for an integer",
            "(Not an error) var definition misunderstand function declaration",
            "(Not an error) void function declaration misunderstand main function", //35
            "No such function table",
            "Expected FINISH symbol at the end of your code",
            "Repeated definition",
            "Failed to add this identifier to symbol table",
            "Parameter error",                                                      //40
            "Used of undeclared identifier",
            "Unexpected '[' after a non-array identifier",
            "Unexpected '(' after a non-function identifier",
            "Cannot use a non-integer identifier for calculating",
            "Cannot use a void function for calculating",                           //45
            "Cannot assign a non-variable",
            "Cannot assign a unproper type value to the variable",
            "Cannot use a non-integer as the index",
            "Expected a '[' after an array identifier",
            "Cannot write a non-variable",                                          //50
            "Cannot write an array",
            "Unknown comparator",
            "Unknown quadruple operator",
            "In MIPS generating: Cannot find in symbol table",
            "Illegal symbol",                                                       //55
            "Cannot assign an invalid type value to a varible",
            "Expected an index when assigning an array"
    };
    errorCnt++;
    if(errorCode == 37)
        printf("%s\n", errorMsg[errorCode].c_str());
    else if (errorCode == 54)
        printf("MIPS line %d: %s\n", line, errorMsg[errorCode].c_str());
    else if (errorCode == 55)
        printf("line %d: %s : %s\n", line, errorMsg[errorCode].c_str(), symbol.c_str());
    else
        printf("line %d: %s\n", line, errorMsg[errorCode].c_str());
    //exit(0);
}

ExceptionHandler::ExceptionHandler() {
    errorCnt = 0;

}