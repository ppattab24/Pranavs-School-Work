#include "../header/manager.h"

vector<Token> cl;

void Manager::run() {

    char input[1024];

    while(true)
    {
        memset(input, 0, sizeof(input));  

        cout << "$ ";
        cin.getline(input, 1024);
        input[cin.gcount()] = '\0';  
        cout << endl;

        Token currLine;
        queue<Token> delimited_queue, main_token_queue;
        Parse firstParse(input, ';', true, true);  
	
	int i = 0; 

        while(!firstParse.done())
        {
            firstParse >> currLine;

            Parse secondParse(currLine.toString(), ' ', true, true);
            secondParse.compressTokens();
            secondParse.checkFlagsAndReinitStatus();

            while(!secondParse.done())
            {
                Token currToken;
                secondParse >> currToken;

		//cout << currToken.toString() << endl;

                delimited_queue.push(currToken);
		cl.push_back(currToken);
	//	cout << cl.at(i).toString() << " ";
		++i;
            }
	   // cout << endl;


            shuntingYard sy(delimited_queue);
            queue<Token> evalQueue = sy.getReversePolish();
            evalParsed(evalQueue);

            clearAll(delimited_queue);

	    cl.clear();

            cout << endl;
        }
    }
}

void Manager::execute(string commandStr) {

//	cout << commandStr << endl;
    char * cStr = str_to_char(commandStr);
    char * cmd[64];
    memset(cmd, 0, sizeof(cmd));

    parse(cStr, cmd);

    pid_t process_id;
    int status;

    if (equals(*cmd, "exit", false)){

        exit(0);
    }

    
    if((process_id = fork()) < 0)   
    {
        exit(1);
    }
    else if (process_id == 0)     
    {

        if(execvp(*cmd, cmd) < 0)
        {
            cerr << "ERROR: command failed to execute()" << endl;
            wasSuccess = false;
        }
        else
        {
            
            wasSuccess = true;
        }
    }
    else
    {
        
        wasSuccess = true;
        while(wait(&status) != process_id);

        if(WEXITSTATUS(status)) {wasSuccess = false;}
    }


    delete [] cStr;
}

void Manager::parse(char *input, char **command)
{
    while (*input != '\0')  
    {

        while (*input == ' ' || *input == '\t' || *input == '\n')
        {
            *input = '\0';
            input++;
        }

        *command = input;
        command++;
        
        while (*input != '\0' && *input != ' ' && *input != '\t' && *input != '\n')
        {
            input++;
        }
    }
}

