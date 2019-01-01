//
// Created by andyson on 18-12-1.
//

#ifndef C0COMPILER_STACKMANAGER_H
#define C0COMPILER_STACKMANAGER_H

#include <string>
#include <vector>
//Idiot Class
//本以为他会做更多事情，事实上这个类基本没用，他也不是运行栈
using namespace std;

class StackManager {
public:
    vector<string> functionStack;
    StackManager();
};


#endif //C0COMPILER_STACKMANAGER_H
