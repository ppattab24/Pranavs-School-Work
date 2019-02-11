#ifndef MANAGER_H
#define MANAGER_H

#include <string>
#include <cstring>
#include <stack>
#include <queue>
#include <unistd.h>
#include <cstdlib>
#include <climits>
#include <cassert>
#include <vector>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include "token.h"
#include "parse.h"
#include "genuse.h"
#include "shuntingYard.h"

using namespace std;

class Manager {
     public:
	void run();
     private:
	void execute(char **command);
	void execute(string command);
	void parse(char*, char **);
	bool shouldExecute(vector<Token> commands);
	void evaluate(vector<Token> binExpression);
	void evalPostFix(queue<Token>& postfix_queue);
	queue<Token> combineCommands(queue<Token>& old_queue);
	bool isThisADirectory(string pathname);
	bool isThisAFile(string pathname);

	bool successful;
};

#endif
