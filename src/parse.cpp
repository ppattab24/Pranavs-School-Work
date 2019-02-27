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

        vector<Token> combinedVector;
        Token firstToken = q.front();
        combinedVector.push_back(firstToken);
        q.pop();



        if (!_isBlacklisted(firstToken)) {


            while (!q.empty()

                   && (!_isBlacklisted(q.front()) || firstToken.toString().at(0) == '\"')) {

                Token token_to_combine = q.front();
                combinedVector.push_back(token_to_combine);
                q.pop();

                if (firstToken.toString().at(0) == '\"'
                    && token_to_combine.toString().at(token_to_combine.toString().size() - 1) == '\"')
                    break;
            }

        }

        Token combinedToken(combinedVector);

        if(combinedToken.isTest())
            combinedToken.reduce();

        replacementQueue.push(combinedToken);
    }



    addItems(q, replacementQueue);
    clearAll(replacementQueue);
}

Parse& operator >>(Parse& parser, Token& t) {

    //assert(!parser.q.empty());

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

    if (cstring == NULL) {return;}

    char* walker = cstring;
    bool weCareAboutQuotes = quotesSeparately && _Quotes(cstring);

    while(*walker != '\0') {
        cout << "WE ARE HEAR" << endl << endl;

        string currentStr = "";
        bool inQuoteFlag = false, commentFound = false;

        while (*walker != '\0' && ( (*walker != parser) || (inQuoteFlag && weCareAboutQuotes)) ) {

            char currentChar = *walker;

            if (currentChar == '\"') {inQuoteFlag = true; }

            if (inQuoteFlag == false) {
                if (currentChar == '#') {
                    commentFound = true;
                    break;
                }
            }

            currentStr += currentChar;

            walker++;

        }

        if (!currentStr.empty())
        {
            if ( (!q.empty()) && (q.back().isTest()) && (Tests) && (currentStr != "&&" && currentStr != "||") ) 
            {
                q.push(Token(currentStr, q.back().getStatus()));
            }
            else {q.push(Token(currentStr, Tests));}
        }



        if (commentFound) {return;}

        if (*walker != '\0') {walker++;}
    }

}

bool Parse::_Quotes(char* c) {

    string str(c);
    return _Quotes(str);
}

bool Parse::_Quotes(string str) {

    size_t totalQuotes = 0;

    for (size_t i = 0; i < str.size(); i++) {

        if (str[i] == '\"') {totalQuotes++;}
    }

    return totalQuotes % 2 == 0;
}

void Parse::checkFlagsAndReinitStatus()
{
    queue<Token> replacementQueue;

    if(q.front().toString().substr(0,1) == "[") {q.front().reduce();}

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
