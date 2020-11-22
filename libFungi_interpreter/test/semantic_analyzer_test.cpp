#include <gtest/gtest.h>
#include "../src/interpreter/semantic_analyzer.h"
#include "../src/lang/sporelang_syntaxtree.h"
#include "../src/interpreter/data_varient.h"
#include <iostream>
using std::cout;


TEST(SemanticAnalyzerTest, Assignment_Expression) {

    SemanticAnalyzer* interpreter = new SemanticAnalyzer();
    cout<<"made interpreter\n";
    AssignmentNode* instructions 
        = new AssignmentNode(
            new IdentifierNode("foo"),
            new AddOperatorNode(
                new IntegerNode(5),
                new IntegerNode(3)
            )
        );
    cout<<"made AST\n";
    
    interpreter->inspect(instructions);
    cout<<"inspected tree\n";

    Either<SyntaxError,SymbolVariant> errorOrResult = interpreter->currentResult;
    ASSERT_TRUE(errorOrResult.isRight());
    SymbolVariant resultSymbol = errorOrResult.getRight();
    ASSERT_EQ(SymbolVariant::t_unknown, resultSymbol.type);

    ASSERT_EQ(SymbolVariant::t_data_variable, interpreter->globalScope.lookupInCurrentScopeOnly("foo").type);

}

TEST(SemanticAnalyzerTest, BinaryOp_Add) {

    SemanticAnalyzer* interpreter = new SemanticAnalyzer();
    cout<<"made interpreter\n";
    AddOperatorNode* instructions 
        = new AddOperatorNode(
            new IntegerNode(5),
            new IntegerNode(3)
        );
    cout<<"made AST\n";
    
    interpreter->inspect(instructions);
    cout<<"inspected tree\n";

    Either<SyntaxError,SymbolVariant> errorOrResult = interpreter->currentResult;
    ASSERT_TRUE(errorOrResult.isRight());
    SymbolVariant resultSymbol = errorOrResult.getRight();
    ASSERT_EQ(SymbolVariant::t_data_variable, resultSymbol.type);

}

TEST(SemanticAnalyzerTest, BinaryOp_Subtract) {

    SemanticAnalyzer* interpreter = new SemanticAnalyzer();
    cout<<"made interpreter\n";
    SubtractOperatorNode* instructions 
        = new SubtractOperatorNode(
            new IntegerNode(5),
            new IntegerNode(3)
        );
    cout<<"made AST\n";
    
    interpreter->inspect(instructions);
    cout<<"inspected tree\n";

    Either<SyntaxError,SymbolVariant> errorOrResult = interpreter->currentResult;
    ASSERT_TRUE(errorOrResult.isRight());
    SymbolVariant resultSymbol = errorOrResult.getRight();
    ASSERT_EQ(SymbolVariant::t_data_variable, resultSymbol.type);

}

TEST(SemanticAnalyzerTest, BinaryOp_Multiply) {

    SemanticAnalyzer* interpreter = new SemanticAnalyzer();
    cout<<"made interpreter\n";
    MultiplyOperatorNode* instructions 
        = new MultiplyOperatorNode(
            new IntegerNode(5),
            new IntegerNode(3)
        );
    cout<<"made AST\n";
    
    interpreter->inspect(instructions);
    cout<<"inspected tree\n";

    Either<SyntaxError,SymbolVariant> errorOrResult = interpreter->currentResult;
    ASSERT_TRUE(errorOrResult.isRight());
    SymbolVariant resultSymbol = errorOrResult.getRight();
    ASSERT_EQ(SymbolVariant::t_data_variable, resultSymbol.type);

}

TEST(SemanticAnalyzerTest, BinaryOp_Divide) {

    SemanticAnalyzer* interpreter = new SemanticAnalyzer();
    cout<<"made interpreter\n";
    DivideOperatorNode* instruction 
        = new DivideOperatorNode(
            new IntegerNode(5),
            new IntegerNode(3)
        );
    cout<<"made AST\n";
    
    interpreter->inspect(instruction);
    cout<<"inspected tree\n";

    Either<SyntaxError,SymbolVariant> errorOrResult = interpreter->currentResult;
    ASSERT_TRUE(errorOrResult.isRight());
    SymbolVariant resultSymbol = errorOrResult.getRight();
    ASSERT_EQ(SymbolVariant::t_data_variable, resultSymbol.type);

}
