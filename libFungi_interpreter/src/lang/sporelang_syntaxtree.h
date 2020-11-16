#pragma once
#include <string>

struct ASTNode{
    
};
struct RootNode:ASTNode{
    RootNode(ASTNode* program);
};
struct StatementsNode:ASTNode{
    StatementsNode(StatementsNode* statements, ASTNode* statement);
};
struct FloatNode:ASTNode{
    FloatNode(double value);

};
struct IntegerNode:ASTNode{
    IntegerNode(long value);

};
struct StringNode:ASTNode{
    StringNode(std::string value);

};
struct IdentifierNode:ASTNode{
    IdentifierNode(std::string value);

};
struct NotOperatorNode:ASTNode{
    NotOperatorNode(ASTNode* expression);

};
struct LambdaNode:ASTNode{
    LambdaNode(ASTNode* arguments, ASTNode* expression);

};
struct IfElseNode:ASTNode{
    IfElseNode(ASTNode* expression, ASTNode* statements_ifTrue, ASTNode* statements_ifFalse);
};
struct IfNode:ASTNode{
    IfNode(ASTNode* expression, ASTNode* statements_ifTrue);
};
struct AssignmentNode:ASTNode{
    AssignmentNode(IdentifierNode* name, ASTNode* expression);
};
struct AssociateNode:ASTNode{
    AssociateNode(ASTNode* expressionToEvalSecond, ASTNode* expressionToEvalFirst);
};
struct CompositionNode:ASTNode{
    CompositionNode(ASTNode* expression, IdentifierNode* function);
};
struct IsEqualOperatorNode:ASTNode{
    IsEqualOperatorNode(ASTNode* left,ASTNode* right);
};
struct IsGreaterThanOrEqualOperatorNode:ASTNode{
    IsGreaterThanOrEqualOperatorNode(ASTNode* left,ASTNode* right);

};
struct IsGreaterThanNode:ASTNode{
    IsGreaterThanNode(ASTNode* left,ASTNode* right);

};
struct IsNotEqualOperatorNode:ASTNode{
    IsNotEqualOperatorNode(ASTNode* left,ASTNode* right);

};
struct IsLessThanOrEqualOperatorNode:ASTNode{
    IsLessThanOrEqualOperatorNode(ASTNode* left,ASTNode* right);

};
struct IsLessThanOperatorNode:ASTNode{
    IsLessThanOperatorNode(ASTNode* left,ASTNode* right);

};
struct DivideOperatorNode:ASTNode{
    DivideOperatorNode(ASTNode* left,ASTNode* right);

};
struct MultiplyOperatorNode:ASTNode{
    MultiplyOperatorNode(ASTNode* left,ASTNode* right);

};
struct AddOperatorNode:ASTNode{
    AddOperatorNode(ASTNode* left,ASTNode* right);

};
struct SubtractOperatorNode:ASTNode{
    SubtractOperatorNode(ASTNode* left,ASTNode* right);

};
struct AndOperatorNode:ASTNode{
    AndOperatorNode(ASTNode* left,ASTNode* right);
};
struct OrOperatorNode:ASTNode{
    OrOperatorNode(ASTNode* left,ASTNode* right);
};
struct XorOperatorNode:ASTNode{
    XorOperatorNode(ASTNode* left,ASTNode* right);

};



struct INodeInspector{
    virtual void inspect(const RootNode& node)=0;
    virtual void inspect(const StatementsNode* node)=0;
    virtual void inspect(const IfElseNode* node)=0;
    virtual void inspect(const IfNode* node)=0;
    virtual void inspect(const AssignmentNode* node)=0;
    virtual void inspect(const AssociateNode* node)=0;
    virtual void inspect(const CompositionNode* node)=0;
    virtual void inspect(const IsEqualOperatorNode* node)=0;
    virtual void inspect(const IsGreaterThanOrEqualOperatorNode* node)=0;
    virtual void inspect(const IsGreaterThanNode* node)=0;
    virtual void inspect(const IsNotEqualOperatorNode* node)=0;
    virtual void inspect(const IsLessThanOrEqualOperatorNode* node)=0;
    virtual void inspect(const IsLessThanOperatorNode& node)=0;
    virtual void inspect(const DivideOperatorNode* node)=0;
    virtual void inspect(const MultiplyOperatorNode* node)=0;
    virtual void inspect(const AddOperatorNode* node)=0;
    virtual void inspect(const SubtractOperatorNode* node)=0;
    virtual void inspect(const AndOperatorNode* node)=0;
    virtual void inspect(const OrOperatorNode* node)=0;
    virtual void inspect(const XorOperatorNode* node)=0;
    virtual void inspect(const FloatNode* node)=0;
    virtual void inspect(const IntegerNode* node)=0;
    virtual void inspect(const StringNode* node)=0;
    virtual void inspect(const IdentifierNode* node)=0;

    virtual void inspect(const ASTNode* node)=0;

};


