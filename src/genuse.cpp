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

void clearQueue(queue<Token>& queue) {
    while(!queue.empty())
        queue.pop();
}

void populateQueue(queue<Token>& queueAddItems, queue<Token> original) {

    clearQueue(queueAddItems);

    while(!original.empty()) {

        Token& t = original.front();
        queueAddItems.push(t);
        original.pop();

    }
}

string padDelim(string str, char delim)
{
    for (size_t i = 0; i < str.size(); i++) {

        //Pad left of delim, if necessary
        if (str[i] == delim
            && (i == 0 || str[i - 1] != ' ')) {//safe from i=0 by short-circuit evaluation

            string delimStr(1, ' ');
            str.insert(i, delimStr);

            i++;    //Keep up the loop with the insertion
        }

        //Pad right of delim, if necessary
        if (str[i] == delim
            && (i == str.size() - 1 || str[i + 1] != ' ')) {

            string delimStr(1, ' ');
            str.insert(i + 1, delimStr);

            i++;
        }
    }

    return str;
}
