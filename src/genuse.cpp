#include "genuse.h"

char* str_to_char(const string& stringCommand)
{
	char* cstr = new char[stringCommand.size() + 1];//+1 to null-terminate

	for (size_t i = 0; i < stringCommand.size(); i++) 
	{
		cstr[i] = stringCommand[i];
	}

	cstr[stringCommand.size()] = '\0';
	return cstr;
}

char toLowercase(char cstring )
{
	if ('A' <= cstring && cstring <= 'Z')        
	{
		return cstring - 'A' + 'a';
	}
	else 
	{
		return cstring;
	}
}

string toLowercase (string input)
{
	for (size_t i = 0; i < input.size(); i++) 
	{
		input[i] = toLower(input[i]);
	}
	return input;
}
