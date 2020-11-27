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
#include <vector>
#include <libAlgae_dsp/dsp.h>
#include "libFungi_interpreter/src/lang/sporelang_syntaxtree.h"
#include <iostream>

RootNode* syntaxTreeRoot;
extern int yylex();
extern int yyparse();
extern FILE *yyin;
void yyerror(std::string s) {
  // fprintf(stderr, "%s\n", s);
  std::cout<<s<<"\n";
}
extern void scan_string(const char* str);
// extern yy_buffer_state;
// typedef yy_buffer_state *YY_BUFFER_STATE;
// extern int yyparse();
// extern YY_BUFFER_STATE yy_scan_buffer(char *, size_t);

%}

%debug

%union {
  int                    INTEGER_NUM;
  float                  FLOAT_NUM;
  const char*            STRING;
  struct ASTNode*        NODE;
  struct IdentifierNode*        ID_NODE;
  struct StatementsNode* STATEMENTS_NODE;
  struct ArgsNode* ARGS_NODE;
  struct ExpressionsNode* EXPRESSIONS_NODE;
}


%token <INTEGER_NUM> INTEGER
%token <FLOAT_NUM> FLOAT
%token <STRING> IDENTIFIER

%type <STATEMENTS_NODE> statements
%type <NODE> statement
%type <EXPRESSIONS_NODE> expressions
%type <NODE> expression
%type <ARGS_NODE> args
%type <ID_NODE> name


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
%left  OPEN_PAREN COMPOSE_OP 
%precedence NEG 




%%

start: statements                               { std::cout<<"ROOT\n";syntaxTreeRoot = new RootNode($1);}



statements:
    statements statement                        { std::cout<<"statements\n";$$ = new StatementsNode($1, $2); }
    | %empty                                    { std::cout<<"empty statement\n";$$ = nullptr; }

statement:
    IF OPEN_PAREN expression CLOSE_PAREN OPEN_BRACE statements CLOSE_BRACE ELSE OPEN_BRACE statements CLOSE_BRACE STATEMENT_END
                                                { std::cout<<"if else\n";$$ = new IfElseNode($3, $6, $10); }
    | IF OPEN_PAREN expression CLOSE_PAREN OPEN_BRACE statements CLOSE_BRACE STATEMENT_END
                                                { std::cout<<"if\n";$$ = new IfNode($3, $6); }
    | name ASSIGNMENT expression[stuff] STATEMENT_END    
                                                { std::cout<<"assignment of "<<$1->value<<"|"<<$stuff<<"\n";$$ = new AssignmentNode($1->value, $stuff); }
    | name OPEN_PAREN expressions[stuff] CLOSE_PAREN STATEMENT_END
                                                { std::cout<<"func call for "<<$1->value<<"|"<<$stuff<<"\n";$$ = new FunctionCallNode($1->value,$stuff);}
    | expression STATEMENT_END                  { std::cout<<"expression\n";$$ = new StatementsNode($1); }

name: IDENTIFIER {std::cout<<"name is..."<<$1<<"\n";$$ = new IdentifierNode($1);}

expressions:
    expressions expression                      {std::cout<<"expr list\n";$$ = new ExpressionsNode($1, $2);}
    | %empty                                    {std::cout<<"empty expr list\n";$$ = nullptr;}

args:
    args IDENTIFIER                             {std::cout<<"args\n";$$ = new ArgsNode($1, new IdentifierNode($2));}
    | IDENTIFIER                                {std::cout<<"one args\n";$$ = new ArgsNode( new IdentifierNode($1));}

expression:
      SUBTRACT_OP expression %prec NEG          { $$ = new SubtractOperatorNode(new IntegerNode(0), $2); }
    | NOT_OP expression                         { $$ = new NotOperatorNode($2); }
    | expression ASSOCIATE_OP expression  
                                                { $$ = new AssociateNode($1,$3); }
    | expression COMPOSE_OP expression          { $$ = new CompositionNode($1, $3); }
    | OPEN_PAREN args CLOSE_PAREN LAMBDA_BEGIN OPEN_BRACKET statements CLOSE_BRACKET STATEMENT_END      
                                                { std::cout<<"lambda def\n";$$ = new StatementsNode(new LambdaNode($2, new BlockNode($6))); }
    | expression IS_EQUAL_OP expression         { $$ = new IsEqualOperatorNode($1, $3); }
    | expression IS_GT_OR_EQUAL_OP expression   { $$ = new IsGreaterThanOrEqualOperatorNode($1, $3); }
    | expression IS_GT_OP expression            { $$ = new IsGreaterThanNode($1, $3); }
    | expression IS_NOT_EQUAL_OP expression     { $$ = new IsNotEqualOperatorNode($1, $3); }
    | expression IS_LT_OR_EQUAL_OP expression   { $$ = new IsLessThanOrEqualOperatorNode($1, $3); }
    | expression IS_LT_OP expression            { $$ = new IsLessThanOperatorNode($1, $3); }
    | expression DIV_OP expression              { $$ = new DivideOperatorNode($1, $3); }
    | expression MULT_OP expression             { std::cout<<"mult\n";$$ = new MultiplyOperatorNode($1, $3); }
    | expression ADD_OP expression              { $$ = new AddOperatorNode($1, $3); }
    | expression SUBTRACT_OP expression         { $$ = new SubtractOperatorNode($1, $3); }
    | expression AND_OP expression              { $$ = new AndOperatorNode($1, $3); }
    | expression OR_OP expression               { $$ = new OrOperatorNode($1, $3); }
    | expression XOR_OP expression              { $$ = new XorOperatorNode($1, $3); }
    | OPEN_PAREN expression CLOSE_PAREN         { $$ = $2; }
    | QUOTE name QUOTE                          { $$ = new StringNode($2->value); }
    | name                                      { std::cout<<"ID\n";$$ = $1; }
    | INTEGER                                   { $$ = new IntegerNode($1); }
    | FLOAT                                     { $$ = new FloatNode($1); }


  

%%

