#pragma once

#include "../lang/interpreter_interface.h"
#include "../lang/sporelang_syntaxtree.h"
#include "data_varient.h"
#include "functional"
#include "unordered_map"
#include <cassert>
#include "either.h"
#include "error.h"
#include "call_stack.h"
#include "library.h"

// using E = Either<SporeError, SporeDataVariant>;

struct FungiInterpreter:INodeInspector{
    Either<SporeError, SporeDataVariant> currentResult=Either<SporeError, SporeDataVariant>::rightOf(SporeDataVariant());
    CallStack callStack = CallStack();
    Library globals = Library();
    
    //stack
    FungiInterpreter(){}
    FungiInterpreter(std::map<std::string, SporeDataVariant> bindings){
        globals.load(bindings);
    }
    void inspect( RootNode* node){
        callStack.push(ActivationRecord("PROGRAM", ActivationRecord::t_program,1));
        //should not work if result is not a signal and return
        //SporeDataVariant result = node->identify(this);
        currentResult = Either<SporeError, SporeDataVariant>(SporeDataVariant());
        
        //return result.type==SporeDataVariant::t_signal?result:SporeDataVariant();
        node->program->identify(this);

    }
    void inspect( StatementsNode* node){
        currentResult = currentResult.flatMap([&](SporeDataVariant previous){
            if(node->statements)
                node->statements->identify(this);
            return currentResult.flatMap([&](SporeDataVariant result)->Either<SporeError, SporeDataVariant>{
                node->statement->identify(this);
                return currentResult;
            });
        });
       
    }
    void inspect( BlockNode* node){
        std::cout<<"inspecting block...\n";

        currentResult = currentResult.flatMap([&](SporeDataVariant previous){
            node->statements->identify(this);
            return currentResult;
        });
    }
    void inspect( IfElseNode* node){
        currentResult = Either<SporeError, SporeDataVariant>::leftOf(SporeError("NOT IMPLEMENTED"));
    }
    void inspect(class ExpressionsNode* node){
        std::cout<<"inspecting expressions...\n";
        //currentResult = Either<SporeError, SporeDataVariant>::leftOf(SporeError("NOT IMPLEMENTED"));
        currentResult = currentResult.flatMap([&](SporeDataVariant previous){
            if(node->expressions)
                node->expressions->identify(this);
            return currentResult.flatMap([&](SporeDataVariant exprs){
                node->expression->identify(this);
                return currentResult.flatMap([&](SporeDataVariant last){
                    if(exprs.type==SporeDataVariant::t_list){
                        std::vector<SporeDataVariant> _exprs = exprs.as_list;
                        _exprs.push_back(last);
                        return Either<SporeError,SporeDataVariant>(_exprs);
                    } else{
                        return Either<SporeError, SporeDataVariant>(SporeDataVariant(std::vector<SporeDataVariant>({last})));
                    }
                });
            });
        });
    }
    void inspect( IfNode* node){
        // SporeDataVariant conditionfunction = node->expression->identify(this);
        currentResult = Either<SporeError, SporeDataVariant>::leftOf(SporeError("NOT IMPLEMENTED"));
    }
    void inspect( AssignmentNode* node){
        // will need to store the identifier in the interpreter context
        currentResult = currentResult.flatMap([&](SporeDataVariant previous){
            std::cout << "inspecting assingment for "+node->name+"\n";
            node->expression->identify(this);
            return currentResult.flatMap([&](SporeDataVariant value){
                std::cout << "about to set "+node->name+"\n";

                callStack.peek().set(node->name,value);
                return Either<SporeError, SporeDataVariant>(SporeDataVariant());
            });
            
        });
    }
    void inspect( AssociateNode* node){
        currentResult = Either<SporeError, SporeDataVariant>::leftOf(SporeError("NOT IMPLEMENTED"));
    }
    void inspect( LambdaNode* node){
        std::cout <<"inspecting lambda...\n";
        callStack.push(ActivationRecord("LAMBDA",ActivationRecord::t_function_call,callStack.peek().nestingLevel+1));

        currentResult = currentResult.flatMap([&](SporeDataVariant previous){
            // node->arguments->identify(this);
            // return currentResult.flatMap([&](SporeDataVariant formalParams){
            //     std::cout << "about to make func... formal params is type... "<<int(formalParams.type)<<" \n";

            //     return constructLambda(formalParams, node->block);
            // });
            return constructLambda(node->formalParams, node->block);
        });
        callStack.pop();
    }

