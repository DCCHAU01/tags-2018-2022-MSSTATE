/*********************************************************************
  Programming Environment: wsl ubuntu
  Purpose of File: contains production parsing functions
**********************************************************************/

#ifndef PRODUCTIONS_H
#define PRODUCTIONS_H

#include <iostream>
#include "parse_tree_nodes.h"

extern map<string, int> symbolTable;
int nextToken = 0;              // token returned from yylex
int level = 0;                  // used to indent output to approximate parse tree
bool visible = false;           //used to toggle console display debug

extern "C"
{
	// Instantiate global variables used by flex
	extern char* yytext;        // text of current lexeme
	extern int   yylex();       // the generated lexical analyzer
}

// Forward declarations of production parsing functions
ProgramNode* program();
BlockNode* block();
StatementNode* statement();
AssignmentNode* assignment();
CompoundNode* compound();
NestedCompoundNode* nested_compound();
IfNode* iff();
WhileNode* whilee();
ReadNode* read();
WriteNode* write();
ExpressionNode* expression();
SimpleExpressionNode* simple_expression();
TermNode* term();
FactorNode* factor();

// Forward declarations of functions that check whether the current token is
// in the first set of a production rule
// or
// checks for other conditions
bool isit(int sometoken, int YN);
bool first_of_expression();
bool first_of_statement();
bool termcond();
bool simple_expressioncond();
bool expressioncond();
void varloop();
void termloop(TermNode* a);
void simple_expressionloop(SimpleExpressionNode* a);
//*****************************************************************************
void dis() {//debug tool
    cout << yytext << "--" << nextToken << endl;
}
//*****************************************************************************
// Get the next lexeme (word in sentence)
int lex() {
    nextToken = yylex();
    if (visible == true) {
        dis();
    }
    return nextToken;
}
//*****************************************************************************
// prints indent based on level
inline void indent(){
    for (int i = 0; i<level; i++)
        cout << ("    ");
}
//*****************************************************************************
// prints output : used when a match is encountered
void output(){
    indent();
    cout << "-->found " << yytext << endl;
}
//*****************************************************************************
//<program> -> TOK_PROGRAM TOK_IDENT TOK_SEMICOLON <block>
//First Token Set: { TOK_PROGRAM }
//the first function to run when parsing, the root of the parsing tree
ProgramNode* program() {

    if (isit(TOK_PROGRAM, 2) == false) {            //if first word is not program, throw
        throw("3: \'PROGRAM\' expected");   
    }

    indent();                                       //print indent
    cout << "enter <program>" << endl;      
    ++level;                                        //increase indent size

    output();                                       //since first is PROGRAM, output PROGRAM
    lex();                                          //get next word
    isit(TOK_IDENT, 1);                             //if word is identifier, output word, else throw 
    ProgramNode* newProgramNode = new ProgramNode(string(yytext));  //declare a new object 
    lex();                                          //get next word
    isit(TOK_SEMICOLON, 1);                         //if word is semicolon, output word, else throw 
    lex();                                          //get next word
    newProgramNode->startBlock = block();           //enter block branch                             

    --level;                                        //decrease indent size
    indent();                                       //print indent
    cout << "exit <program>" << endl;
    while (nextToken != TOK_EOF) {                  //assuming all words have been used, the next word must be End-of-File
        lex();                              
    }                                               //at this point, we exit the root and go back to main
    return newProgramNode;
}
//*****************************************************************************
//<block> -> [ TOK_VAR TOK_IDENT TOK_COLON (TOK_INTEGER | TOK_REAL) TOK_SEMICOLON 
//             { TOK_IDENT TOK_COLON (TOK_INTEGER | TOK_REAL) TOK_SEMICOLON } ]
//             <compound>
//First Token Set: { TOK_VAR, TOK_BEGIN }
//enter block branch from program root
//the [] after -> mean there are 0 or 1 variable list.  There is a minimum of 1 variable in the list
//the {} mean there may be more than 1 variable in the variable list
BlockNode* block() {
    //if word is not variable or a begin, throw
    if ((isit(TOK_VAR, 2) == false) && (isit(TOK_BEGIN, 2) == false)) {
        throw("18: error in declartion part OR 17: \'BEGIN\' expected");
    }

    indent();
    cout << "enter <block>\n";
    ++level;

    if (isit(TOK_VAR,0) == true) {          //if word is a variable, output word and continue, else skip if statement
        lex();                              //get next word,
        varloop();                          //iterate over and parse variable list
    }                                       
    BlockNode* newBlockNode = new BlockNode();
    newBlockNode->startCompound = compound();                             //enter compound sub branch

    --level;
    indent();
    cout << "exit <block>\n";               //exit block branch and return to program root
    return newBlockNode;
}
//parses a list of words, assumed to be variables
void varloop() {
    isit(TOK_IDENT, 3);                     //if word is identifier and not a duplicate variable then output word, else throw
    symbolTable.insert(pair<string, int>(yytext, 0));             //insert word into symbolTable, initialize variable with 0
    lex();                                  //get next word
    isit(TOK_COLON, 1);                     //if word is colon, outputd wor, else throw 
    lex();                                  //get next word
    //if word is not an integer or real, then throw
    if ((isit(TOK_INTEGER, 0) == false) && (isit(TOK_REAL, 0) == false)) {
        throw("10: error in type");
    }
    lex();                                  //get next word
    isit(TOK_SEMICOLON, 1);                 //if word is semicolon, output word, else throw 
    lex();                                  //get next word
    cout << "\n";
    if (nextToken != TOK_BEGIN) {           //begin signifies end of variable list
        varloop();                          //if word is not begin, then there might be more to the list to parse
    }
    else {
        return;                             //exit parsing of variable list
    }
}
//*****************************************************************************
//<statement> -> <assignment> | <compound> | <if> | <while> | <read> | <write>
//First Token Set: { TOK_IDENT, TOK_BEGIN, TOK_IF, TOK_WHILE, TOK_READ, TOK_WRITE }
//enter statement branch from compound, iff, or whilee branch
//the | means logical or
StatementNode* statement() {
    //if the word is not in First Token Set, throw
    if (first_of_statement() == false) {
        throw("900: illegal type of statement");
    }
    
    indent();
    cout << "enter <statement>\n";
    ++level;

    StatementNode* newStatementNode = nullptr;

    if (nextToken == TOK_IDENT)                 //enter assignment sub branch if word is identifier
        newStatementNode = assignment();
    if (nextToken == TOK_BEGIN)                 //enter compound sub branch if word is BEGIN
        newStatementNode = nested_compound();
    if (nextToken == TOK_READ)                  //enter READ sub branch if token is READ
        newStatementNode = read();
    if (nextToken == TOK_WRITE)                 //enter WRITE sub branch if token is WRITE
        newStatementNode = write();
    if (nextToken == TOK_IF)                    //enter IF sub branch if token is IF
        newStatementNode = iff();
    if (nextToken == TOK_WHILE)                 //enter WHILE sub branch if token is WHILE
        newStatementNode = whilee();

    --level;
    indent();
    cout << "exit <statement>\n";
    return newStatementNode;                    //exit statement branch and return to the parent branch that called it
}
//check to see if word is in First Token Set of statement
bool first_of_statement() {
    if (isit(TOK_IDENT, 2) == true)     return true;
    if (isit(TOK_BEGIN, 2) == true)     return true;
    if (isit(TOK_IF, 2) == true)        return true;
    if (isit(TOK_WHILE, 2) == true)     return true;
    if (isit(TOK_READ, 2) == true)      return true;
    if (isit(TOK_WRITE, 2) == true)     return true;
    /*          else             */     return false;
}
//*****************************************************************************
//<assignment> -> TOK_IDENT TOK_ASSIGN <expression>
//First Token Set: { TOK_IDENT }
//enter assignment branch from statement branch
AssignmentNode* assignment() {

    indent();
    cout << "enter <assignment>\n";
    ++level;

    //AssignmentNode* newAssignmentNode = new AssignmentNode(string(yytext));
        
    isit(TOK_IDENT, 4);                     //check if word is IDENT and if it is in symbolTable
    AssignmentNode* newAssignmentNode = new AssignmentNode(string(yytext));
    lex();                                  //get next word,
    isit(TOK_ASSIGN, 1);                    //check if word is ASSIGN, output if true, throw if false
    lex();                                  //get next word, 
    newAssignmentNode->startExpression = expression();                           
    
    --level;
    indent();
    cout << "exit <assignment>\n";          //exit assignment branch
    return newAssignmentNode;
}
//*****************************************************************************
//<compound> -> TOK_BEGIN <statement> { TOK_SEMICOLON <statement> } TOK_END
//First Token Set: { TOK_BEGIN }
//enter compound branch from statement branch or block branch
//BEGIN <statement> (0 or more SEMICOLON <statement) END
CompoundNode* compound() {

    indent();
    cout << "enter <compound_statement>\n";
    ++level;

    isit(TOK_BEGIN, 1);                             //if word is BEGIN, output word, else throw
    CompoundNode* newCompoundNode = new CompoundNode();
    lex();                                          //get next word
    newCompoundNode->startStatement = statement();  //enter statement sub branch

    while (nextToken != TOK_END) {                  //loop until end of compound is encountered, aka a word that is END
        if (nextToken == TOK_SEMICOLON) {           //if word is semicolon
            output();                               //output word
            lex();                                  //get next word
            newCompoundNode->restStatements.push_back(statement());
        }
        else {
            if (nextToken == TOK_END) {             //if word is END
                output();                           //output word
                lex();                              //get next word
                --level;
                indent();
                cout << "exit <compound_statement>\n";
                return newCompoundNode;             //exit compound branch
            }
            lex();                                  //get next word.  this part may cause a bug, check later if it does
            if (nextToken != TOK_SEMICOLON) {       //if word is not semicolon
                if (nextToken != TOK_END) {         //of word is not END
                    throw("13: \'END\' expected");  //throw
                }
            }
        }
    }
    isit(TOK_END, 1);                               //if word is END, output word, else throw
    lex();                                          //get next word

    --level;
    indent();
    cout << "exit <compound_statement>\n";          //exit compound branch
    return newCompoundNode;
}

