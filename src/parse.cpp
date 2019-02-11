#include "../header/parse.h"

Parse::Parse() {

    _init(NULL, ' ', true, false);
}

Parse::Parse(char *cstring, char parser, bool quotesSeparately, bool Tests) {

    _init(cstring, parser, quotesSeparately, Tests);
}

Parse::Parse(string str, char parser, bool quotesSeparately, bool Tests) {

    str = padParse(str, '(');
    str = padParse(str, ')');

    char* c = str_to_char(str);
    _init(c, parser, quotesSeparately, Tests);
    delete [] c;
}

bool Parse::done() const { return q.empty(); }

size_t Parse::size() const { return q.size(); }

void Parse::compressTokens() {

    queue<Token> replacementQueue;


    while(!q.empty()) {

        //Prepare to append to current Token
        vector<Token> aggregateVector;
        Token initialToken = q.front();
        aggregateVector.push_back(initialToken);
        q.pop();



        //Only consider appending if it's not blacklisted
        if (!_isBlacklisted(initialToken)) {


            while (!q.empty()

                   //              //Iterate through q for as long as the type matches initialToken
                   //              //Or, iterate through q for as long as the quotations extends
                   //              && (q.front().getStatus() == initialToken.getStatus() || initialToken.toString().at(0) == '\"')) {

                   //Iterate thorugh q for as long as we don't reach something that we should delimit
                   //Or, iterate through q for as long as the quotations extends
                   && (!_isBlacklisted(q.front()) || initialToken.toString().at(0) == '\"')) {

                Token appendMe = q.front();
                aggregateVector.push_back(appendMe);
                q.pop();

                //Exit the loop if we were in a quotations, and there's a \" at the end of this token to end the quotations
                if (initialToken.toString().at(0) == '\"'
                    && appendMe.toString().at(appendMe.toString().size() - 1) == '\"')
                    break;
            }

        }

        //Make the combine token based on what you collected, and push it into replacementQueue
        Token aggregateToken(aggregateVector);

        if(aggregateToken.isTest())
            aggregateToken._pruneTest();

        replacementQueue.push(aggregateToken);
    }



    //Replace queue
    addItems(q, replacementQueue);
    clearAll(replacementQueue);
//    q.swap(replacementQueue);
//    assert(replacementQueue.empty());
}

Parse& operator >>(Parse& parser, Token& t) {

    assert(!parser.q.empty());

    t = parser.q.front();
    parser.q.pop();

    return parser;
}

ostream& operator <<(ostream& outs, const Parse& d) {

    queue<Token> qCopy(d.q);

    outs << "{ ";

    while(!qCopy.empty()) {

        outs << "(" << qCopy.front() << ") ";
        qCopy.pop();
    }

    outs << "}";

    return outs;
}

void Parse::_init(char *cstring, char parser, bool quotesSeparately, bool Tests) {

    if (cstring == NULL)
        return;

    char* walker = cstring;
    bool weCareAboutQuotes = quotesSeparately && _Quotes(cstring);

    //Walk through the cstring
    while(*walker != '\0') {

        string currentStr = "";
        bool inQuoteFlag = false, commentFound = false;

        //Don't delimit if we're in a quotations!
        while (*walker != '\0' && ( (*walker != parser) || (inQuoteFlag && weCareAboutQuotes)) ) {

            char currentChar = *walker;

            if (currentChar == '\"')
                inQuoteFlag = !inQuoteFlag; //toggle whether we're in or not

            if (currentChar == '#' && !inQuoteFlag) {
                commentFound = true;
                break;
            }

            currentStr += currentChar;

            walker++;

        }

        if (!currentStr.empty())
        {
            if ( (!q.empty()) && (q.back().isTest()) && (Tests) && (currentStr != "&&" && currentStr != "||") )
                q.push(Token(currentStr, q.back().getStatus()));
            else
                q.push(Token(currentStr, Tests));
        }



        if (commentFound)
            return;

        if (*walker != '\0')
            walker++;
    }

}

bool Parse::_Quotes(char* c) {

    string str(c);
    return _Quotes(str);
}

bool Parse::_Quotes(string str) {

    size_t totalQuotes = 0;

    for (size_t i = 0; i < str.size(); i++) {

        if (str[i] == '\"')
            totalQuotes++;
    }

    return totalQuotes % 2 == 0;
}

void Parse::checkFlagsAndReinitStatus()
{
    queue<Token> replacementQueue;

    if(q.front().toString().substr(0,1) == "[")
        q.front()._pruneTest();

    while(!q.empty())
    {
        Token currentToken = q.front();

        if(currentToken.isTest())
        {
            if(currentToken.toString().substr(0, 2) == "-d")
            {
                currentToken.setStatus(Token::test1);

                string currentTok_str = currentToken.toString();
                currentTok_str.erase(0, 3);
                currentToken.setString(currentTok_str);
            }
            else if(currentToken.toString().substr(0, 2) == "-f")
            {
                currentToken.setStatus(Token::test3);

                string currentTok_str = currentToken.toString();
                currentTok_str.erase(0, 3);
                currentToken.setString(currentTok_str);
            }
        }

        replacementQueue.push(currentToken);
        q.pop();
    }


    //Replace queue
//    q.swap(replacementQueue);
//    assert(replacementQueue.empty());
    addItems(q, replacementQueue);
    clearAll(replacementQueue);
}

bool Parse::_isBlacklisted(Token t) {

    size_t thisStatus = t.getStatus();

    return thisStatus == Token::lParren
            || thisStatus == Token::rParren
            || thisStatus == Token::connector
            || thisStatus == Token::error;
}
