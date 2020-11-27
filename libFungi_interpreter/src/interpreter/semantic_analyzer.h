#pragma once
#include "../lang/interpreter_interface.h"
#include "error.h"
#include "scoped_symbol_table.h"
#include "either.h"
#include <iostream>

using E = Either<SporeError, SymbolVariant>;

struct SemanticAnalyzer:INodeInspector{
    ScopedSymbolTable globalScope;
    ScopedSymbolTable* currentScope;
    E currentResult;

    SemanticAnalyzer()
    :globalScope(ScopedSymbolTable("GLOBAL",1))
    ,currentResult(E::rightOf(SymbolVariant::voidSymbol()))
    {
        currentScope=&globalScope;
    }

    SemanticAnalyzer(std::vector<SymbolVariant> semanticsForLibrary):SemanticAnalyzer(){
        for(auto symbol:semanticsForLibrary){
            globalScope.insert(symbol);
        }
    }

    void inspect(class RootNode* node){
        //TODO: init built_in name symbols
        std::cout << "inspecting root node\n";

        node->program->identify(this);
        // currentScope = currentScope->enclosingScope;
    }
    void inspect(class StatementsNode* node){
        std::cout<< "inspecing statements";
        if(node->statements)
            node->statements->identify(this);

        currentResult = currentResult.flatMap([&](SymbolVariant result){
            std::cout<< "inspecing final statement";

            node->statement->identify(this);
            return currentResult;
        });
        
    }
    void inspect(class BlockNode* node){
        std::cout <<"inspecting block node\n";
        node->statements->identify(this);
    }

    void inspect(class IfElseNode*){
        currentResult = E::leftOf(SporeError("NOT IMPLEMENTED"));
    }
    void inspect(class IfNode*){
        currentResult = E::leftOf(SporeError("NOT IMPLEMENTED"));
    }
    void inspect(class AssignmentNode* node){
        auto result = currentResult.flatMap([&](SymbolVariant previous){
            std::string name = node->name;
        
            std::cout << "got the name...\n";
            node->expression->identify(this);
            std::cout << "evaluated expr name...\n";

            return currentResult.flatMap([=](SymbolVariant value){
                std::cout << "evaluated expr no error.. about to set on scope..\n";

                // if(value.type != SymbolVariant::t_data_variable|value.type != SymbolVariant::t_function_variable)
                //     return SporeError("bad value or function");
                setName(value, name);
                setScopeLevel(value, currentScope->scopeLevel);
                currentScope->insert(value);
                std::cout << "inserted name into scope...\n";

                return E::rightOf(SymbolVariant::voidSymbol());
            });
        });
        std::cout << "evaluated assignment...\n";
        if(result.isLeft()){
            std::cout<<"got and error\n";
        } else {
            std::cout<<"got a result\n";
            if(result.getRight().type != SymbolVariant::t_unknown){
                std::cout << "not what you expected\n";
            } else {
                std::cout << "its what you thought... a void result\n";

            }
        }
        currentResult = result;
        std::cout << "assigned result...";
       
    }
    void inspect(ArgsNode* args){
        std::cout << "inspecting args node\n";
        if(args->arguments)
            args->arguments->identify(this);
        currentResult=currentResult.flatMap([&](SymbolVariant a){
            std::string last = args->lastArg->value;
            std::vector<std::string> list = std::vector<std::string>();
            switch (a.type)
            {
            case SymbolVariant::t_arguments_list:
                list = a.as_arg_list.list;
                break;
            
            default:
                list.push_back(last);
                break;
            }
            return Either<SporeError,SymbolVariant>(ArgumentsListSymbol(list));
        });
    }
    void inspect(LambdaNode* node){
        std::cout <<"IN LAMBDA NODE\n";
        ScopedSymbolTable lambdaScope = ScopedSymbolTable("LAMBDA",currentScope->scopeLevel+1,currentScope);
        currentScope = &lambdaScope;

        node->arguments->identify(this);
        currentResult = currentResult.flatMap([&](SymbolVariant argsList){
            if(argsList.type != SymbolVariant::t_arguments_list){
                return E::leftOf(SporeError("Bad args for lambda"));
            }
                
            ArgumentsListSymbol formalParams = argsList.as_arg_list;
            for(std::string param:formalParams.list){
                currentScope->insert(DataVariableSymbol(param));
            }
            
            node->block->identify(this);
            currentResult = currentResult.flatMap([&](SymbolVariant expr){
                return E::rightOf(FunctionVariableSymbol(formalParams.list,*(node->block)));
            });
            return currentResult;
        });

        currentScope = currentScope->enclosingScope;

    }
    void inspect(class AssociateNode*){
        currentResult = E::leftOf(SporeError("NOT IMPLEMENTED"));
    }
    void inspect(class CompositionNode*){
        currentResult = E::leftOf(SporeError("NOT IMPLEMENTED"));
    }
    void inspect(class IsEqualOperatorNode* node){
        handleBinaryOperator<IsEqualOperatorNode>(node);
    }
    void inspect(class IsGreaterThanOrEqualOperatorNode* node){
        handleBinaryOperator<IsGreaterThanOrEqualOperatorNode>(node);
    }
    void inspect(class IsGreaterThanNode* node){
        handleBinaryOperator<IsGreaterThanNode>(node);
    }
    void inspect(class IsNotEqualOperatorNode* node){
        handleBinaryOperator<IsNotEqualOperatorNode>(node);
    }
    void inspect(class IsLessThanOrEqualOperatorNode* node){
        handleBinaryOperator<IsLessThanOrEqualOperatorNode>(node);
    }
    void inspect(class IsLessThanOperatorNode* node){
        handleBinaryOperator<IsLessThanOperatorNode>(node);
    }
    void inspect(class DivideOperatorNode* node){
        handleBinaryOperator<DivideOperatorNode>(node);
    }
    void inspect(class MultiplyOperatorNode* node){
        handleBinaryOperator<MultiplyOperatorNode>(node);
    }
    void inspect(class AddOperatorNode* node){
        handleBinaryOperator<AddOperatorNode>(node);
    }
    void inspect(class SubtractOperatorNode* node){
        handleBinaryOperator<SubtractOperatorNode>(node);
    }
    void inspect(class AndOperatorNode* node){
        handleBinaryOperator<AndOperatorNode>(node);
    }
    void inspect(class OrOperatorNode* node){
        handleBinaryOperator<OrOperatorNode>(node);
    }
    void inspect(class XorOperatorNode* node){
        handleBinaryOperator<XorOperatorNode>(node);
    }
    void inspect(class FloatNode* node){
        currentResult = E::rightOf(SymbolVariant(BuiltInSymbol("FLOAT")));
    }
    void inspect(class IntegerNode* node){
        currentResult = E::rightOf(SymbolVariant(BuiltInSymbol("INTEGER")));
    }
    void inspect(class StringNode*node ){
        currentResult = E::rightOf(SymbolVariant(BuiltInSymbol("STRING")));
    }
    void inspect(class IdentifierNode* node){
        std::cout << "in identifier node visit..\n";
        currentResult = E::rightOf(SymbolVariant(IdentifierSymbol(node->value)));
    }

