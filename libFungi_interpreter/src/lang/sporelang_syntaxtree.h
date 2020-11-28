#pragma once
#include <string>
#include "interpreter_interface.h"
#include "../interpreter/data_varient.h"
#include <iostream>

struct ASTNode{
    virtual void identify(INodeInspector* visitor)=0;
};

struct RootNode:ASTNode{
    ASTNode* program;
    RootNode(ASTNode* program):program(program){};
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }
};


struct StatementsNode: ASTNode{
    StatementsNode* statements;
    ASTNode* statement;

    StatementsNode(StatementsNode* statements, ASTNode* statement)
    :statements(statements),statement(statement){}

    StatementsNode(ASTNode* statement):statement(statement){}
    
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }

};

struct BlockNode: ASTNode{
    StatementsNode* statements;
    BlockNode(StatementsNode* statements)
    :statements(statements)
    {}
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }

};

struct ExpressionsNode: ASTNode{
    ExpressionsNode* expressions;
    ASTNode* expression;
    ExpressionsNode(ExpressionsNode* expressions, ASTNode* expression)
    :expressions(expressions), expression(expression)
    {}
    ExpressionsNode(ASTNode* expression)
    :expression(expression)
    {}
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }
};


struct NotOperatorNode:ASTNode{
    ASTNode* expression;
    NotOperatorNode(ASTNode* expression)
    :expression(expression){}
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }
    
};

struct ArgsNode:ASTNode{
    ArgsNode* arguments;
    IdentifierNode* lastArg;
    ArgsNode(ArgsNode* arguments, IdentifierNode* lastArg)
    :arguments(arguments),lastArg(lastArg){}
    ArgsNode(IdentifierNode* lastArg)
    :lastArg(lastArg){}
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }
};



struct IfElseNode:ASTNode{
    ASTNode* expression;
    ASTNode* statements_ifTrue;
    ASTNode* statements_ifFalse;
    IfElseNode(ASTNode* expression, ASTNode* statements_ifTrue, ASTNode* statements_ifFalse)
    :expression(expression),statements_ifTrue(statements_ifTrue),statements_ifFalse(statements_ifFalse){}
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }
};
struct IfNode:ASTNode{
    ASTNode* expression;
    ASTNode* statements_ifTrue;
    IfNode(ASTNode* expression, ASTNode* statements_ifTrue)
    :expression(expression),statements_ifTrue(statements_ifTrue){}
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }
};
struct AssignmentNode:ASTNode{
    std::string name;
    ASTNode* expression;
    AssignmentNode(std::string name, ASTNode* expression)
    :name(name),expression(expression){}
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }
};
struct LambdaNode:ASTNode{
    ArgsNode* arguments;
    // std::vector<std::string>* arguments;
    BlockNode* block;
    std::vector<std::string> formalParams = std::vector<std::string>();
    LambdaNode(ArgsNode* arguments, BlockNode* block)
    :arguments(arguments),block(block){}
    void identify(INodeInspector* visitor){
        std::cout << "in the identify for lambda\n";
        visitor->inspect(this);
    }
};
struct AssociateNode:ASTNode{
    ASTNode* expressionToEvalSecond;
    ASTNode* expressionToEvalFirst;
    AssociateNode(ASTNode* expressionToEvalSecond, ASTNode* expressionToEvalFirst)
    :expressionToEvalSecond(expressionToEvalSecond),expressionToEvalFirst(expressionToEvalFirst){}
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }
};
struct CompositionNode:ASTNode{
    ASTNode* expression;
    ASTNode* function;
    CompositionNode(ASTNode* expression, ASTNode* function)
    :expression(expression),function(function){}
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }
};
struct IsEqualOperatorNode:ASTNode{
    ASTNode* left;
    ASTNode* right;
    IsEqualOperatorNode(ASTNode* left,ASTNode* right)
    :left(left),right(right){}
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }
};
struct IsGreaterThanOrEqualOperatorNode:ASTNode{
    ASTNode* left;
    ASTNode* right;
    IsGreaterThanOrEqualOperatorNode(ASTNode* left,ASTNode* right)
    :left(left),right(right){}
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }
};
struct IsGreaterThanNode:ASTNode{
    ASTNode* left;
    ASTNode* right;
    IsGreaterThanNode(ASTNode* left,ASTNode* right)
    :left(left),right(right){}
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }
};
struct IsNotEqualOperatorNode:ASTNode{
    ASTNode* left;
    ASTNode* right;
    IsNotEqualOperatorNode(ASTNode* left,ASTNode* right)
    :left(left),right(right){}
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }
};
struct IsLessThanOrEqualOperatorNode:ASTNode{
    ASTNode* left;
    ASTNode* right;
    IsLessThanOrEqualOperatorNode(ASTNode* left,ASTNode* right)
    :left(left),right(right){}
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }
};
struct IsLessThanOperatorNode:ASTNode{
    ASTNode* left;
    ASTNode* right;
    IsLessThanOperatorNode(ASTNode* left,ASTNode* right)
    :left(left),right(right){}
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }
};
struct DivideOperatorNode:ASTNode{
    ASTNode* left;
    ASTNode* right;
    DivideOperatorNode(ASTNode* left,ASTNode* right)
    :left(left),right(right){}
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }
};
struct MultiplyOperatorNode:ASTNode{
    ASTNode* left;
    ASTNode* right;
    MultiplyOperatorNode(ASTNode* left,ASTNode* right)
    :left(left),right(right){}
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }
};
struct AddOperatorNode:ASTNode{
    ASTNode* left;
    ASTNode* right;
    AddOperatorNode(ASTNode* left,ASTNode* right)
    :left(left),right(right){}
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }
};
struct SubtractOperatorNode:ASTNode{
    ASTNode* left;
    ASTNode* right;
    SubtractOperatorNode(ASTNode* left,ASTNode* right)
    :left(left),right(right){}
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }
};
struct AndOperatorNode:ASTNode{
    ASTNode* left;
    ASTNode* right;
    AndOperatorNode(ASTNode* left,ASTNode* right)
    :left(left),right(right){}
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }
};
struct OrOperatorNode:ASTNode{
    ASTNode* left;
    ASTNode* right;
    OrOperatorNode(ASTNode* left,ASTNode* right)
    :left(left),right(right){}
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }
};
struct XorOperatorNode:ASTNode{
    ASTNode* left;
    ASTNode* right;
    XorOperatorNode(ASTNode* left,ASTNode* right)
    :left(left),right(right){}
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }
};
struct FloatNode:ASTNode{
    double value;
    FloatNode(double value):value(value){}
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }
};
struct IntegerNode:ASTNode{
    int value;
    IntegerNode(long value):value(value){}
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }
};
struct StringNode:ASTNode{
    std::string value;
    StringNode(std::string value):value(value){}
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }
};
struct IdentifierNode:ASTNode{
    std::string value;
    IdentifierNode(const char * value):value(value){}
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }
};

struct FunctionCallNode:ASTNode{
    std::string name;
    ExpressionsNode* args;
    FunctionCallNode(std::string name, ExpressionsNode* args)
    :name(name),args(args){}
    void identify(INodeInspector* visitor){
        visitor->inspect(this);
    }
};
