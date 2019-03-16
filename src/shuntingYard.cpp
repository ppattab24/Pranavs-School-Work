#include "../header/shuntingYard.h"

shuntingYard::shuntingYard(queue<Token> command)
{
    delivery = command;
    run();
}

void shuntingYard::run()
{
    Token temp;
    while(!delivery.empty())    
    {
//	cout << "\nShuntingYard is starting.\n";
        Token currentToken = delivery.front();
        delivery.pop();

//	cout << "***" << currentToken.toString() << "****" << endl;
        if( currentToken.getStatus() == Token::middle || currentToken.getStatus() == Token::quotations || currentToken.isTest() )
	{
		// This branch will only have commands
	//	cout << currentToken.toString() << endl;
		command_queue.push(currentToken);
	}
	
        if(currentToken.getStatus() == Token::connector)
        {

            while(!operator_stack.empty() && operator_stack.top().getStatus() != Token::lParren)
            {
                command_queue.push(operator_stack.top());
                operator_stack.pop();
            }

            operator_stack.push(currentToken);
        }

        if(currentToken.getStatus() == Token::redirectLeft)
        {

            while(!operator_stack.empty() && operator_stack.top().getStatus() != Token::lParren)
            {
                command_queue.push(operator_stack.top());
                operator_stack.pop();
            }

            operator_stack.push(currentToken);
        }

	if(currentToken.getStatus() == Token::redirectRight)
        {

            while(!operator_stack.empty() && operator_stack.top().getStatus() != Token::lParren)
            {
                command_queue.push(operator_stack.top());
                operator_stack.pop();
            }

            operator_stack.push(currentToken);
        }

	if(currentToken.getStatus() == Token::redirectDoubleRight)
        {

            while(!operator_stack.empty() && operator_stack.top().getStatus() != Token::lParren)
            {
                command_queue.push(operator_stack.top());
                operator_stack.pop();
            }

            operator_stack.push(currentToken);
        }

        if(currentToken.getStatus() == Token::Pipe)
        {

            while(!operator_stack.empty() && operator_stack.top().getStatus() != Token::lParren)
            {
                command_queue.push(operator_stack.top());
                operator_stack.pop();
            }

            operator_stack.push(currentToken);
        }




        if(currentToken.getStatus() == Token::lParren)
   	{ 
//		command_queue.push(operator_stack.top());
//		operator_stack.pop();
		operator_stack.push(currentToken);
	}

        if(currentToken.getStatus() == Token::rParren)
        {
//	    cout << "\nReached a ')'\n";
            while(/*operator_stack.empty() &&*/ operator_stack.top().getStatus() != Token::lParren)
            {
                command_queue.push(operator_stack.top());
                operator_stack.pop();
            }

            //assert(!operator_stack.empty());
            operator_stack.pop();  
        }
//	queue<Token> dummy = command_queue;
//	cout << "This is the queue: ";
//	while(!dummy.empty())
//	{
//		cout << dummy.front().toString() << " ";
//		dummy.pop();
//	}
//	cout << endl;

    }

    while(!operator_stack.empty())
    {
        command_queue.push(operator_stack.top());
        operator_stack.pop();
    }

 /*   	queue<Token> dummy = command_queue;

	cout << "This is the queue in shuntingYard: ";
	while(!dummy.empty())
	{
		cout << dummy.front().toString() << " ";
		dummy.pop();
	}
	cout << endl;*/


}

queue<Token> shuntingYard::getReversePolish() {
       //	queue<Token> temp = command_queue;
//	while(!temp.empty()){
//		cout << temp.front().toString() << endl;
//		temp.pop();
//	}
	return command_queue; 
}
