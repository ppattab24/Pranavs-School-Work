#include "../header/shuntingYard.h"

shuntingYard::shuntingYard(queue<Token> command)
{
    delivery = command;
    run();
}

void shuntingYard::run()
{
    while(!delivery.empty())    
    {
        Token currentToken = delivery.front();
        delivery.pop();


        if( currentToken.getStatus() == Token::middle || currentToken.getStatus() == Token::quotations
                || currentToken.isTest() )
            command_queue.push(currentToken);
        if(currentToken.getStatus() == Token::connector)
        {

            while(!operator_stack.empty() && operator_stack.top().getStatus() != Token::lParren)
            {
                command_queue.push(operator_stack.top());
                operator_stack.pop();
            }

            operator_stack.push(currentToken);
        }

        if(currentToken.getStatus() == Token::lParren)
            operator_stack.push(currentToken);

        if(currentToken.getStatus() == Token::rParren)
        {
            while(operator_stack.top().getStatus() != Token::lParren)
            {
                command_queue.push(operator_stack.top());
                operator_stack.pop();
            }

            assert(!operator_stack.empty());
            operator_stack.pop();  
        }
    }


    while(!operator_stack.empty())
    {
        command_queue.push(operator_stack.top());
        operator_stack.pop();
    }

}

queue<Token> shuntingYard::getReversePolish() {
       //	queue<Token> temp = command_queue;
//	while(!temp.empty()){
//		cout << temp.front().toString() << endl;
//		temp.pop();
//	}
	return command_queue; 
}
