#ifndef DELIM_H
#define DELIM_H

#include <iostream>
#include <queue>
#include <string>
#include <cassert>

#include "token.h"
#include "util.h"

using namespace std;

class Parse
{
public:
    Parse();

    Parse(char* cstring, char parser, bool quotesSeparately, bool Tests);
    Parse(string str, char parser, bool quotesSeparately, bool Tests);

    bool done() const;
    size_t size() const;

    /**
     * @brief combineRelated Groups together commands with several parts while leaving non-command Tokens isolated.
     * For use for expressions in normal notation ONLY, i.e. not postfix.
     *
     * Example: (ls) (-a) (;) (cat) (man) => (ls -a) (;) (cat man)
     * To be used in "emergencies" only, since improper use of this function causes erratic behavior.
     *
     * Tokens are compressed if they are a) of the same status,
     * or b) part of a quotations encapsulated by space-padded quotation marks.
     *
     * For example, improperly running compressCmds() on the following queue
     *      (ls) (echo hello) (cat m.txt) (&&) (||)
     * will produce the following undesired output:
     *      (ls echo hello cat m.txt) (&& ||)
     *
     * Please be extremely careful with this function!!!!!!
     */
    void compressTokens();

    //Used only to check for test commands
    void checkFlagsAndReinitStatus();

    friend Parse& operator >>(Parse& parser, Token& t);

    friend ostream& operator <<(ostream& outs, const Parse& d);
private:

    /**
     * @brief _init initializes Parse
     * @param cstring Takes the contents of cstring, and populates the queue q within this object
     * @param parser the delimiter character
     */
    void _init(char* cstring, char parser, bool quotesSeparately, bool Tests);

    bool _Quotes(string str);
    bool _Quotes(char* c);

    bool _isBlacklisted(Token t);

    queue<Token> q;
};

#endif // DELIM_H
