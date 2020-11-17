#pragma once

#include "../lang/interpreter_interface.h"
#include "../lang/sporelang_syntaxtree.h"

struct FungiInterpreter:INodeInspector{
    void inspect( RootNode* node){}
    void inspect( StatementsNode* node){}
    void inspect( IfElseNode* node){}
    void inspect( IfNode* node){}
    void inspect( AssignmentNode* node){}
    void inspect( AssociateNode* node){}
    void inspect( CompositionNode* node){}
    void inspect( IsEqualOperatorNode* node){}
    void inspect( IsGreaterThanOrEqualOperatorNode* node){}
    void inspect( IsGreaterThanNode* node){}
    void inspect( IsNotEqualOperatorNode* node){}
    void inspect( IsLessThanOrEqualOperatorNode* node){}
    void inspect( IsLessThanOperatorNode* node){}
    void inspect( DivideOperatorNode* node){}
    void inspect( MultiplyOperatorNode* node){}
    void inspect( AddOperatorNode* node){}
    void inspect( SubtractOperatorNode* node){}
    void inspect( AndOperatorNode* node){}
    void inspect( OrOperatorNode* node){}
    void inspect( XorOperatorNode* node){}
    void inspect( FloatNode* node){}
    void inspect( IntegerNode* node){}
    void inspect( StringNode* node){}
    void inspect( IdentifierNode* node){}

    void inspect( ASTNode* node){}
};