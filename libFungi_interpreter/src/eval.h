#pragma once
#include <string>
#include "interpreter/interpreter.h"
#include "interpreter/semantic_analyzer.h"
#include <lib/sporelang_bison.hpp>
#include "lang/sporelang_syntaxtree.h"
#include "interpreter/either.h"
#include "interpreter/error.h"
#include <functional>
#include <sstream>
#include <iostream>
#include "interpreter/algae_bridge.h"


extern std::istream * yyin;
extern RootNode* syntaxTreeRoot;
extern void scan_string(const char*);
extern int yyparse();

using ErrorOrSignal = Either<SporeError,Signal<double,int>>;

ErrorOrSignal eval(std::string program){
    std::cout<<"about to parse\n";
    scan_string((program).c_str());
    if(yyparse()==0){
        std::cout<<"PARSE SUCCESSFUL\n";

        SemanticAnalyzer analyzer = SemanticAnalyzer(getAlgaeSemantics());
        FungiInterpreter interpreter = FungiInterpreter(makeAlgaeBindings<double,double,int>(48000));
        analyzer.inspect(syntaxTreeRoot);
        std::cout<<"ANALYSIS SUCCESSFUL\n";

        return analyzer.currentResult.flatMap([&](SymbolVariant s)->ErrorOrSignal{
            interpreter.inspect(syntaxTreeRoot);
            return interpreter.currentResult.flatMap([&](SporeDataVariant result)->ErrorOrSignal{
                std::cout<<"Interp SUCCESSFUL\n";

                // return ErrorOrSignal(SporeError(" instead..."));
                if(result.type==SporeDataType::t_signal){
                    std::cout << "returning signal!!!";
                    return ErrorOrSignal::rightOf(result.as_signal);
                } else {
                    std::cout<<"it did what you thought\n";
                    
                    return ErrorOrSignal::leftOf(
                        SporeError("final return value not a Signal...was "+typeToString(result)+" instead.")
                    );
                }
            });
        }); 
    } else {
        std::cout<<"PARSE FAILED\n";

        return ErrorOrSignal::leftOf(SporeError("failed to parse..."));

    }
    
}