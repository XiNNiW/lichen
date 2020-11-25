#include "eval.h"

ErrorOrSignal eval(std::string program){
    std::stringstream programStream(program);
    yyin = (&programStream);
    if(yyparse()==0){
        SemanticAnalyzer analyzer = SemanticAnalyzer();
        FungiInterpreter interpreter = FungiInterpreter();
        analyzer.inspect(syntaxTreeRoot);
        return analyzer.currentResult.flatMap([&](SymbolVariant s)->ErrorOrSignal{
            interpreter.inspect(syntaxTreeRoot);
            return interpreter.currentResult.flatMap([&](SporeDataVariant result)->ErrorOrSignal{
                return ErrorOrSignal(SporeError(" instead..."));
                // if(result.type==SporeDataVariant::t_signal){
                //     return ErrorOrSignal(result.as_signal);
                // } else {
                //     std::string err="final return value not a signal...was ";
                //     switch (result.type)
                //     {
                //     case SporeDataVariant::t_int:
                //         err+="INT of value: "+result.as_integer;
                //         break;
                    
                //     default:
                //         break;
                //     }
                //     return ErrorOrSignal(SporeError(err+" instead..."));
                // }
            });
        }); 
    } else {
        return ErrorOrSignal(SporeError("failed to parse..."));

    }
    
}