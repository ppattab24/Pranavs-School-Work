#include "../header/manager.h"

void Manager::run() {

    char input[1024];

    while(true)
    {
        memset(input, 0, sizeof(input));  //clears input for future input

        cout << "$ ";
        cin.getline(input, 1024);
        input[cin.gcount()] = '\0';  //null-terminated
        cout << endl;

        Token currLine;
        queue<Token> delimited_queue, main_token_queue;
        Parse d(input, ';', false, false);  //Constructor delimits input

        while(!d.done())
        {
            d >> currLine;

            Parse dd(currLine.toString(), ' ', true, true);
            dd.compressTokens();
            dd.checkFlagsAndReinitStatus();

            while(!dd.done())
            {
                Token currToken;
                dd >> currToken;

                delimited_queue.push(currToken);
            }

            //main_token_queue = combineCommands(delimited_queue);

            if(!parenthesisChecker(delimited_queue))
            {
                cerr << "ERROR: Uneven amount of parenthesis" << endl;
                continue;
            }

            shuntingYard sy(delimited_queue);
            queue<Token> evalQueue = sy.getReversePolish();
            evalPostFix(evalQueue);

            clearAll(delimited_queue);

            cout << endl;
        }
    }
}

void Manager::execute(string commandStr) {

    char * cStr = str_to_char(commandStr);
    char * cmd[64];
    memset(cmd, 0, sizeof(cmd));

    parse(cStr, cmd);

     pid_t process_id;
    int status;

    if (equals(*cmd, "exit", false)){

        //cerr << "Exiting!!" << endl;
        exit(0);
    }

    //cerr << "Would be running execute() here!!" << endl;
    if((process_id = fork()) < 0)   // if something went wrong with forking the process
    {
        //cerr << "ERROR: child process forking failed" << endl;
        //cerr << "In first block in execute()" << endl;
        exit(1);
    }
    else if (process_id == 0)       // if child process was created
    {
        //cerr << "In second block in execute()" << endl;

        if(execvp(*cmd, cmd) < 0)
        {
            cerr << "ERROR: command failed to execute()" << endl;
            wasSuccess = false;
        }
        else
        {
            //cerr << "hey I ran, wasSuccess should be true" << endl;
            wasSuccess = true;
        }
    }
    else
    {
        //cerr << "In third block in execute()" << endl;
        wasSuccess = true;
        while(wait(&status) != process_id);

        if(WEXITSTATUS(status)) //if it wasn't successful
            wasSuccess = false;
    }


    delete [] cStr;
}

/** parse
 * @brief Populates command (an array of cstrings) with cstrings delimited by whitespace
 * @param input Beginning of cstring containing all args
 * @param command array of cstrings which this function will populate
**/
void Manager::parse(char *input, char **command)
{
    while (*input != '\0')  //While you're not at the end of the cstring
    {

        //Replace whitespace with \0
        while (*input == ' ' || *input == '\t' || *input == '\n')
        {
            *input = '\0';
            input++;
        }

        //Save input to its place in command, and move command walker forwward
        *command = input;
        command++;

        //Advance input to the next non-whitespace character
        while (*input != '\0' && *input != ' ' && *input != '\t' && *input != '\n')
        {
            input++;
        }
    }
}

/**
 * @brief Evaluates the queue (already in postfix notation)
 * @param string_postfix_queue a queue that contains an expression in postfix notation
 */
void Manager::evalPostFix(queue<Token>& token_postfix_queue)
{
    stack<Token> token_eval_stack;
    vector<Token> vectorToEval;

    while(!token_postfix_queue.empty())
    {
        if (token_postfix_queue.front().getStatus() != Token::connector)
        {
            token_eval_stack.push(token_postfix_queue.front());
            token_postfix_queue.pop();
        }
        else //if token is an operator
        {
            //Prepare the binary expression

            Token op2 = token_eval_stack.top();
            token_eval_stack.pop();

            Token connector = token_postfix_queue.front();
            token_postfix_queue.pop();

            Token op1 = token_eval_stack.top();
            token_eval_stack.pop();

            // [command] [connector] [command]
            vectorToEval.push_back(op1);
            vectorToEval.push_back(connector);
            vectorToEval.push_back(op2);

            //Evaluate the binary expression!
            evaluate(vectorToEval); //updates wasSuccess
            vectorToEval.clear();   //clears vector for next bin expression

            //Push result onto the stack
            if (wasSuccess) {
                Token t("", Token::good);
                token_eval_stack.push(t);
            } else {
                Token t("", Token::bad);
                token_eval_stack.push(t);
            }
        }
    }

    if (token_eval_stack.size() == 1 && !token_eval_stack.top().toString().empty())
    {
        ifstream path(token_eval_stack.top().toString().c_str());

        switch(token_eval_stack.top().getStatus())
        {
            case Token::middle:
                execute(token_eval_stack.top().toString());
                break;
            case Token::test2:

                if(path.good())
                    cout << "(True)" << endl;
                else
                    cout << "(False)" << endl;

                break;
            case Token::test1:

                if(isThisADirectory(token_eval_stack.top().toString()))
                    cout << "(True)" << endl;
                else
                    cout << "(False)" << endl;

                break;
            case Token::test3:

                if(isThisAFile(token_eval_stack.top().toString()))
                    cout << "(True)" << endl;
                else
                    cout << "(False)" << endl;

                break;
            default:
                cerr << "ERROR: Incorrect token type to evaluate. Token is enum "
                     << token_eval_stack.top().getStatus() << endl;
                exit(7);
        }
    }
}

