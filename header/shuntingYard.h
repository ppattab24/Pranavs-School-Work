#ifndef SHUNTINGYARD_H
#define SHUNTINGYARD_H

#include "genuse.h"

using namespace std;

class ShuntingYard {
private:
queue<token> delivery;
queue<token> commands;

stack<token> operators;

public:
void run();
ShuntingYard(queue<token> );


};


#endif 
