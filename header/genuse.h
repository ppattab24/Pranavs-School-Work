#include <iostream>
#include <string>
#include <climits>
#include <queue>
#include <stack>
#include <cstring>
#include <cassert>

#include "token.h"
class token;

using namespace std;

char* str_to_char(const string&);

char toLowercase(char);

string toLowercase (string);

string cutDown(const string&);

void eraseBothSides(string&, size_t);

void clearAll(queue<Token>& queue);

void addItems(queue<Token>& queueAddItems, queue<Token> original);

bool equals(const char *, string, bool);

string padDelim(string, char);

// We do not need the parenthesisChecker right now
// testChecker and myXOR are unneccessary
