#ifndef SHUNTINGYARD_H
#define SHUNTINGYARD_H

#include "genuse.h"

using namespace std;

class ShuntingYard {
private:
queue<Token> delivery;
queue<Token> command_token_queue;

stack<Token> opStack;

public:
void run();
ShuntingYard(queue<Token> commandQ );
queue<Token> getReversie();


};


#endif 