void Manager::evalParsed(queue<Token>& token_postfix_queue)
{
    bool AndBeforeEndOr = false;
    bool singleExecutionOr = false;
    bool checkBeforeEndOr = false;
    stack<Token> token_eval_stack;
    stack<Token> special_token_eval_stack;
    vector<Token> vectorToEval;
    vector<string> operators;
    int i = 0;	
    int numAmpersandsLor = 0;
    int numConnectors = 0;
    stack<Token> temp;
    
    queue<Token> dummy = token_postfix_queue;
    queue<Token> connectors;

    cout << "This is the queue: ";
    while(!dummy.empty())
    {
 	cout << dummy.front().toString() << " ";
	if(dummy.front().getStatus() == 2)
		operators.push_back(" ");
	else
	{
		operators.push_back(dummy.front().toString());
	}

	if(dummy.front().toString() != "&&" && dummy.front().toString() != "||")
	{	
		special_token_eval_stack.push(dummy.front());
	}
	else
	{
		connectors.push(dummy.front());
		++numConnectors;
	}
	dummy.pop();
	
   }
   cout << endl;

   cout << "This is the vector:_";

   for(size_t i = 0; i < operators.size(); ++i)
   {
    	cout << operators.at(i) << "_";
   }

	cout << endl;
	if(operators.size() > 2)
   		if(operators.at(2) != "||" && operators.at(2) != "&&")
			singleExecutionOr = true;
		else if(numConnectors == 2 && operators.at(operators.size() - 1) == "||")
			singleExecutionOr = true;
   if(operators.at(operators.size() - 1) == "||")
   {
    	if((operators.at(operators.size() - 2) == "||") || (operators.at(operators.size() - 2) == "&&"))
    	{
    		checkBeforeEndOr = true;
    	}
	else
       		checkBeforeEndOr = false;	
// change
    	if((operators.at(operators.size() - 2) == "&&"))
	{
		AndBeforeEndOr = true;
		for(size_t j = 0; j < cl.size(); ++j)
		{
			if(cl.at(j).toString() == "&&")
				++numAmpersandsLor; 	

			else if(cl.at(j).toString() == "||")
				break;	
		}
	}
   }

   //cout << "checkBeforeEndOr = " << checkBeforeEndOr << endl;

    while(!token_postfix_queue.empty())
    {
        if (token_postfix_queue.front().getStatus() != Token::connector)
        {	    
		if(!token_eval_stack.empty())
		{
//		    cout << "The status of top of stack is: " << token_eval_stack.top().getStatus() << endl;
//		    cout << "The size of the stack is: " << token_eval_stack.size() << endl;
	    	if(token_eval_stack.top().getStatus() == 1 && token_eval_stack.size() == 1)
		{
			while(token_postfix_queue.front().getStatus() != Token::connector)
			{
//				cout << "This is the current front of the postfix queue " << token_postfix_queue.front().toString() << endl; 
				if(token_eval_stack.top().getStatus() != 1)
					token_postfix_queue.pop();

				else break;

			}
//			cout << "The size of the queue is: " << token_postfix_queue.size() << endl;
			 dummy = token_postfix_queue;

//			 cout << "This is the queue: ";
			while(!dummy.empty())
			{
//				cout << dummy.front().toString() << " ";
				dummy.pop();
			}

				if(token_postfix_queue.size() == 1)
				{
//					cout << "The front of the queue: " << token_postfix_queue.front().toString() << endl;  
					if(token_postfix_queue.front().toString() == "||")
					{
						token_postfix_queue.pop();
						continue;
					}
				}					
		}
	    }
   
              token_eval_stack.push(token_postfix_queue.front());
              token_postfix_queue.pop(); 
        }
        else 
        {
		if(checkBeforeEndOr)
		{
			stack<Token> flipped_eval_stack;
			stack<Token> special_flipped_eval_stack;
			while(!token_eval_stack.empty())
			{
				flipped_eval_stack.push(token_eval_stack.top());
				token_eval_stack.pop();
			}

			while(!special_token_eval_stack.empty())
			{
				special_flipped_eval_stack.push(special_token_eval_stack.top());
				special_token_eval_stack.pop();
			}


		//	cout << "singleExecutionOr = " << singleExecutionOr << endl;
			if(singleExecutionOr)
			{
                         	token_eval_stack = special_flipped_eval_stack;

				Token blah = token_eval_stack.top();
				vectorToEval.push_back(blah);
				token_eval_stack.pop(); 

				evaluate(vectorToEval);
				vectorToEval.clear();
				
				if(wasSuccess)
				{
					while(!token_postfix_queue.empty())
						token_postfix_queue.pop();
					break;
				}
				else
				{
					Token op1 = token_eval_stack.top();
				        token_eval_stack.pop();

            				Token connector = token_postfix_queue.front();
            				token_postfix_queue.pop();

            				Token op2 = token_eval_stack.top();
            				token_eval_stack.pop();

            				vectorToEval.push_back(op1);
            				vectorToEval.push_back(connector);
            				vectorToEval.push_back(op2);
           				evaluate(vectorToEval); 
            				vectorToEval.clear();  
					break;
				}	
			}

			// The error is here with numAmpersandsLor
			// because we are executing just the first three expressions in the queue and disregarding the rest
			// This doesn't work because in our case, the first && expression doesn't execute
			if(numAmpersandsLor != 0)
			{
				token_eval_stack = special_flipped_eval_stack;
				for(int z = 0; z < numAmpersandsLor; ++z)
				{
					Token blah1 = token_eval_stack.top();
					vectorToEval.push_back(blah1);
					token_eval_stack.pop();
			
					Token blah2 = connectors.front();
					connectors.pop();
					vectorToEval.push_back(blah2);
				
					Token blah3 = token_eval_stack.top();
					vectorToEval.push_back(blah3);
					token_eval_stack.pop();
					
				//	cout << vectorToEval.at(2).toString() << endl;

					evaluate(vectorToEval);
					vectorToEval.clear();

				//	cout << "The size of vectorToEval is " << vectorToEval.size() << endl;
					cout << "numAmpersandsLor= " << numAmpersandsLor << endl;
					if(wasSuccess)
					{
					  	    	Token t("", Token::good);
                					token_eval_stack.push(t);
					}
					else if(AndBeforeEndOr && !wasSuccess)
					{
			 				Token t("", Token::bad);
                					token_eval_stack.push(t);
							AndBeforeEndOr = false;
					}
					else
					{
						++numAmpersandsLor;
					}
				}
				while(!token_postfix_queue.empty())
					token_postfix_queue.pop();

				break;
	
			}
			else
			{
				token_eval_stack = flipped_eval_stack;

				Token blah = token_eval_stack.top();
				vectorToEval.push_back(blah);
				token_eval_stack.pop();
			
				blah = token_postfix_queue.front();
				token_postfix_queue.pop();
				vectorToEval.push_back(blah);
			
				blah = token_eval_stack.top();
				vectorToEval.push_back(blah);
				token_eval_stack.pop();

				evaluate(vectorToEval);
				vectorToEval.clear();
				
				while(!token_postfix_queue.empty())
					token_postfix_queue.pop();

				break;
			}
		}
//		cout << "\nThis is the Stack: " << endl;
//		temp = token_eval_stack;
//		while(!temp.empty()){
//		cout << temp.top().toString() << endl;
//		temp.pop();
		//cout << token_postfix_queue.front().toString() << endl;
//		}

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
	    }

            
            if (wasSuccess) 
	    {
		    // this might be the problem
//		    if(token_postfix_queue.back().toString() == "||")
//		    {
  //              	Token t("", Token::bad);
    //           		token_eval_stack.push(t);
//			cout << token_eval_stack.top().getStatus() << endl;
//		    }

//		    else
//		    {
                	Token t("", Token::good);
                	token_eval_stack.push(t);
//		    }
            } 
	    
	    else 
	    {
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
	    case Token::test1:
		if(goodDirectory(token_eval_stack.top().toString()))
                    cout << "(True)" << endl;
                else
                    cout << "(False)" << endl;

                break;
	    case Token::test2:

                if(path.good())
                    cout << "(True)" << endl;
                else
                    cout << "(False)" << endl;

                break;

	    case Token::test3:

                if(goodFile(token_eval_stack.top().toString()))
                    cout << "(True)" << endl;
                else
                    cout << "(False)" << endl;

                break;
          
                default:
                cerr << "ERROR: Incorrect token type: "
                     << token_eval_stack.top().getStatus() << endl;
                exit(7);
        }
    }
}