/**
 * @brief Evaluates a binary expression according to rules for && and ||
 * @param binExpression - expression of the form A ** B
 *        (where A and B are individually executable commands,
 *        and where ** is connector && or ||)
 * NOTE: Modifies Manager::wasSuccess
 */
void Manager::evaluate(vector<Token> binExpression)
{
    assert(binExpression.size() == 3);  //first command, connector, last command

    ifstream path(binExpression[0].toString().c_str());

    switch (binExpression[0].getStatus())
    {
        case Token::middle:

            execute(binExpression[0].toString());

            break;
        case Token::test2:

            wasSuccess = path.good();

            if (wasSuccess)
                cout << "(True)" << endl;
            else
                cout << "(False)" << endl;

            break;
        case Token::test1:

            wasSuccess = isThisADirectory(binExpression[0].toString());

            if (wasSuccess)
                cout << "(True)" << endl;
            else
                cout << "(False)" << endl;

            break;
        case Token::test3:

            wasSuccess = isThisAFile(binExpression[0].toString());

            if (wasSuccess)
                cout << "(True)" << endl;
            else
                cout << "(False)" << endl;

            break;
        default:
            cerr << "ERROR: Incorrect token type to evaluate. Token is enum "
                 << binExpression[0].getStatus() << endl;
            exit(7);

    }

    path.close();

    binExpression[0].setStatus(wasSuccess);

    if(shouldExecute(binExpression))
        execute(binExpression[2].toString());
}

/**
 * @brief Determines if connectors permit the execution of a command
 * @param vector of Tokens of either form "<cmd> ** <cmd>" or "** <cmd>" (where ** denotes a connector, && or ||)
 * NOTE: wasSuccess must be properly updated from the previous command!!!!!!!!!!!!!
**/
bool Manager::shouldExecute(vector<Token> expr)
{

    if ( (expr.size() != 1 && expr.size() != 3) ||
         (expr[0].getStatus() != Token::middle && expr[2].getStatus() != Token::middle) )
        return false;

    return ( (expr[0].getStatus() == Token::good && expr[1].toString() == "&&")
             || (expr[0].getStatus() == Token::bad && expr[1].toString() == "||") );
}

queue<Token> Manager::combineCommands(queue<Token>& old_token_queue)
{
    queue<Token> new_token_queue;

    while(!old_token_queue.empty())
    {
        Token t("", Token::middle);

        if(old_token_queue.front().getStatus() == Token::middle)
        {
            while (old_token_queue.front().getStatus() == Token::middle
                   || old_token_queue.front().getStatus() == Token::quotations)
            {
                t += old_token_queue.front();
                old_token_queue.pop();
            }

            new_token_queue.push(t);
        }
        else
        {
            new_token_queue.push(old_token_queue.front());
            old_token_queue.pop();
        }
    }

    return new_token_queue;
}

bool Manager::isThisADirectory(string pathname)
{
    struct stat sb;

    stat(pathname.c_str(), &sb);

    if ( access(pathname.c_str(),0) == 0 )
    {
        if (sb.st_mode & S_IFDIR) {
            wasSuccess = true;  //directory exists
            return true;
        }
        else {
            wasSuccess = false; //directory is not found
            return false;
        }
    }

    wasSuccess = false; //path is not found
    return false;
}

bool Manager::isThisAFile(string pathname)
{
    struct stat sb;

    stat(pathname.c_str(), &sb);

    if ( access(pathname.c_str(),0) == 0 )
    {
        if (sb.st_mode & S_IFREG) {
            wasSuccess = true;  //file exists
            return true;
        }
        else {
            wasSuccess = false; //file is not found
            return false;
        }
    }

    wasSuccess = false; //path is not found
    return false;
}