    Either<SporeError, SporeDataVariant> constructLambda(std::vector<std::string> formalParams, BlockNode* block){
        return Either<SporeError, SporeDataVariant>(SporeDataVariant(std::function<Either<SporeError, SporeDataVariant>(SporeDataVariant)>(
            [=](SporeDataVariant args)->Either<SporeError, SporeDataVariant> {
                bool hasMultipleArgs = args.type==SporeDataVariant::t_list;
                bool hasValidArgs = hasMultipleArgs ? args.as_list.size() == formalParams.size() : false;

                if(hasValidArgs){

                    for(size_t idx=0; idx<args.as_list.size();idx++){
                        callStack.peek().set(formalParams[idx],args.as_list[idx]);
                    }

                    block->identify(this);
                    return currentResult;
                
                } else {

                    return Either<SporeError, SporeDataVariant>::leftOf(SporeError("Bad Args"));
                }
            }
        )));
    }

    //TODO: allow partial application
    // Either<SporeError, SporeDataVariant> constructLambda(SporeDataVariant formalParams, BlockNode* block){
    //     std::cout<<"making the func\n";
    //     return Either<SporeError, SporeDataVariant>(SporeDataVariant(std::function<Either<SporeError, SporeDataVariant>(SporeDataVariant)>(
    //         [=](SporeDataVariant args)->Either<SporeError, SporeDataVariant> {
    //             std::cout << "function is activated\n";
    //             callStack.push(ActivationRecord("LAMBDA",ActivationRecord::t_function_call,callStack.peek().nestingLevel+1));
    //             bool hasMultipleParams = formalParams.type==SporeDataVariant::t_list;
    //             bool hasMultipleArgs = args.type==SporeDataVariant::t_list;

    //             if(hasMultipleParams&&hasMultipleArgs){
    //                 std::cout << "args are lists\n";

    //                 bool hasValidNumberOfArgs = args.as_list.size() == formalParams.as_list.size();
    //                 if(hasValidNumberOfArgs){
    //                     std::cout << "args pass muster\n";

    //                     for(size_t idx=0; idx<args.as_list.size();idx++){
    //                         std::cout << "setting param...\n";

    //                         callStack.peek().set(formalParams.as_list[idx].as_string,args.as_list[idx]);
    //                     }
    //                     if(args.as_list.size()==formalParams.as_list.size()){
    //                         std::cout << "about to execute body\n";

    //                         block->identify(this);
    //                         callStack.pop();
    //                         return currentResult;
    //                     } else {
    //                         return Either<SporeError,SporeDataVariant>::leftOf(SporeError("Wrong number of args"));
    //                     }
    //                     // else {
    //                     //     std::cout << "partial application\n";
    //                     //     callStack.pop();
    //                     //     return constructLambda(
    //                     //         SporeDataVariant(
    //                     //             std::vector<SporeDataVariant>(
    //                     //                 formalParams.as_list.begin()+args.as_list.size(),
    //                     //                 formalParams.as_list.end()
    //                     //             )
    //                     //         ),
    //                     //         block
    //                     //     );

    //                     // }
    //                 }
    //                 else {
    //                     std::cout << "bad number of params...\n";
    //                     callStack.pop();
    //                     return Either<SporeError, SporeDataVariant>::leftOf(SporeError("Bad Args"));
    //                 }
    //             } else {
    //                 std::cout << "bad args...not a list...\n";
    //                 std::cout << "formal params is type... "<<int(formalParams.type)<<" \n";
    //                 std::cout << "args is type... "<<int(args.type)<<" \n";
    //                 callStack.pop();
    //                 return Either<SporeError, SporeDataVariant>::leftOf(SporeError("Bad Args"));
    //             }
    //         }
    //     )));
    // }

