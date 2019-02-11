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
string str;
size_t testKind(string str);

public:
Token(); // don't use
Token(string str, int status);
Token(string str, bool Tests = false);
Token(const vector<Token>& tokens_to_merge);
enum Status{ good, bad, middle,
    connector,
    error,
    quotations,
    lParren, rParren,
    test1, test2, test3};
    
void setStatus(Status);
void setStatus(bool);
int getStatus();
string toString();
void setString(string );
void _pruneTest();

friend ostream& operator <<(ostream& outs, const Token& printMe);

friend bool operator ==(const Token& t, const string& str);
friend bool operator ==(const string& str, const Token& t);

friend bool operator !=(const Token& t, const string& str);
friend bool operator !=(const string& str, const Token& t);

Token& operator +=(const Token& t);

bool isTest() const;


};



#endif
