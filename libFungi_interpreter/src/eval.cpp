#include "eval.h"

ASTNode* syntaxTreeRoot;

int eval(std::string program){
    std::stringstream programStream(program);
    yyin = (&programStream);
    yyparse();
    return 0;
}