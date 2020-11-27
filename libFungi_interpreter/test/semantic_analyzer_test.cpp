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
            "foo",
            new AddOperatorNode(
                new IntegerNode(5),
                new IntegerNode(3)
            )
        );
    cout<<"made AST\n";
    
    interpreter->inspect(instructions);
    cout<<"inspected tree\n";

    Either<SporeError,SymbolVariant> errorOrResult = interpreter->currentResult;
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

    Either<SporeError,SymbolVariant> errorOrResult = interpreter->currentResult;
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

    Either<SporeError,SymbolVariant> errorOrResult = interpreter->currentResult;
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

    Either<SporeError,SymbolVariant> errorOrResult = interpreter->currentResult;
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

    Either<SporeError,SymbolVariant> errorOrResult = interpreter->currentResult;
    ASSERT_TRUE(errorOrResult.isRight());
    SymbolVariant resultSymbol = errorOrResult.getRight();
    ASSERT_EQ(SymbolVariant::t_data_variable, resultSymbol.type);

}

TEST(SemanticAnalyzerTest, Assignment_AndThenReference) {

    SemanticAnalyzer interpreter = SemanticAnalyzer();
    cout<<"made interpreter\n";
    RootNode* instructions 
        = new RootNode(
            new StatementsNode(
                new StatementsNode(
                    new AssignmentNode(
                        "foo",
                        new MultiplyOperatorNode(
                            new IntegerNode(5),
                            new IntegerNode(3)
                        )
                    )
                ),
                new AssignmentNode(
                    "bar",
                    new MultiplyOperatorNode(
                        new IdentifierNode("foo"),
                        new IntegerNode(3)
                    )
                )
            )
        );
    cout<<"made AST\n";
    
    interpreter.inspect(instructions);
    cout<<"inspected tree\n";

    Either<SporeError, SymbolVariant> errorOrResult = interpreter.currentResult;
    ASSERT_TRUE(errorOrResult.isRight());
    SymbolVariant resultSymbol = errorOrResult.getRight();
    ASSERT_EQ(SymbolVariant::t_unknown, resultSymbol.type);

    ASSERT_EQ(SymbolVariant::t_data_variable, interpreter.globalScope.lookup("foo").type);
    ASSERT_EQ(SymbolVariant::t_data_variable, interpreter.globalScope.lookup("bar").type);

}

TEST(SemanticAnalyzerTest, Assignment_Lambda) {

    SemanticAnalyzer interpreter = SemanticAnalyzer();
    cout<<"made interpreter\n";
    RootNode* instructions 
        = new RootNode(
            new StatementsNode(
                new StatementsNode(
                    new AssignmentNode(
                        "foo",
                        new LambdaNode(
                            new ArgsNode(new IdentifierNode("bill")),
                            new BlockNode(
                                new StatementsNode(
                                    new MultiplyOperatorNode(
                                        new IntegerNode(6),
                                        new IdentifierNode("bill")
                                    )
                                )
                            )
                        )
                    )
                )
            )
        );
    cout<<"made AST\n";
    
    interpreter.inspect(instructions);
    cout<<"inspected tree\n";

    Either<SporeError, SymbolVariant> errorOrResult = interpreter.currentResult;
    ASSERT_TRUE(errorOrResult.isRight());
    SymbolVariant resultSymbol = errorOrResult.getRight();
    ASSERT_EQ(SymbolVariant::t_unknown, resultSymbol.type);

    ASSERT_EQ(SymbolVariant::t_function_variable, interpreter.globalScope.lookup("foo").type);

}

TEST(SemanticAnalyzerTest, FunctionCall) {

    SemanticAnalyzer interpreter = SemanticAnalyzer();
    cout<<"made interpreter\n";
    RootNode* instructions 
        = new RootNode(
            new StatementsNode(
                new StatementsNode(
                    new AssignmentNode(
                        "foo",
                        new LambdaNode(
                            new ArgsNode(new IdentifierNode("bill")),
                            new BlockNode(
                                new StatementsNode(
                                    new MultiplyOperatorNode(
                                        new IntegerNode(6),
                                        new IdentifierNode("bill")
                                    )
                                )
                            )
                        )
                    )
                ),
                new AssignmentNode(
                    "hannah",
                    new FunctionCallNode(
                        "foo",
                        new ExpressionsNode(new IntegerNode(5))
                    )
                )
            )
        );
    cout<<"made AST\n";
    
    interpreter.inspect(instructions);
    cout<<"inspected tree\n";

    Either<SporeError, SymbolVariant> errorOrResult = interpreter.currentResult;
    ASSERT_TRUE(errorOrResult.isRight());
    SymbolVariant resultSymbol = errorOrResult.getRight();
    ASSERT_EQ(SymbolVariant::t_unknown, resultSymbol.type);

    ASSERT_EQ(SymbolVariant::t_data_variable, interpreter.globalScope.lookup("hannah").type);

}