    void inspect( ArgsNode* node){
        std::cout << "INspecting ARGS\n";
        currentResult = currentResult.flatMap([&](SporeDataVariant previous){
            if(node->arguments)
                node->arguments->identify(this);
            return currentResult.flatMap([&](SporeDataVariant args){
                SporeDataVariant finalArg = SporeDataVariant(node->lastArg->value);
               
                if(args.type == SporeDataVariant::t_list){
                    std::vector<SporeDataVariant> _args = args.as_list;
                    _args.push_back(finalArg);
                    return Either<SporeError,SporeDataVariant>::rightOf(SporeDataVariant(_args));
                } else {
                    return Either<SporeError,SporeDataVariant>::rightOf(SporeDataVariant(std::vector<SporeDataVariant>({finalArg})));
                }
                
                
            });
        });

        if(currentResult.isRight())
            std::cout << "args are .. " << typeToString(currentResult.getRight()) << "\n";
            if(currentResult.getRight().type == SporeDataVariant::t_list)
                std::cout << "args have "<< currentResult.getRight().as_list.size() << " elements\n";

    }
    void inspect( CompositionNode* node){
        // i will need to know if the function is fully curried somewhere... the result may also need to be a result
        // SporeDataVariant expr = node->expression->identify(this);
        // SporeDataVariant func = node->function->identify(this);
        currentResult = Either<SporeError, SporeDataVariant>::leftOf(SporeError("NOT IMPLEMENTED"));

        // return SporeDataVariant(std::function<SporeDataVariant(SporeDataVariant)>([](SporeDataVariant x){return func.as_lambda(expr); }));
    }
    void inspect( IsEqualOperatorNode* node){
        // SporeDataVariant lhs = node->left->identify(this);
        // SporeDataVariant rhs = node->right->identify(this);
        // return SporeDataVariant(lhs==rhs);
        currentResult = Either<SporeError, SporeDataVariant>::leftOf(SporeError("NOT IMPLEMENTED"));

    }
    void inspect( IsGreaterThanOrEqualOperatorNode* node){
        // SporeDataVariant lhs = node->left->identify(this);
        // SporeDataVariant rhs = node->right->identify(this);
        // return SporeDataVariant(lhs>=rhs);
        currentResult = Either<SporeError, SporeDataVariant>::leftOf(SporeError("NOT IMPLEMENTED"));

    }
    void inspect( IsGreaterThanNode* node){
        // SporeDataVariant lhs = node->left->identify(this);
        // SporeDataVariant rhs = node->right->identify(this);
        // return SporeDataVariant(lhs>rhs);
        currentResult = Either<SporeError, SporeDataVariant>::leftOf(SporeError("NOT IMPLEMENTED"));

    }
    void inspect( IsNotEqualOperatorNode* node){
        // SporeDataVariant lhs = node->left->identify(this);
        // SporeDataVariant rhs = node->right->identify(this);
        // return SporeDataVariant(lhs!=rhs);
        currentResult = Either<SporeError, SporeDataVariant>::leftOf(SporeError("NOT IMPLEMENTED"));

    }
    void inspect( IsLessThanOrEqualOperatorNode* node){
        // SporeDataVariant lhs = node->left->identify(this);
        // SporeDataVariant rhs = node->right->identify(this);
        // return SporeDataVariant(lhs<=rhs);
        currentResult = Either<SporeError, SporeDataVariant>::leftOf(SporeError("NOT IMPLEMENTED"));

    }
    void inspect( IsLessThanOperatorNode* node){
        // SporeDataVariant lhs = node->left->identify(this);
        // SporeDataVariant rhs = node->right->identify(this);
        // return SporeDataVariant(lhs<rhs);
        currentResult = Either<SporeError, SporeDataVariant>::leftOf(SporeError("NOT IMPLEMENTED"));

    }
    void inspect( DivideOperatorNode* node){
        // SporeDataVariant lhs = node->left->identify(this);
        // SporeDataVariant rhs = node->right->identify(this);
        // return SporeDataVariant(lhs/rhs);
        currentResult = Either<SporeError, SporeDataVariant>::leftOf(SporeError("NOT IMPLEMENTED"));

    }
    void inspect( MultiplyOperatorNode* node){
        node->left->identify(this);
        currentResult=currentResult.flatMap([&](SporeDataVariant lhs){
            node->right->identify(this);
            return currentResult.flatMap([&](SporeDataVariant rhs){
                return lhs*rhs;
            });
        });
    }
    void inspect( AddOperatorNode* node){
    //    node->left->identify(this);
    //     currentResult=currentResult.flatMap([&](SporeDataVariant lhs){
    //         node->right->identify(this);
    //         return currentResult.flatMap([&](SporeDataVariant rhs){
    //             return lhs+rhs;
    //         });
    //     });
        currentResult = Either<SporeError, SporeDataVariant>::leftOf(SporeError("NOT IMPLEMENTED"));

    }
    void inspect( SubtractOperatorNode* node){
        // SporeDataVariant lhs = node->left->identify(this);
        // SporeDataVariant rhs = node->right->identify(this);
        // return SporeDataVariant(lhs-rhs);
        currentResult = Either<SporeError, SporeDataVariant>::leftOf(SporeError("NOT IMPLEMENTED"));

    }
    void inspect( AndOperatorNode* node){
        // SporeDataVariant lhs = node->left->identify(this);
        // SporeDataVariant rhs = node->right->identify(this);
        // return SporeDataVariant(lhs&rhs);
        currentResult = Either<SporeError, SporeDataVariant>::leftOf(SporeError("NOT IMPLEMENTED"));

    }
    void inspect( OrOperatorNode* node){
        // SporeDataVariant lhs = node->left->identify(this);
        // SporeDataVariant rhs = node->right->identify(this);
        // return SporeDataVariant(lhs|rhs);
        currentResult = Either<SporeError, SporeDataVariant>::leftOf(SporeError("NOT IMPLEMENTED"));

    }
    void inspect( XorOperatorNode* node){
        // SporeDataVariant lhs = node->left->identify(this);
        // SporeDataVariant rhs = node->right->identify(this);
        // return SporeDataVariant(lhs^rhs);
        currentResult = Either<SporeError, SporeDataVariant>::leftOf(SporeError("NOT IMPLEMENTED"));

    }
    void inspect( FloatNode* node){
        currentResult = Either<SporeError, SporeDataVariant>(SporeDataVariant(node->value));
    }
    void inspect( IntegerNode* node){
        currentResult = Either<SporeError, SporeDataVariant>(SporeDataVariant(node->value));
    }
    void inspect( StringNode* node){
        currentResult = Either<SporeError, SporeDataVariant>(SporeDataVariant(node->value));
    }
    void inspect( IdentifierNode* node){
        //return SporeDataVariant(node->value)
        //should look up identifier from built in functions
        //if not found it should look in interpreter context for variables
        std::cout << "about to look up identifier: "<<node->value<<"\n";
        auto maybeValue = callStack.peek().get(node->value);
        
        maybeValue.flatMap([&](SporeDataVariant value){
             currentResult = Either<SporeError, SporeDataVariant>(value);
        });
        if(maybeValue.isNothing()){
            currentResult = Either<SporeError, SporeDataVariant>(SporeError("Name not found"));
        }
        std::cout << node->value << " is " << typeToString(currentResult.getRight())<<"\n";
    }

    void inspect(FunctionCallNode* node){
        std::cout << "inspecting function call\n";
        currentResult = currentResult.flatMap([&](SporeDataVariant previous){

            Maybe<SporeDataVariant> function = callStack.peek().get(node->name);

            if(function.isNothing())
                function = globals.get(node->name);
                
            return function
                .toEither<SporeError>(SporeError(node->name+" is not found"))
                .flatMap([&](SporeDataVariant func){
                    if(func.type == SporeDataVariant::t_lambda){
                        node->args->identify(this);
                        return currentResult.flatMap([&](SporeDataVariant args){
                            std::cout << "calling function...\n";
                            // if(args.type == SporeDataVariant::t_list){
                            return func.as_lambda(args);
                            // } else {
                            //     return Either<SporeError,SporeDataVariant>(SporeError("improper args for function: "+node->name));
                            // }
                        });
                    } else {
                        return Either<SporeError,SporeDataVariant>::leftOf(SporeError(node->name+" is not a function"));
                    }

                });
           
        });

    }

    void inspect( ASTNode* node){
        //die this should never happen
        assert(false);
    }
};