void Manager::evaluate(vector<Token> bin)
{
    ifstream path(bin[0].toString().c_str());
 //   cout << "The bin size is " << bin.size() << endl; 
  //  cout << "This Token is '" << bin[0].toString() << "' it's status is: ";
 //   cout << bin[0].getStatus() << endl << endl;
    switch (bin[0].getStatus())
    {
        case Token::middle:
//cout << bin[0].toString() << endl;
    	    execute(bin[0].toString());
            break;

	case Token::good:
	    break;

	case Token::bad:
//	    cout << "wasSuccess = " << wasSuccess << endl;
	    break;
	case Token::test1:

	    wasSuccess = goodDirectory(bin[0].toString());

            if (wasSuccess)
                cout << "(True)" << endl;
            else
                cout << "(False)" << endl;

            break;

        case Token::test2:

            wasSuccess = path.good();

            if (wasSuccess) {cout << "(True)" << endl;}
            else {cout << "(False)" << endl;}

            break;

	case Token::test3:

	     wasSuccess = goodFile(bin[0].toString());

            if (wasSuccess)
                cout << "(True)" << endl;
            else
                cout << "(False)" << endl;

            break;

	case Token::connector:
	    //cout << "This is the connector case in evalate" << endl << endl; 
	    wasSuccess = false;
	    break;
	    
        default:
            cerr << "This is our error: ERROR: Token type: " << bin[0].getStatus() << endl;
            exit(7);

    }

    path.close();
//	cout << "We got to <path.close();>" << endl;

    bin[0].setStatus(wasSuccess);
 //   cout << bin[2].toString() << endl;
    if(shouldExecute(bin))
        execute(bin[2].toString());
}

bool Manager::shouldExecute(vector<Token> expression)
{

    if ( (expression.size() != 1 && expression.size() != 3) ||
         (expression[0].getStatus() != Token::middle && expression[2].getStatus() != Token::middle) )
        return false;

    return ( (expression[0].getStatus() == Token::good && expression[1].toString() == "&&")
             || (expression[0].getStatus() == Token::bad && expression[1].toString() == "||") );
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

bool Manager::goodDirectory(string pathname)
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

bool Manager::goodFile(string pathname)
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
