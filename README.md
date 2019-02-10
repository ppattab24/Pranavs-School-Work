# CS 100 Programming Project
 By Pranav Pattabiraman (SID#: 861052235) and Tinh La (SID#: 861199635)
# Introduction
This project’s goal is the design and creation of a command shell that is able to take in input, interprets it based off of different pieces of information embedded within that input such as: different commands, character flags, file pathnames, and strings.  We will be using the Composite design pattern to build and structure the necessary classes.

# Diagram
![UML Diagram](https://github.com/cs100/assignment-cs_100_tinh_and_pranav/blob/master/images/CS_100_Assignment1_UML_Diagram%20(1).png?raw=true)

# Classes
 * __Base(Manager)__
  So we need a Base class, through which any potential client or user would be interacting with the program through, and the Base class that would extend all its functionality to its children, as appropriate.  
 * __Parsing(Composition)__
  The purpose of this is to take the user input and parse it into component parts: command, flag, and connector to then be used in the token subclass.
 * __Token(Composite)__
  Token would have the role of parsing through that input and creating the appropriate sub-token classes 
 
 * __Command Token(Primitive)__
 This would handle all the functionality that pertains specifically to the command portion of the user input.
 
 * __Flag Token(Primitive)__
 This would handle all the functionality that pertains specifically to the argument portion of the user input that has been parsed and handed down to the token composite
 
 * __Connector(Primitive)__
  This would be the final primitive that handles the interaction between the commands and the order in which they are to take precedence. 
  
 # Research/Prototype
 
 *  __waitpid()__;
   Makes the system wait for the current process to change state before continuing
   
 *  __execvp()__;
   Executes an executable to be executed from the process you are currently in
   
 *  __fork()__;
   Makes a duplicate of current process.	
 
 # Development and Testing Roadmap
  
   For this project Tinh will be the primary coder for the Parsing class, specifically he will be the lead dev in creating and implementing the [state table](https://github.com/cs100/assignment-cs_100_tinh_and_pranav/issues/1) that will be used. Pranav will handle coding the [Token Class](https://github.com/cs100/assignment-cs_100_tinh_and_pranav/issues/2) and its associated subclass(i.e. the primatives). Both will collaborate on the [Manager class](https://github.com/cs100/assignment-cs_100_tinh_and_pranav/issues/3), in particular making sure the control flow for the entire program runs as intended. 
   Potential roadblocks for this assignment are as follows. While we both have an understanding of state tables, only Tinh has actually implemented a rudimentry state table and as such this will be a focus for us to figure out how to implement as it is central to making the assignment work. Also another potential issue we could run into is in making sure the order in which the parsing takes place then issuing those commands into the token and then into the correct primitive is the order that the user intended. Also dealing with the [order of operation](https://github.com/cs100/assignment-cs_100_tinh_and_pranav/issues/4) for the string check from the user input to make sure what is a complete command i.e. is it in parenthesis and or if it is preceded by a #. 
