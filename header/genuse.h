#include <iostream>
#include <string>
#include <climits>
#include <queue>
#include <stack>

#include "token.h"


using namespace std;

class Token;

char* str_to_char(const string& str);

string cutDown(const string& trimMe);

bool equals(const char * cStr, string str, bool capsSensitive);

char toLowercase(char c);

string toLowercase(string str);

string padParse(string str, char parser);

bool parenthesisChecker(queue<Token> checkMe);

bool testChecker(queue<Token> checkMe);

void eraseBothSides(string& trimMe, size_t thisManyChars);

void clearAll(queue<Token>& queue_to_clear);

void addItems(queue<Token>& queue_to_AddTo, queue<Token> original);
