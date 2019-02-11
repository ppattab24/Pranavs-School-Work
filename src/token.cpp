#include "../header/token.h"

Token::Token() : status(error), str("") {}

Token::Token(string str, int status) : status(status), str(cutDown(str)) {}

Token::Token(string str, bool Tests) : str(cutDown(str)) {

    size_t testType = _whatKindOfTest(this->str);

    if (testType != string::npos && Tests) {

        status = testType;
        //_pruneTest();

    } else if (this->str == "&&" || this->str == "||") {

        status = connector;

    } else if (this->str == "(") {

        status = lParren;

    } else if (this->str == ")") {

        status = rParren;

    } else if (this->str[0] == '[') {   //jankyyy

        status = test2;

    } else if (!(this->str.empty()) && (this->str[0] == '\"' && this->str[str.size() - 1] == '\"')) {

        status = quotations;
        eraseBothSides(this->str, 1);
        this->str = cutDown(this->str);

    } else {

        status = middle;
    }
}

Token::Token(const vector<Token>& combineUs) {

    if (combineUs.empty()) {

        str = "";
        status = error;
    } else {

        //Set status explicitly
        status = combineUs[0].getStatus();

        //Combine strings, delimiting with ' '
        string strAggregate = "";

        for (size_t i = 0; i < combineUs.size(); i++) {

            Token currentToken = combineUs[i];  //changed from Token& to Token due to compile error
            strAggregate += currentToken.toString();
            strAggregate += ' ';
        }

        str = strAggregate;
        str = cutDown(str);
    }
}

void Token::setStatus(bool wasSuccessful) {

    assert(status == middle || this->isTest());   //could also be test command - added to assertion

    if (wasSuccessful) {

        this->status = good;
    } else {

        this->status = bad;
    }
}

void Token::setStatus(Status s) {

    this->status = s;

}

void Token::setString(string str) {

    this->str = str;

}

int Token::getStatus() const {
    return status;
}

string Token::toString() const {
    return str;
}

ostream& operator <<(ostream& outs, const Token& printMe)
{
    outs << printMe.str << " | Enum " << printMe.status;
    return outs;
}

bool operator ==(const Token& t, const string& str) {

    return t.toString() == str;
}

bool operator ==(const string& str, const Token& t) {

    return str == t.toString();
}

bool operator !=(const Token& t, const string& str) {

    return t.toString() != str;
}

bool operator !=(const string& str, const Token& t) {

    return str == t.toString();
}

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

size_t Token::_whatKindOfTest(string str) {

    str = cutDown(str);

    if (str.size() < 2) {

        return string::npos;
    }

    if ( !(str[0] == '[' && str[str.size() - 1] == ']') && toLowercase(str.substr(0, 4)) != "test")
        return string::npos;

    //Now, we know it's a token - just figure out what kind of token it is
    if (str.find("-d") != string::npos) {

        return Token::test1;

    } else if (str.find("-e") != string::npos) {

        return Token::test2;

    } else if (str.find("-f") != string::npos) {

        return Token::test3;

    } else {    //Default, if there is no flag included

        return Token::test2;
    }
}

void Token::_pruneTest() {

    //assert(this->isTest());

    //Get rid of [] brackets, or "test"
    if (str[0] == '[' && str[str.size() - 1] == ']') {

        eraseBothSides(str, 1);

    } else {

        assert(toLowercase(str.substr(0, 4)) == "test");

        str.erase(0, 4);
    }



    //Get rid of flags, if there is any
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

            //assert(status == Token::test2);

            foundIndex = str.find("-e");

            if (foundIndex == string::npos) {

                str = cutDown(str);
                return;
            }
        }
    }

    str = cutDown(str.substr(foundIndex + 3, string::npos));
}
