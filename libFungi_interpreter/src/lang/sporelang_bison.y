%define parse.error verbose
%require "3.7"
// %language "c++"

%code requires{
    #include "libFungi_interpreter/src/lang/sporelang_syntaxtree.h"
}
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <libAlgae_dsp/dsp.h>
#include "libFungi_interpreter/src/lang/sporelang_syntaxtree.h"

ASTNode* syntaxTreeRoot;
extern int yylex();
extern int yyparse();
extern FILE *yyin;
void yyerror(std::string s) {
  // fprintf(stderr, "%s\n", s);
}

%}

%union {
  int             INTEGER_NUM;
  float           FLOAT_NUM;
  const char*          STRING;
  struct ASTNode* NODE;
  struct StatementsNode* STATEMENTS_NODE;
}

%type <STATEMENTS_NODE> statements
%type <NODE> statement
%type <NODE> expression

%token <INTEGER_NUM> INTEGER
%token <FLOAT_NUM> FLOAT
%token <STRING> IDENTIFIER INVALID_IDENTIFIER

%token IN IF ELSE LAMBDA_BEGIN ASSIGN 
%token OPEN_PAREN CLOSE_PAREN OPEN_BRACKET CLOSE_BRACKET OPEN_BRACE CLOSE_BRACE QUOTE
%token ADD_OP SUBTRACT_OP MULT_OP DIV_OP POWER_OP LEFT_OP RIGHT_OP
%token IS_EQUAL_OP IS_NOT_EQUAL_OP IS_GT_OP IS_LT_OP IS_GT_OR_EQUAL_OP IS_LT_OR_EQUAL_OP
%token AND_OP OR_OP XOR_OP 
%token NOT_OP TILDE COLON SWITCH_OP TERNARY_OP ASSOCIATE_OP COMPOSE_OP
%token RIGHT_ARROW LEFT_ARROW
%token STATEMENT_END

%precedence ASSIGNMENT
%right NOT_EQUALITY IS_NOT_EQUAL_OP
%right IS_GT_OR_EQUAL_OP IS_LT_OR_EQUAL_OP IS_LT_OP IS_GT_OP
%right ADD_OP SUBTRACT_OP
%right MULT_OP DIV_OP POWER_OP
%right ASSOCIATE_OP
%left  AND_OP OR_OP XOR_OP 
%left  NOT_OP
%left  COMPOSE_OP
%precedence NEG


%%

start: statements                               {syntaxTreeRoot = new RootNode($1);}

statements:
    statements statement                        { $$ = new StatementsNode($1, $2); }
    | %empty                                    { $$ = nullptr; }

statement:
    IF OPEN_PAREN expression CLOSE_PAREN OPEN_BRACE statements CLOSE_BRACE ELSE OPEN_BRACE statements CLOSE_BRACE
                                                { $$ = new IfElseNode($3, $6, $10); }
    | IF OPEN_PAREN expression CLOSE_PAREN OPEN_BRACE statements CLOSE_BRACE
                                                { $$ = new IfNode($3, $6); }
    | expression STATEMENT_END                  { $$ = $1; }

expression:
      SUBTRACT_OP expression %prec NEG          { $$ = new SubtractOperatorNode(new IntegerNode(0), $2); }
    | NOT_OP expression                         { $$ = new NotOperatorNode($2); }
    | IDENTIFIER ASSIGNMENT expression          { $$ = new AssignmentNode(new IdentifierNode($1), $3); }
    | expression LAMBDA_BEGIN expression        { $$ = new LambdaNode($1, $3); }
    | expression ASSOCIATE_OP expression STATEMENT_END        { $$ = new AssociateNode($1,$3); }
    | expression COMPOSE_OP IDENTIFIER       { $$ = new CompositionNode($1, new IdentifierNode($3)); }
    | expression IS_EQUAL_OP expression         { $$ = new IsEqualOperatorNode($1, $3); }
    | expression IS_GT_OR_EQUAL_OP expression   { $$ = new IsGreaterThanOrEqualOperatorNode($1, $3); }
    | expression IS_GT_OP expression            { $$ = new IsGreaterThanNode($1, $3); }
    | expression IS_NOT_EQUAL_OP expression     { $$ = new IsNotEqualOperatorNode($1, $3); }
    | expression IS_LT_OR_EQUAL_OP expression   { $$ = new IsLessThanOrEqualOperatorNode($1, $3); }
    | expression IS_LT_OP expression            { $$ = new IsLessThanOperatorNode($1, $3); }
    | expression DIV_OP expression              { $$ = new DivideOperatorNode($1, $3); }
    | expression MULT_OP expression             { $$ = new MultiplyOperatorNode($1, $3); }
    | expression ADD_OP expression              { $$ = new AddOperatorNode($1, $3); }
    | expression SUBTRACT_OP expression         { $$ = new SubtractOperatorNode($1, $3); }
    | expression AND_OP expression         { $$ = new AndOperatorNode($1, $3); }
    | expression OR_OP expression         { $$ = new OrOperatorNode($1, $3); }
    | expression XOR_OP expression         { $$ = new XorOperatorNode($1, $3); }
    | OPEN_PAREN expression CLOSE_PAREN         { $$ = $2; }
    | QUOTE IDENTIFIER QUOTE         { $$ = new StringNode($2); }
    | IDENTIFIER                                { $$ = new IdentifierNode($1); }
    | INTEGER                                   { $$ = new IntegerNode($1); }
    | FLOAT                                     { $$ = new FloatNode($1); }


%%
