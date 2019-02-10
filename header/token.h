#ifndef TOKEN_H
#define TOKEN_H


#include <iostream>
#include <string>
#include <cassert>
#include <queue>

using namespace std;

class Token {

private:
int status;

public:
Token(string, int);

enum Status{ good, bad, middle,
    connector,
    error,
    quotations,
    lParren, rParren,
    test1, test2, test3};
    
void setStatus(status);
int getStatus();
string toString();
void setString(string );





};



#endif;
