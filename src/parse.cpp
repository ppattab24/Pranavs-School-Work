#include "../src/parse.h"

Parse::Parse() 
{
	_initialize(NULL, ' ', true, false);
}

Parse::Parse(char* cstring, char delimiter, bool quotesSeparate, bool Tests)
{
	_initialize(cstring, delimiter, quotesSeparate, Tests);
}

Parse::Parse(string str, char delimiter, bool quotesSeparate, bool Tests)
{
	str 
}
