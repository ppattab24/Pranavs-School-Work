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

		    Parse(char* cstring, char delimiter, bool quotesSeparate, bool Tests);
		    Parse(string str, char delimiter, bool quotesSeparate, bool Tests);

		    bool done() const;
		    size_t size() const;
		    void compressContents();
		    void reinitializeStates();
		    friend Parse& operator >> (Parse& delimiter, Token& t);
		    friend ostream& operator << (ostream& outs, const Parse& delimiter);
	private:
		    void _initialize(char* cstring, char delimiter, bool quotesSeparate, bool worryAboutTests);
		    bool _Quotes (string str);
		    bool _Quotes (char* c);
		    bool _isGoodCommand(Token);

		    queue<Token> q;
};

#endif
