//
// Created by andyson on 18-11-20.
//

#ifndef GRAMMARANALYZER_EXCEPTIONHANDLER_H
#define GRAMMARANALYZER_EXCEPTIONHANDLER_H

#include <string>

using namespace std;

class ExceptionHandler {
public:
    int errorLine;

    static ExceptionHandler &getExceptionHandler();  // 单例设计模式

    void error(int errorCode, int line);

private:
    int errorCnt;
    ExceptionHandler();
};


#endif //GRAMMARANALYZER_EXCEPTIONHANDLER_H
