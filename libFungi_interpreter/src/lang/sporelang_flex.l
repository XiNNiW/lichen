%{

#include "sporelang_bison.hpp"

%}


INT [0-9]+
FLOAT [0-9]+\.[0-9]+
ID [A-Za-z][A-Za-z0-9_]*     
SPACE [ \t\r\n]
NEWLINE \n


%option noyywrap
%option always-interactive

%%
"//"	                     {}
"in"                         { return(IN);               }
"if"                         { return(IF);               }
"else"                       { return(ELSE);             }
                     
"=>"                         { return(LAMBDA_BEGIN);     }
"="                          { return(ASSIGNMENT);       }
                     
"??"                         { return(SWITCH_OP);        }
"?"                          { return(TERNARY_OP);       }
":"                          { return(COLON);            }
"$"                          { return(ASSOCIATE_OP);     }
"."                          { return(COMPOSE_OP);       }
"->"                         { return(RIGHT_ARROW);      }
"<-"                         { return(LEFT_ARROW);       }
"*"                          { return(MULT_OP);          }
"**"                         { return(POWER_OP);         }
"/"                          { return(DIV_OP);           }
"+"                          { return(ADD_OP);           }
"-"                          { return(SUBTRACT_OP);      }
"<<"                         { return(LEFT_OP);          }
">>"                         { return(RIGHT_OP);         }
"=="                         { return(IS_EQUAL_OP);      }
"!="                         { return(IS_NOT_EQUAL_OP);  }
">="                         { return(IS_GT_OR_EQUAL_OP);}
"<="                         { return(IS_LT_OR_EQUAL_OP);}
"<"                          { return(IS_LT_OP);         }
">"                          { return(IS_GT_OP);         }
"&"                          { return(AND_OP);           }
"|"                          { return(OR_OP);            }
"^"                          { return(XOR_OP);           }
"!"                          { return(NOT_OP);           }
"~"                          { return(TILDE);            }
"[\"']"                      { return(QUOTE);            }
"("                          { return(OPEN_PAREN);       }
")"                          { return(CLOSE_PAREN);      }
"["                          { return(OPEN_BRACKET);     }
"]"                          { return(CLOSE_BRACKET);    }
"{"                          { return(OPEN_BRACE);       }
"}"                          { return(CLOSE_BRACE);      }
{INT}                        { /*yyvalue.INTEGER_NUM = atoi(yytext);*/ return(INTEGER);}
{FLOAT}                      { /*yyvalue.FLOAT_NUM = atof(yytext);*/ return(FLOAT);}
{ID}                         { /*yyvalue.STRING = std::string(yytext);*/ return(IDENTIFIER);}
{NEWLINE}{SPACE}*{NEWLINE}   { return(STATEMENT_END);    }
{SPACE}                      {}

.                            {printf("fuk!");yyterminate();      }

%%

void scan_string(const char* str)
{
    yy_switch_to_buffer(yy_scan_string(str));
}