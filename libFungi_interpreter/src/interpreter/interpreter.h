#pragma once

#include "../lang/interpreter_interface.h"
#include "../lang/sporelang_syntaxtree.h"
#include "data_varient.h"
#include "functional"
#include "unordered_map"
#include <cassert>
#include "either.h"
#include "runtime_error.h"
#include "call_stack.h"

using E = Either<RuntimeError, SporeDataVariant>;

struct FungiInterpreter:INodeInspector{
    E curretResult=E::rightOf(SporeDataVariant());
    CallStack callStack = CallStack();
    //stack
    FungiInterpreter(){
        callStack.push(ActivationRecord("PROGRAM", ActivationRecord::t_program,1));
    }
    void inspect( RootNode* node){
        //should not work if result is not a signal and return
        //SporeDataVariant result = node->identify(this);
        
        //return result.type==SporeDataVariant::t_signal?result:SporeDataVariant();
        node->program->identify(this);
        curretResult = E(SporeDataVariant());

    }
    void inspect( StatementsNode* node){
        if(node->statements)
            node->statements->identify(this);
        node->statement->identify(this);
    }
    void inspect( IfElseNode* node){}
    void inspect( IfNode* node){
        // SporeDataVariant conditionfunction = node->expression->identify(this);

    }
    void inspect( AssignmentNode* node){
        // will need to store the identifier in the interpreter context
        node->expression->identify(this);
        callStack.peek().set(node->name->value,curretResult.getRight());
        curretResult = E(SporeDataVariant());
    }
    void inspect( AssociateNode* node){}
    void inspect( CompositionNode* node){
        // i will need to know if the function is fully curried somewhere... the result may also need to be a result
        // SporeDataVariant expr = node->expression->identify(this);
        // SporeDataVariant func = node->function->identify(this);
        // return SporeDataVariant(std::function<SporeDataVariant(SporeDataVariant)>([](SporeDataVariant x){return func.as_lambda(expr); }));
    }
    void inspect( IsEqualOperatorNode* node){
        // SporeDataVariant lhs = node->left->identify(this);
        // SporeDataVariant rhs = node->right->identify(this);
        // return SporeDataVariant(lhs==rhs);
    }
    void inspect( IsGreaterThanOrEqualOperatorNode* node){
        // SporeDataVariant lhs = node->left->identify(this);
        // SporeDataVariant rhs = node->right->identify(this);
        // return SporeDataVariant(lhs>=rhs);
    }
    void inspect( IsGreaterThanNode* node){
        // SporeDataVariant lhs = node->left->identify(this);
        // SporeDataVariant rhs = node->right->identify(this);
        // return SporeDataVariant(lhs>rhs);
    }
    void inspect( IsNotEqualOperatorNode* node){
        // SporeDataVariant lhs = node->left->identify(this);
        // SporeDataVariant rhs = node->right->identify(this);
        // return SporeDataVariant(lhs!=rhs);
    }
    void inspect( IsLessThanOrEqualOperatorNode* node){
        // SporeDataVariant lhs = node->left->identify(this);
        // SporeDataVariant rhs = node->right->identify(this);
        // return SporeDataVariant(lhs<=rhs);
    }
    void inspect( IsLessThanOperatorNode* node){
        // SporeDataVariant lhs = node->left->identify(this);
        // SporeDataVariant rhs = node->right->identify(this);
        // return SporeDataVariant(lhs<rhs);
    }
    void inspect( DivideOperatorNode* node){
        // SporeDataVariant lhs = node->left->identify(this);
        // SporeDataVariant rhs = node->right->identify(this);
        // return SporeDataVariant(lhs/rhs);
    }
    void inspect( MultiplyOperatorNode* node){
        node->left->identify(this);
        curretResult=curretResult.flatMap([&](SporeDataVariant lhs){
            node->right->identify(this);
            return curretResult.flatMap([&](SporeDataVariant rhs){
                return lhs*rhs;
            });
        });
    }
    void inspect( AddOperatorNode* node){
    //    node->left->identify(this);
    //     curretResult=curretResult.flatMap([&](SporeDataVariant lhs){
    //         node->right->identify(this);
    //         return curretResult.flatMap([&](SporeDataVariant rhs){
    //             return lhs+rhs;
    //         });
    //     });
    }
    void inspect( SubtractOperatorNode* node){
        // SporeDataVariant lhs = node->left->identify(this);
        // SporeDataVariant rhs = node->right->identify(this);
        // return SporeDataVariant(lhs-rhs);
    }
    void inspect( AndOperatorNode* node){
        // SporeDataVariant lhs = node->left->identify(this);
        // SporeDataVariant rhs = node->right->identify(this);
        // return SporeDataVariant(lhs&rhs);
    }
    void inspect( OrOperatorNode* node){
        // SporeDataVariant lhs = node->left->identify(this);
        // SporeDataVariant rhs = node->right->identify(this);
        // return SporeDataVariant(lhs|rhs);
    }
    void inspect( XorOperatorNode* node){
        // SporeDataVariant lhs = node->left->identify(this);
        // SporeDataVariant rhs = node->right->identify(this);
        // return SporeDataVariant(lhs^rhs);
    }
    void inspect( FloatNode* node){
        curretResult = E(SporeDataVariant(node->value));
    }
    void inspect( IntegerNode* node){
        curretResult = E(SporeDataVariant(node->value));
    }
    void inspect( StringNode* node){
        curretResult = E(SporeDataVariant(node->value));
    }
    void inspect( IdentifierNode* node){
        //return SporeDataVariant(node->value)
        //should look up identifier from built in functions
        //if not found it should look in interpreter context for variables
        curretResult = callStack.peek().get(node->value);
    }

    void inspect( ASTNode* node){
        //die this should never happen
        assert(false);
    }
};