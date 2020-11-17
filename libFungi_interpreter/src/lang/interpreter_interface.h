#pragma once

struct INodeInspector{
    virtual void inspect(class RootNode*)=0;
    virtual void inspect(class StatementsNode*)=0;
    virtual void inspect(class IfElseNode*)=0;
    virtual void inspect(class IfNode*)=0;
    virtual void inspect(class AssignmentNode*)=0;
    virtual void inspect(class AssociateNode*)=0;
    virtual void inspect(class CompositionNode*)=0;
    virtual void inspect(class IsEqualOperatorNode*)=0;
    virtual void inspect(class IsGreaterThanOrEqualOperatorNode*)=0;
    virtual void inspect(class IsGreaterThanNode*)=0;
    virtual void inspect(class IsNotEqualOperatorNode*)=0;
    virtual void inspect(class IsLessThanOrEqualOperatorNode*)=0;
    virtual void inspect(class IsLessThanOperatorNode*)=0;
    virtual void inspect(class DivideOperatorNode*)=0;
    virtual void inspect(class MultiplyOperatorNode*)=0;
    virtual void inspect(class AddOperatorNode*)=0;
    virtual void inspect(class SubtractOperatorNode*)=0;
    virtual void inspect(class AndOperatorNode*)=0;
    virtual void inspect(class OrOperatorNode*)=0;
    virtual void inspect(class XorOperatorNode*)=0;
    virtual void inspect(class FloatNode*)=0;
    virtual void inspect(class IntegerNode*)=0;
    virtual void inspect(class StringNode*)=0;
    virtual void inspect(class IdentifierNode*)=0;

    virtual void inspect(class ASTNode* node)=0;

};
