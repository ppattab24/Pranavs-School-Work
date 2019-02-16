#include "../header/token.h"

Token::Token() : status(error), str("") {}

Token::Token(string str, int status) : status(status), str(cutDown(str)) {}

Token::Token(string str, bool Tests) : str(cutDown(str)) {

    size_t testType = checkTest(this->str);

    if (testType != string::npos && Tests) {

        status = testType;


    } else if (this->str == "&&" || this->str == "||") {

        status = connector;

    } else if (this->str[0] == '[') { 

        status = test2;

    } else if (!(this->str.empty()) && (this->str[0] == '\"' && this->str[str.size() - 1] == '\"')) {

        status = quotations;
        eraseBothSides(this->str, 1);
        this->str = cutDown(this->str);

    } else {

        status = middle;
    }
}

Token::Token(const vector<Token>& tokens_to_combine) {

    if (tokens_to_combine.empty()) {

        str = "";
        status = error;
    } else {

    
        status = tokens_to_combine[0].getStatus();


        string combinedString = "";

        for (size_t i = 0; i < tokens_to_combine.size(); i++) {

            Token currentToken = tokens_to_combine[i];
            combinedString += currentToken.toString();
            combinedString += ' ';
        }

        str = combinedString;
        str = cutDown(str);
    }
}

void Token::setStatus(bool wasSuccessful) {

    //assert(status == middle || this->isTest());  

    if (wasSuccessful) {this->status = good;} 
    
    else {this->status = bad;}
}

void Token::setStatus(Status s) {this->status = s;}

void Token::setString(string str) {this->str = str;}

int Token::getStatus() const {return status;}

string Token::toString() const {return str;}

ostream& operator <<(ostream& outs, const Token& printMe)
{
    outs << printMe.str << " | Enum " << printMe.status;
    return outs;
}

bool operator ==(const Token& t, const string& str) {return t.toString() == str;}

bool operator ==(const string& str, const Token& t) {return str == t.toString();}

bool operator !=(const Token& t, const string& str) {return t.toString() != str;}

bool operator !=(const string& str, const Token& t) {return str == t.toString();}

Token &Token::operator+=(const Token &t) {

    assert(this->getStatus() == Token::middle);

    this->str += " " + t.str;

    this->str = cutDown(this->str);

    return *this;
}

bool Token::isTest() const
{
    return (this->status == Token::test1)
           || (this->status == Token::test2)
           || (this->status == Token::test3);
}

size_t Token::checkTest(string str) {

    str = cutDown(str);

    if (str.size() < 2) {

        return string::npos;
    }

    if ( !(str[0] == '[' && str[str.size() - 1] == ']') && toLowercase(str.substr(0, 4)) != "test") {return string::npos;}


    if (str.find("-d") != string::npos) {return Token::test1;} 
    else if (str.find("-e") != string::npos) {return Token::test2;} 
    else if (str.find("-f") != string::npos) {return Token::test3;} 
    else {return Token::test2;}
}

void Token::reduce() {


    if (str[0] == '[' && str[str.size() - 1] == ']') {eraseBothSides(str, 1);} 
    else 
    {
        assert(toLowercase(str.substr(0, 4)) == "test");
        str.erase(0, 4);
    }

    size_t foundIndex;

    switch (status) {

        case Token::test1: {

            foundIndex = str.find("-d");
            assert(foundIndex != string::npos);

            break;
        }

        case Token::test3: {

            foundIndex = str.find("-f");
            assert(foundIndex != string::npos);

            break;
        }

        default: {

            foundIndex = str.find("-e");

            if (foundIndex == string::npos) {

                str = cutDown(str);
                return;
            }
        }
    }

    str = cutDown(str.substr(foundIndex + 3, string::npos));
}
