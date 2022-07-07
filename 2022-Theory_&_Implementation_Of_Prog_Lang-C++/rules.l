/*********************************************************************            
  Programming Environment: wsl ubuntu
  Purpose of File: Contains the lexical analysis rules for 
  regular expressions for Part 1 and Part 2 of Class Project
**********************************************************************/
%option yylineno
%option noyywrap
%{
#include "lexer.h"
int line_number = 1;
%}
 // definitions
digit					[0-9]
letter				[A-Z]
anychar				[^'\n\r]
identifier				{letter}{1}({letter}|{digit}){0,7}
string_literal			[']({anychar}{0,80})[']
integer_literal			{digit}+
floating_point_literal		{digit}+"."{digit}+

 // exceptions
anomaly				{not_identifier}|{not_string_literal}
not_identifier			{letter}{1}({letter}|{digit}){8,}
not_string_literal		[']({anychar}{81,})[']
%%

 /* Rules */

 /* line count */

[\n\r]				{ line_number++; }

[ \t]+				/* eat up white space */

 /* Keywords */

CONTINUE				{ return TOK_CONTINUE; }

PROGRAM				{ return TOK_PROGRAM; }

DOWNTO				{ return TOK_DOWNTO; }

BEGIN					{ return TOK_BEGIN; }

BREAK					{ return TOK_BREAK; }

WHILE					{ return TOK_WHILE; }

WRITE					{ return TOK_WRITE; }

READ					{ return TOK_READ; }

THEN					{ return TOK_THEN; }

ELSE					{ return TOK_ELSE; }

VAR					{ return TOK_VAR; }

END					{ return TOK_END; }

FOR					{ return TOK_FOR; }

LET					{ return TOK_LET; }

IF					{ return TOK_IF; }

TO					{ return TOK_TO; }

 /* Datatype Specifiers */

"INTEGER"				{ return TOK_INTEGER; }

"REAL"				{ return TOK_REAL; }

 /* Punctuations */

";"					{ return TOK_SEMICOLON; }

":"					{ return TOK_COLON; }

"("					{ return TOK_OPENPAREN; }

")"					{ return TOK_CLOSEPAREN; }

"{"					{ return TOK_OPENBRACE; }

"}"					{ return TOK_CLOSEBRACE; }

 /* Operators */

"+"					{ return TOK_PLUS; }

"-"					{ return TOK_MINUS; }

"*"					{ return TOK_MULTIPLY; }

"/"					{ return TOK_DIVIDE; }

":="					{ return TOK_ASSIGN; }

"="					{ return TOK_EQUALTO; }

"<"					{ return TOK_LESSTHAN; }

">"					{ return TOK_GREATERTHAN; }

"<>"					{ return TOK_NOTEQUALTO; }

"MOD"					{ return TOK_MOD; }

"NOT"					{ return TOK_NOT; }

"OR"					{ return TOK_OR; }

"AND"					{ return TOK_AND; }

 /* Useful Abstractions */

{floating_point_literal}	{ return TOK_FLOATLIT; }

{integer_literal}			{ return TOK_INTLIT; }

{string_literal}			{ return TOK_STRINGLIT; }

{identifier}			{ return TOK_IDENT; }

{anomaly}				{ return TOK_UNKNOWN; }

 /* Found an unknown character */

.					{ return TOK_UNKNOWN; }

 /* Recognize end of file */

<<EOF>>				{ return TOK_EOF; }

%%

