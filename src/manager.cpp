#include "../header/manager.h"

void Manager::run() 
{
	char userInput[1024];

	while(true)
	{
		memset(userInput, 0, sizeof(userInput));
		cout << "$ ";
		cin.getline(userInput, 1024);
		userInput[cin.gcount()] = '\0';
		cout << endl;
	}
	Token userLine;
	queue<Token> delimited_token_queue, main_token_queue;
	Parse firstParse(userInput, ';', false, false);
	
	while(!limit.done())
	{
		limit >> userLine;

		Parse secondParse(userLine.toString(), ' ', true, true);
		secondParse.compressContents();
		secondParse.reinitializeStates();

		while(!secondParse.done())
		{
			Token thisInput;
			secondParse >> thisInput;
			delimited_token_queue.push(thisInput);
		}

		// Parenthesis check here

		ShuntingYard output(delimited-token_queue);
		queue<Token> parsedQueue = output.getReversePolish();
		evalPostFix(parsedQueue);
		clearQueue(delimited_token_queue);
		cout << endl;
	}
}

void Manager::execute(string command_string) {
	
	char* cString = str_to_char(command_string);
	char* command[64];
	memset(command, 0, sizeof(command));
	
	parse(command_string, command);
	
	execute(command);
	
	delete[] cString;
	
	
}

void Manager::execute(char** command) {
	
	pid_t process_id;	
	int status;
	
	if (equals(*command, "exit", false)) {
		exit(0);
	}
	
	if ((process_id = fork()) < 0) {
		exit(1)
	}
	else if (process_id == 0) {
		if (execvp(*command, command) < 0) {
			cerr<< "Command did not execute:failed" << endl;
			worked = false;
		}
		else {
			worked = true;	
		}
	}
	else {
		worked = true;
		while (wait(&status) != process_id);
		
		if (WEXITSTATUS(status)) {
			worked = false;
		}
	}
	
	
}

void Manager::parse(char *line, char **command)
{
    while (*line != '\0')  
    {

        while (*line == ' ' || *line == '\t' || *line == '\n')
        {
            *line = '\0';
            line++;
        }

        *command = line;
        command++;

        while (*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n')
        {
            line++;
        }
    }
}

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
        else
	{
            Token op2 = token_eval_stack.top();
            token_eval_stack.pop();

            Token connector = token_postfix_queue.front();
            token_postfix_queue.pop();

            Token op1 = token_eval_stack.top();
            token_eval_stack.pop();

            vectorToEval.push_back(op1);
            vectorToEval.push_back(connector);
            vectorToEval.push_back(op2);

            evaluate(vectorToEval);
	    vectorToEval.clear();  
            if (successful ) {
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

            successful  = path.good();

            if (successful )
                cout << "(True)" << endl;
            else
                cout << "(False)" << endl;

            break;
        case Token::test1:

            successful  = isThisADirectory(binExpression[0].toString());

            if (successful )
                cout << "(True)" << endl;
            else
                cout << "(False)" << endl;

            break;
        case Token::test3:

            successful  = isThisAFile(binExpression[0].toString());

            if (successful )
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

    binExpression[0].setStatus(successful );

    if(shouldExecute(binExpression))
        execute(binExpression[2].toString());
}

bool Manager::shouldExecute(vector<Token> expr)
{

    if ( (expr.size() != 1 && expr.size() != 3) ||
         (expr[0].getStatus() != Token::middle && expr[2].getStatus() != Token::middle) )
        return false;

    return ( (expr[0].getStatus() == Token::good && expr[1].toString() == "&&")
             || (expr[0].getStatus() == Token::bad && expr[1].toString() == "||") );
}

queue<Token> Manager::combineCommands(queue<Token>& old_queue)
{
    queue<Token> new_token_queue;

    while(!old_queue.empty())
    {
        Token t("", Token::middle);

        if(old_queue.front().getStatus() == Token::middle)
        {
            while (old_queue.front().getStatus() == Token::middle
                   || old_queue.front().getStatus() == Token::quotations)
            {
                t += old_queue.front();
                old_queue.pop();
            }

            new_token_queue.push(t);
        }
        else
        {
            new_token_queue.push(old_queue.front());
            old_queue.pop();
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
            successful  = true;  //directory exists
            return true;
        }
        else {
            successful  = false; //directory is not found
            return false;
        }
    }

    successful  = false; //path is not found
    return false;
}

bool Manager::isThisAFile(string pathname)
{
    struct stat sb;

    stat(pathname.c_str(), &sb);

    if ( access(pathname.c_str(),0) == 0 )
    {
        if (sb.st_mode & S_IFREG) {
            successful  = true;  //file exists
            return true;
        }
        else {
            successful  = false; //file is not found
            return false;
        }
    }

    successful  = false; //path is not found
    return false;
}

