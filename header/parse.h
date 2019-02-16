#ifndef PARSE_H
#define PARSE_H

#include <iostream>
#include <queue>
#include <string>
#include <cassert>

#include "token.h"
#include "genuse.h"

using namespace std;

class Parse
{
public:
    Parse();

    Parse(char*, char, bool, bool);
    Parse(string, char, bool, bool);

    bool done() const;
    size_t size() const;

    void compressTokens();

    void checkFlagsAndReinitStatus();

    friend Parse& operator >>(Parse&, Token&);

    friend ostream& operator <<(ostream&, const Parse&);
private:

    void _init(char*, char, bool, bool);

    bool _Quotes(string);
    bool _Quotes(char*);

    bool _isBlacklisted(Token);

    queue<Token> q;
};

#endif
