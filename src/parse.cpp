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
	str = padDelim(str, '(');
	str = padDelim(str, ')');

	char* cstring = str_to_char(str);
	_initialize(cstring, delimiter, quotesSeparate, Tests);
	delete [] c;
}

bool Parse::done() const { return q.empty(); }

size_t Parse::size() const { return q.size(); }

void Parse::compressContents()
{
	queue<Token> replacementQueue;

	while(!q.empty())
	{
		vector<Token> combinationVector;
		Token firstToken = q.front();
	        combinedVector.push_back(firstToken);
		q.pop();

		if(!_isGoodCommand(firstToken)) 
		{
			while(!q.empty() && (!_isGoodCommand(q.front()) || firstToken.toString().at(0) == '\"')) 
			{
				Token addToken = q.front();
				combinedVector.push_back(addToken);
				q.pop();

				if(firstToken.toString().at(0) == '\"' && addToken.toString().at(addToken.toString().size() - 1) == '\"') break;
			}
		}
		if(combinedToken.isTest())
			combinedToken._pruneTest();
		replacementQueue.push(combinedToken);	
	}
	addItems(q, replacementQueue);
	clearAll(replacementQueue);
}

Parse& operator >>(Parse& delim, Token& t)
{
	assert(!delim.q.empty());
	t = delim.q.front();
	delim.q.pop();
	return delim;
}

ostream& operator <<(ostream& outs, const Parse& d) 
{
	queue<Token> qCopy(d.q);
	outs << "{ ";
	while(!qCopy.pop.empty())
	{
		outs << "(" << qCopy.front() << ") ";
		qCopy.pop();
	}

	outs << "}";
	return outs;
}

void Parse::_initialize(char *cstring, char delim, bool quotesSeparately, bool Tests) {

    if (cstring == NULL)
        return;

    char* temp = cstring;
    bool careAboutQuotes = separateQuotes&& _Quotes(cstring);

    while(*temp != '\0') {

        string currentString = "";
        bool inQuoteFlag = false, commentFound = false;

        while (*temp != '\0' && ( (*temp != delim) || (inQuoteFlag && careAboutQuotes)) ) {

            char currChar = *temp;

            if (currChar == '\"')
                inQuoteFlag = !inQuoteFlag; //toggle whether we're in or not

            if (currChar == '#' && !inQuoteFlag) {
                commentFound = true;
                break;
            }

            currentString += currChar;

            temp++;

        }

        if (!currentString.empty())
        {
            if ( (!q.empty()) && (q.back().isTest()) && (Tests) && (currentString != "&&" && currentString != "||") )
                q.push(Token(currentString, q.back().getStatus()));
            else
                q.push(Token(currentString, Tests));
        }



        if (commentFound)
            return;

        if (*temp != '\0')
            temp++;
    }

}

bool Parse::_Quotes(char* cstring) {

    string str(cstring);
    return _Quotes(str);
}

bool Parse::_Quotes(string str) {

    size_t totalInput = 0;

    for (size_t i = 0; i < str.size(); i++) {

        if (str[i] == '\"')
            totalInput++;
    }

    return totalInput % 2 == 0;
}

void Parse::reinitializeStates()
{
    queue<Token> replacement;

    if(q.front().toString().substr(0,1) == "[")
        q.front()._pruneTest();

    while(!q.empty())
    {
        Token currToken = q.front();

        if(currToken.isTest())
        {
            if(currToken.toString().substr(0, 2) == "-d")
            {
                currToken.setStatus(Token::test1);

                string currToken_string = currToken.toString();
                currToken_string.erase(0, 3);
                currToken.setString(currToken_string);
            }
            else if(currToken.toString().substr(0, 2) == "-f")
            {
                currToken.setStatus(Token::test3);

                string currToken_string = currToken.toString();
                currToken_string.erase(0, 3);
                currToken.setString(currToken_string);
            }
        }

        replacement.push(currToken);
        q.pop();
    }
}

bool Delim::_isGoodComand(Token t) {

    size_t currentTokenStatus = t.getStatus();

    return thisStatus == Token::lParren 
            || thisStatus == Token::rParren
            || thisStatus == Token::connector
            || thisStatus == Token::error;
}