NestedCompoundNode* nested_compound() {
    indent();
    cout << "enter <compound_statement>\n";
    ++level;

    isit(TOK_BEGIN, 1);                             //if word is BEGIN, output word, else throw
    NestedCompoundNode* newNestedCompoundNode = new NestedCompoundNode();
    lex();                                          //get next word
    newNestedCompoundNode->startStatement = statement();  //enter statement sub branch

    while (nextToken != TOK_END) {                  //loop until end of compound is encountered, aka a word that is END
        if (nextToken == TOK_SEMICOLON) {           //if word is semicolon
            output();                               //output word
            lex();                                  //get next word
            newNestedCompoundNode->restStatements.push_back(statement());
        }
        else {
            if (nextToken == TOK_END) {             //if word is END
                output();                           //output word
                lex();                              //get next word
                --level;
                indent();
                cout << "exit <compound_statement>\n";
                return newNestedCompoundNode;             //exit compound branch
            }
            lex();                                  //get next word.  this part may cause a bug, check later if it does
            if (nextToken != TOK_SEMICOLON) {       //if word is not semicolon
                if (nextToken != TOK_END) {         //of word is not END
                    throw("13: \'END\' expected");  //throw
                }
            }
        }
    }
    isit(TOK_END, 1);                               //if word is END, output word, else throw
    lex();                                          //get next word

    --level;
    indent();
    cout << "exit <compound_statement>\n";          //exit compound branch
    return newNestedCompoundNode;
}
//*****************************************************************************
//<if> -> TOK_IF <expression> TOK_THEN <statement> [ TOK_ELSE <statement> ]
//First Token Set: { TOK_IF }
//IF <expression> THEN <statement> (0 or 1 ELSE <statement)
//enter iff branch from statement branch
IfNode* iff() {
    
    indent();
    cout << "enter <if statement>\n";
    ++level;

    isit(TOK_IF, 1);                        //if word is IF, output word, else throw
    IfNode* newIfNode = new IfNode();
    lex();                                  //get next word
    newIfNode->startExpression = expression();
    isit(TOK_THEN, 1);                      //if word is THEN, output word, else throw
    lex();                                  //get next word

    newIfNode->startStatement = statement();

    if (nextToken == TOK_END) {             //if word is END, exit iff branch
        --level;                            //assumes that statement entered compound
        indent();                           //and compound ends with END
        cout << "exit <if statement>\n";    
        return newIfNode;
    }
    if (nextToken != TOK_ELSE) {            //if word is not ELSE
        lex();                              //get next word.  current position is the last word of statement.  
    }                                       //next word is either ELSE or a word that indicates we must exit this branch
    if (nextToken == TOK_ELSE) {            //if word is ELSE
        output();                           //output word
        lex();                              //get next word
        newIfNode->restStatements.push_back(statement()); //enter statement sub branch
    }

    --level;
    indent();
    cout << "exit <if statement>\n";        //exit iff branch
    return newIfNode;
}
//*****************************************************************************
//<while> -> TOK_WHILE <expression> <statement>
//First Token Set: { TOK_WHILE }
//enter whilee branch from statement branch
WhileNode* whilee() {
    indent();
    cout << "enter <while statement>\n";
    ++level;

    isit(TOK_WHILE, 1);                     //if word is WHILE, output word, else throw
    WhileNode* newWhileNode = new WhileNode();
    lex();                                  //get next word
    newWhileNode->startExpression = expression();
    
    newWhileNode->startStatement = statement(); //enter statement sub branch

    --level;
    indent();
    cout << "exit <while statement>\n";     //exit whilee branch
    return newWhileNode;
}
//*****************************************************************************
//<read> -> TOK_READ TOK_OPENPAREN TOK_IDENT TOK_CLOSEPAREN
//First Token Set: { TOK_READ }
//enter read branch from statement branch
ReadNode* read() {
    indent();
    cout << "enter <read>\n";
    ++level;
    
    isit(TOK_READ, 1);          //if word is READ, output word, else throw
    lex();                      //get next word
    isit(TOK_OPENPAREN, 1);     //if word is (, output word, else throw
    lex();                      //get next word
    isit(TOK_IDENT, 1);         //if word is identifier, output word, else throw
    ReadNode* newReadNode = new ReadNode(string(yytext));
    lex();                      //get next word
    isit(TOK_CLOSEPAREN, 1);    //if word is ), output word, else throw

    --level;
    indent();
    cout << "exit <read>\n";    //exit read branch
    return newReadNode;
}
//*****************************************************************************
//<write> -> TOK_WRITE TOK_OPENPAREN (TOK_IDENT | TOK_STRINGLIT) TOK_CLOSEPAREN
//First Token Set: { TOK_WRITE }
//WRITE ( identifer or string literal )
//enter write branch from statement branch
WriteNode* write() {
    indent();
    cout << "enter <write>\n";
    ++level;
    
    isit(TOK_WRITE, 1);                     //if word is WRITE, output word, else throw
    lex();                                  //get next word
    isit(TOK_OPENPAREN, 1);                 //if word is (, output word, else throw
    lex();                                  //get next word
    if (nextToken == TOK_IDENT)        {               //if word is identifier, output word
        output(); 
        WriteNode* newWriteNode = new WriteNode(string(yytext), false);
        lex();                                  //get next word
        isit(TOK_CLOSEPAREN, 1);                //if word is ), output word, else throw

        --level;
        indent();
        cout << "exit <write>\n";               //exit write branch
        return newWriteNode;
    }
    else if (nextToken == TOK_STRINGLIT)    {               //if word is string literal, output word
        output(); 
        WriteNode* newWriteNode = new WriteNode(string(yytext), true);
        lex();                                  //get next word
        isit(TOK_CLOSEPAREN, 1);                //if word is ), output word, else throw

        --level;
        indent();
        cout << "exit <write>\n";               //exit write branch
        return newWriteNode;
    }
    else { throw("134: illegal type of operand(s)"); }   //else throw
}
//*****************************************************************************
//<expression> -> <simple expression> 
//                [ ( TOK_EQUALTO | TOK_LESSTHAN | TOK_GREATERTHAN | TOK_NOTEQUALTO )
//                  <simple expression> ]
//First Token Set: { TOK_INTLIT, TOK_FLOATLIT, TOK_IDENT, TOK_OPENPAREN, TOK_NOT, TOK_MINUS }
//<simple expression> (0 or 1 math symbol <simple expression>)
//enter expression branch from assignment, iff, whilee, or factor branch
ExpressionNode* expression() {
    //if word is not in First Token Set, throw
    if (first_of_expression() == false) {
        throw("144: illegal type of expression");
    }

    indent();
    cout << "enter <expression>\n";
    ++level;
    
    ExpressionNode* newExpressionNode = new ExpressionNode();
    newExpressionNode->firstChild = simple_expression();                //enter simple expression sub branch
    
    if (expressioncond() == false) {    //if word is not an accepted math symbol, exit expression branch
        --level;
        indent();
        cout << "exit <expression>\n";
        return newExpressionNode;
    }
    if (expressioncond() == true) {     //if word is an accepted math symbol, continue
        output();                       //output word
        newExpressionNode->op = new string(yytext);
        lex();                          //get next word
        newExpressionNode->secondChild = simple_expression();            //enter simple expression sub branch
    }

    --level;
    indent();
    cout << "exit <expression>\n";      //exit expression branch
    return newExpressionNode;
}
//check to see if word is a math symbol in expression
bool expressioncond() {
    if (nextToken == TOK_NOTEQUALTO)    return true;
    if (nextToken == TOK_EQUALTO)       return true;
    if (nextToken == TOK_LESSTHAN)      return true;
    if (nextToken == TOK_GREATERTHAN)   return true;
    /*          else             */     return false;
}
//check to see if word is in First Token Set of expression
bool first_of_expression() {
    if (isit(TOK_INTLIT, 2) == true)     return true;
    if (isit(TOK_FLOATLIT, 2) == true)   return true;
    if (isit(TOK_IDENT, 2) == true)      return true;
    if (isit(TOK_OPENPAREN, 2) == true)  return true;
    if (isit(TOK_NOT, 2) == true)        return true;
    if (isit(TOK_MINUS, 2) == true)      return true;
    /*          else             */      return false;
}
//*****************************************************************************
//<simple expression> -> <term> { ( TOK_PLUS | TOK_MINUS | TOK_OR ) <term> }
//First Token Set: { TOK_INTLIT, TOK_FLOATLIT, TOK_IDENT, TOK_OPENPAREN, TOK_NOT, TOK_MINUS }
//<term> (0 or more math symbols <term>)
//enter simple expression branch from expression branch
SimpleExpressionNode* simple_expression() {
    //if word is not in First Token Set of simple expression, throw
    if (first_of_expression() == false) {
        throw("901: illegal type of simple expression");
    }

    indent();
    cout << "enter <simple expression>\n";
    ++level;
    SimpleExpressionNode* newSimpleExpressionNode = new SimpleExpressionNode();
    newSimpleExpressionNode->startTerm = term();

    if (simple_expressioncond() == false) {     //if word is is not a math symbol in simple expression
        --level;                                //exit simple expression branch
        indent();
        cout << "exit <simple expression>\n";
        return newSimpleExpressionNode;
    }
    if (simple_expressioncond() == true) {      //if word is a math symbol in simple expression branch
        simple_expressionloop(newSimpleExpressionNode);                //iterate over and parse multiple math symbol + term
    }


    --level;
    indent();
    cout << "exit <simple expression>\n";       //exit simple expression branch
    return newSimpleExpressionNode;
}
//check to see if word is a math symbol in simple expression
bool simple_expressioncond() {
    if (nextToken == TOK_PLUS)          return true;
    if (nextToken == TOK_MINUS)         return true;
    if (nextToken == TOK_OR)            return true;
    /*          else             */     return false;
}
//parses 1 or more (math symbol + term)
void simple_expressionloop(SimpleExpressionNode* a) {
    if (visible == true) {
        cout << "enter simple_expressionloop\n";
    }
    //if word is a math symbol in simple expression, continue
    if ((nextToken == TOK_PLUS) || (nextToken == TOK_MINUS) || (nextToken == TOK_OR)) {
        output();                                   //output math symbol
        string* b = new string(yytext);
        a->ops.push_back(b);
        lex();                                      //get next word
        a->restTerms.push_back(term());                                  //enter term sub branch
        if (simple_expressioncond() == true) {      //if another math symbol is found
            lex();                                  //get next word
            simple_expressionloop(a);                //recursively call itself
        }
    }
    if (visible == true) {
        cout << "exit simple_expressionloop\n";
    }                                               //exit simple_expressionloop 
}
//*****************************************************************************
//<term> -> <factor> { ( TOK_MULTIPLY | TOK_DIVIDE | TOK_AND ) <factor> }
//First Token Set: { TOK_INTLIT, TOK_FLOATLIT, TOK_IDENT, TOK_OPENPAREN, TOK_NOT, TOK_MINUS }
//<factor> (or or more math symbol <factor>)
//enter term branch from simple expression branch
TermNode* term() {
    //if word is not in First Token Set of term, throw
    if (first_of_expression() == false) {
        throw("902: illegal type of term");
    } 

    indent();
    cout << "enter <term>\n";
    ++level;
    TermNode* newTermNode = new TermNode();
    newTermNode->startFactor = factor();                   //enter factor sub branch
    lex();                      //get next word
    if (termcond() == true) {   //if word is a math symbol in term
        termloop(newTermNode);             //iterate over and parse multiple math symbol + factor
    }

    --level;
    indent();
    cout << "exit <term>\n";    //exit term branch
    return newTermNode;
}
//parses 1 or more (math symbol + factor)
void termloop(TermNode* a) {
    if (visible == true) {
        cout << "enter termloop\n";
    }
    if (termcond() == true) {       //if word is a math symbol in term
        output();                   //output math symbol
        string* b = new string(yytext);
        a->ops.push_back(b);
        lex();                      //get next word
        a->restFactors.push_back(factor());    //enter factor sub branch
        lex();                      //get next word
        termloop(a);                 //recursively call termloop
    }
    if (visible == true) {
        cout << "exit termloop\n";  
    }                               //exit termloop
}
//check to see if word is a math symbol in term
bool termcond() {
    if (nextToken == TOK_MULTIPLY)      return true;
    if (nextToken == TOK_DIVIDE)        return true;
    if (nextToken == TOK_AND)           return true;
    /*          else             */     return false;
}
//*****************************************************************************
//<factor> -> TOK_INTLIT | TOK_FLOATLIT | TOK_IDENT | 
//            (TOK_OPENPAREN <expression> TOK_CLOSEPAREN ) | 
//            ( TOK_NOT <factor> ) | ( TOK_MINUS <factor> )
//First Token Set: { TOK_INTLIT, TOK_FLOATLIT, TOK_IDENT, TOK_OPENPAREN, TOK_NOT, TOK_MINUS }
//the | means logical or
//enter factor branch from term or factor branch
FactorNode* factor() {
    //if word is not in First Token Set of factor, throw
    if (first_of_expression() == false) {
        throw("903: illegal type of factor");
    }
    
    indent();
    cout << "enter <factor>\n";
    ++level;
    
    FactorNode* a = nullptr;
    //newFactorNode = factorloop();                   //enter parsing function for factor

    bool found = false;     //used to see if identifier exists in variable list
    if (nextToken == TOK_MINUS) {
        output();                                                 //if word is -, output word, get next word, end enter factor sub branch
        string c = string(yytext);
        lex();
        a = new nestedFactorNode(factor(), c);
    }
    else if (nextToken == TOK_NOT) {
        output();                                                 //if word is NOT, output word, get next word, and enter factor sub branch
        string b = string(yytext);
        lex();
        a = new nestedFactorNode(factor(), b);
    }
    else if (nextToken == TOK_OPENPAREN) {
        output(); lex();                                          //if word is (, get next word, enter expression branch, and check if word is ), else throw
        a = new NestedExprNode(expression());
        isit(TOK_CLOSEPAREN, 1);
    }
    else if (nextToken == TOK_FLOATLIT) { output(); a = new TerminalNode(string(yytext)); }         //if word is a float literal, output word
    else if (nextToken == TOK_INTLIT) { output(); a = new TerminalNode(string(yytext)); }         //if word is an intger literal, output worde
    else if (nextToken == TOK_IDENT) {
       map<string, int>::iterator it;       //declare string iterator
        if (symbolTable.empty()) {      //if symbolTable is empty, throw
            throw("104: identifier not declared");
        }                               //check symbolTable to see if identifier is in it
        for (it = symbolTable.begin(); it != symbolTable.end(); ++it) { //
            if (yytext == it->first) {
                found = true;           //if a match is found, output word
                output();
                a = new TerminalNode(string(yytext));
            }
        }
        if (found == false) {           //throws if a match is not found
            throw("104: identifier not declared");
        }
    }
    else {                              //catches any other error
        throw("999: an error has occured");
    }

    --level;
    indent();
    cout << "exit <factor>\n";      //exit factor branch
    return a;
}
//*****************************************************************************
//YN = 0 : output() enabled, return true or false enabled
//YN = 1 : output() and throw enabled, return true or false enabled
//YN = 2 : return true or false enabled
//YN = 3 : check symbolTable for duplicate identifiers enabled
//YN = 4 : see if identifier is in symbolTable
//intuitively : is it an integer literal?
//Issue: could have used "#define" to use isit() more intuitively
bool isit(int sometoken, int YN) {
    if ((YN < 0) && (YN > 4)) {     //throws if an invalid YN as input
        throw("invalid YN in isit");
    }
    if (YN == 3) {
        map<string, int>::iterator it;       //declare string iterator
        if (nextToken == TOK_IDENT) {   //if word is an identifier, loop through symbolTable until a match is found
            for (it = symbolTable.begin(); it != symbolTable.end(); ++it) {
                if (yytext == it->first) {    //if a match is found, throw
                    throw("101: identifier declared twice");
                }
            }
            output();                   //if a match is not found, output word
            return true;                //return true to the function that called it
        }
    }
    if (YN == 4) {
        bool found = false;
        map<string, int>::iterator it;       //declare string iterator
        for (it = symbolTable.begin(); it != symbolTable.end(); ++it) {
            if (yytext == it->first) {
                found = true;           //see if identifier exists, if it doesn't throw
            }
        }
        if (found == false) {           //throws if a match is not found
            throw("104: identifier not declared");
        }
    }
    if (nextToken == sometoken) {       //if your inputted word matches the current word, continue
        if (YN != 2) {                  //output if YN is not 2
            output();
        }
        return true;                    //return true to the function that called it
    }
    if (YN == 1) {                      //if YN is 1 and inputted word does not match the current word, 
        switch (sometoken)              //look through list of cases for a match then throw
        {
        case TOK_IDENT:                 throw("2: identifier expected");
        case TOK_PROGRAM:               throw("3: \'PROGRAM\' expected");
        case TOK_CLOSEPAREN:            throw("\')\' expected");
        case TOK_COLON:                 throw("\':\' expected");
        case TOK_OPENPAREN:             throw("\'(\' expected");
        //case (TOK_INTEGER || TOK_REAL): throw("10: error in type");
        case TOK_END:                   throw("13: \'END\' expected");
        case TOK_SEMICOLON:             throw("14: \';\' expected");
        case TOK_BEGIN:                 throw("17: BEGIN expected");
        //18: error in declaration part OR 17: 'BEGIN' expected -- means <block> does not start with VAR or BEGIN
        case TOK_ASSIGN:                throw("\':=\' expected");
        case TOK_THEN:                  throw("\'THEN\' expected");
        //101: identifier declared twice
        //104: identifier not declared
        //134: illegal type of operand(s) -- WRITE does not contain an identifier or string 
        //144: illegal type of expression" -- illegal first of expression
        //900: illegal type of statement -- illegal first of statement
        //901: illegal type of simple expression -- illegal first of simple expression
        //902: illegal type of term -- illegal first of term
        //903: illegal type of factor -- illegal first of factor
        //999: an error has occured -- any other error
        default:                        throw("999: an error has occured"); //aka any other error
        }

    }
    return false;                       //if YN is not 1, then isit() will return false instead of throwing
}
#endif
