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

string cutDown(const string& token_to_cutDown) {

    if(token_to_cutDown == "")  
        return token_to_cutDown;

    string whiteSpace = " \n\t";
    string newString;

    size_t left = token_to_cutDown.find_first_not_of(whiteSpace);
    size_t right = token_to_cutDown.find_last_not_of(whiteSpace);

    if (left == UINT_MAX || right == UINT_MAX) {

        newString = "";

    } else {

        newString = token_to_cutDown.substr(left, right - left + 1);
    }

    return newString;
}

void eraseBothSides(string& stringToCutDown, size_t amountOfChars) {

    assert(stringToCutDown.size() >= (amountOfChars * 2));

    if (amountOfChars == 0)
        return;

    stringToCutDown.erase(0, amountOfChars);
    stringToCutDown.erase(stringToCutDown.size() - amountOfChars, string::npos);
}
