#include "../header/shuntingYard.h"

ShuntingYard::ShuntingYard(queue<Token> commandQ)
{
    delivery = commandQ;
    run();
}

void ShuntingYard::run()
{
    while(!delivery.empty())    
    {
        Token currentToken = delivery.front();
        delivery.pop();

        if( currentToken.getStatus() == Token::middle || currentToken.getStatus() == Token::quotations
                || currentToken.isTest() )
            command_token_queue.push(currentToken);
        if(currentToken.getStatus() == Token::connector)
        {
            while(!opStack.empty() && opStack.top().getStatus() != Token::lParren)
            {
                command_token_queue.push(opStack.top());
                opStack.pop();
            }

            opStack.push(currentToken);
        }

        if(currentToken.getStatus() == Token::lParren)
            opStack.push(currentToken);

        if(currentToken.getStatus() == Token::rParren)
        {
            while(opStack.top().getStatus() != Token::lParren)
            {
                command_token_queue.push(opStack.top());
                opStack.pop();
            }

            assert(!opStack.empty());
            opStack.pop();  
        }
    }

    while(!opStack.empty())
    {
        command_token_queue.push(opStack.top());
        opStack.pop();
    }

}

queue<Token> ShuntingYard::getReversie() { return command_token_queue; }
