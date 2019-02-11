#include <iostream>
#include <string>
#include <climits>
#include <queue>
#include <stack>

#include "token.h"
class Token;    //Token constructor uses cutDown, so there is circular dependency

using namespace std;

//IMPORTANT: need to deallocate the char* after finishing the use of str_to_char
char* str_to_char(const string& str);

string cutDown(const string& trimMe);

bool equals(const char * cStr, string str, bool capsSensitive);

char toLowercase(char c);

string toLowercase(string str);

string padParse(string str, char parser);

bool parenthesisChecker(queue<Token> checkMe);

bool testChecker(queue<Token> checkMe);

bool myXOR(bool val1, bool val2);

void eraseBothSides(string& trimMe, size_t thisManyChars);

void clearAll(queue<Token>& queue_to_clear);

void addItems(queue<Token>& queue_to_AddTo, queue<Token> original);
