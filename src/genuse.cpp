#include "../header/genuse.h"

char* str_to_char(const string& str) {

	char* c = new char[str.size() + 1];//+1 to null-terminate

	for (size_t i = 0; i < str.size(); i++) {

	c[i] = str[i];
				    }
	c[str.size()] = '\0';

	return c;
}

string cutDown(const string& trimMe) {

	if(trimMe == "")  
		return trimMe;

	string whitespace = " \n\t";
	string newStr;

	size_t left = trimMe.find_first_not_of(whitespace);
	size_t right = trimMe.find_last_not_of(whitespace);

	if (left == UINT_MAX || right == UINT_MAX) {
		newStr = "";
        } 
	else {
	        newStr = trimMe.substr(left, right - left + 1);
	}

	return newStr;
}

bool equals(const char * cStr, string str, bool capsSensitive)
{
	string compareMeString;
	const char * compareMeCStr;
	bool shouldDelete;

	if (capsSensitive)
	{
		compareMeString = str;
		compareMeCStr = reinterpret_cast<char *>(*cStr);
		shouldDelete = false;
	}
	else
	{
		compareMeCStr = str_to_char(toLowercase(cStr));
		compareMeString = toLowercase(str);
		shouldDelete = true;
	}
	for (size_t i = 0; i < compareMeString.size(); i++)
	{
		if (compareMeString[i] != compareMeCStr[i])
		{
      	        	if (shouldDelete)
				delete [] compareMeCStr;
		                return false;
	        }
	 }
         if (shouldDelete)
		delete[] compareMeCStr;
		return true;
}

char toLowercase(char c)
{
	if ('A' <= c && c <= 'Z') {
		return c - 'A' + 'a';
	}
	else 
	{
	return c;
	}
}

string toLowercase(string str)
{
	    for (size_t i = 0; i < str.size(); i++) 
	    {
		str[i] = toLowercase(str[i]);
	    }
	        return str;
}

string padParse(string str, char parser)
{
	    for (size_t i = 0; i < str.size(); i++) 
	    {
	            if (str[i] == parser && (i == 0 || str[i - 1] != ' ')) 
		    {
		            string delimStr(1, ' ');
		            str.insert(i, delimStr);
  	                    i++;    
		     }
	            if (str[i] == parser && (i == str.size() - 1 || str[i + 1] != ' ')) {
	                    string delimStr(1, ' ');
			    str.insert(i + 1, delimStr);
		            i++;
		     }
	     }
	     return str;
}

bool parenthesisChecker(queue<Token> checkMe)
{
	    stack<char> s;

	    while(!checkMe.empty()) {
		   if(checkMe.front().getStatus() == Token::lParren)
		   {
		    	s.push('(');
			checkMe.pop();
		   }
		   else if(checkMe.front().getStatus() == Token::rParren)
		   {
		   	if(s.empty() || s.top() != '(')
		   		return false;
			s.pop();
			checkMe.pop();
		   }
		   else
		   	checkMe.pop();
	    }
	    return s.empty();
}

void eraseBothSides(string& trimMe, size_t thisManyChars) {

	    assert(trimMe.size() >= (thisManyChars * 2));

	        if (thisManyChars == 0)
			return;

		 trimMe.erase(0, thisManyChars);
		 trimMe.erase(trimMe.size() - thisManyChars, string::npos);
}

void clearAll(queue<Token>& queue_to_clear) {
	    while(!queue_to_clear.empty())
	    	queue_to_clear.pop();
}

void addItems(queue<Token>& queue_to_AddTo, queue<Token> original) {

	    clearAll(queue_to_AddTo);

	    while(!original.empty()) {

	    Token& t = original.front();
	    queue_to_AddTo.push(t);
	    original.pop();
	    }
}
