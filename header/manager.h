#ifndef MANAGER_H
#define MANAGER_H


#include <iostream>
#include <stack>
#include <queue>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <string>
#include <climits>
#include <cassert>
#include <vector>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h> //uncomment for hammer

#include "token.h"
#include "parse.h"
#include "genuse.h"
#include "shuntingYard.h"

using namespace std;

class Manager
{
public:
    void run();
private:
    void execute(char **command);
    void execute(string command);
    void parse(char *, char **);
    bool shouldExecute(vector<Token> expr);
    void evaluate(vector<Token> binExpression);
    void evalPostFix(queue<Token>& token_postfix_queue);
    queue<Token> combineCommands(queue<Token>& old_token_queue);
    bool isThisADirectory(string pathname);
    bool isThisAFile(string pathname);

    bool wasSuccess;
};


#endif //MANAGER_H

