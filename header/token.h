#ifndef TOKEN_H
#define TOKEN_H


#include <iostream>
#include <string>
#include <cassert>
#include <queue>

#include "genuse.h"

using namespace std;

class Token
{
public:
    Token(); 
    Token(string, int);
    Token(string, bool Tests = false);
    Token(const vector<Token>&); 

    enum Status { good, bad, middle,
        connector,
        error,
        quotations,
        lParren, rParren,
        test1, test2, test3
   	redirectLeft, redirectRight, Pipe };

    void setStatus(bool); 
    void setStatus(Status);

    int getStatus() const;

    string toString() const;

    void setString(string);

    friend ostream& operator <<(ostream&, const Token&);

    friend bool operator ==(const Token&, const string&);
    friend bool operator ==(const string&, const Token&);

    friend bool operator !=(const Token&, const string&);
    friend bool operator !=(const string&, const Token&);

    Token& operator +=(const Token&);

    bool isTest() const;

    void reduce();

private:
    int status;

    string str;

    size_t checkTest(string);
};


#endif 