    void inspect(FunctionCallNode* node){
        std::cout <<"IN Function call NODE\n";

        currentResult = currentResult.flatMap([&](SymbolVariant previous){
            if(currentScope->lookup(node->name).type==SymbolVariant::t_unknown){
                return Either<SporeError,SymbolVariant>(SporeError("Function: \""+node->name+"\" not found!"));
            }else{
                node->args->identify(this);
                return currentResult.flatMap([&](SymbolVariant args){
                    // TODO check number of args against formal params... should be <= number of formal params
                    return Either<SporeError,SymbolVariant>(DataVariableSymbol());
                });
            }
        });

    }
    void inspect(class ExpressionsNode* node){
        currentResult = currentResult.flatMap([&](SymbolVariant previous){
            if(node->expressions)
                node->expressions->identify(this);
            return currentResult.flatMap([&](SymbolVariant args){
                node->expression->identify(this);
                return currentResult.flatMap([&](SymbolVariant last){
                    return Either<SporeError, SymbolVariant>(SymbolVariant());
                });
            });
        });
      
    }

    void inspect(class ASTNode* node){
        std::cout << "THIS IS BAD\n";
        assert(false);
    }

    template<typename T>
    void handleBinaryOperator(T* node){
        std::cout << "aobut to call identify on left node\n";
        node->left->identify(this);

        auto res = currentResult.flatMap([&](SymbolVariant leftSymbol){
            std::cout << "about to to call identify on right node\n";

            node->right->identify(this);
            return currentResult.flatMap([&](SymbolVariant rightSymbol){
                std::cout << "about to make bin op result\n";

                std::cout << "about to return bin op result\n";

                return E::rightOf(
                    SymbolVariant(DataVariableSymbol(currentScope->scopeLevel))
                );

                // return E::rightOf(SymbolVariant::voidSymbol());
            });

        });
        std::cout << "binop handled\n";
        currentResult = res;
    }
};