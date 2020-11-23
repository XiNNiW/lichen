#include <gtest/gtest.h>
#include "../src/interpreter/interpreter.h"
#include "../src/lang/sporelang_syntaxtree.h"
#include "../src/interpreter/data_varient.h"
#include <iostream>
using std::cout;

TEST(InterpreterTest, Assignment_AndThenReference) {

    FungiInterpreter* interpreter = new FungiInterpreter();
    cout<<"made interpreter\n";
    RootNode* instructions 
        = new RootNode(
            new StatementsNode(
                new StatementsNode(
                    new AssignmentNode(
                        new IdentifierNode("foo"),
                        new MultiplyOperatorNode(
                            new IntegerNode(5),
                            new IntegerNode(3)
                        )
                    )
                ),
                new AssignmentNode(
                    new IdentifierNode("bar"),
                    new MultiplyOperatorNode(
                        new IdentifierNode("foo"),
                        new IntegerNode(3)
                    )
                )
            )
        );
    cout<<"made AST\n";
    
    interpreter->inspect(instructions);
    cout<<"inspected tree\n";

    Either<RuntimeError, SporeDataVariant> errorOrResult = interpreter->curretResult;
    ASSERT_TRUE(errorOrResult.isRight());
    SporeDataVariant resultSymbol = errorOrResult.getRight();
    ASSERT_EQ(SporeDataVariant::t_unknown, resultSymbol.type);

    ASSERT_EQ(SporeDataVariant::t_int, interpreter->callStack.peek().get("foo").type);
    ASSERT_EQ(15, interpreter->callStack.peek().get("foo").as_integer);
    ASSERT_EQ(SporeDataVariant::t_int, interpreter->callStack.peek().get("bar").type);
    ASSERT_EQ(45, interpreter->callStack.peek().get("bar").as_integer);

}


TEST(InterpreterTest, Assignment_Expression) {

    FungiInterpreter* interpreter = new FungiInterpreter();
    cout<<"made interpreter\n";
    AssignmentNode* instructions 
        = new AssignmentNode(
            new IdentifierNode("foo"),
            new MultiplyOperatorNode(
                new IntegerNode(5),
                new IntegerNode(3)
            )
        );
    cout<<"made AST\n";
    
    interpreter->inspect(instructions);
    cout<<"inspected tree\n";

    Either<RuntimeError, SporeDataVariant> errorOrResult = interpreter->curretResult;
    ASSERT_TRUE(errorOrResult.isRight());
    SporeDataVariant resultSymbol = errorOrResult.getRight();
    ASSERT_EQ(SporeDataVariant::t_unknown, resultSymbol.type);

    ASSERT_EQ(SporeDataVariant::t_int, interpreter->callStack.peek().get("foo").type);
    ASSERT_EQ(15, interpreter->callStack.peek().get("foo").as_integer);

}

TEST(InterpreterTest, BinaryOp_Multiply) {
    FungiInterpreter* interpreter = new FungiInterpreter();
    MultiplyOperatorNode* multiplicationInstruction 
        = new MultiplyOperatorNode(
            new IntegerNode(5),
            new IntegerNode(3)
        );
    interpreter->inspect(multiplicationInstruction);
    Either<RuntimeError,SporeDataVariant> result = interpreter->curretResult;
    ASSERT_TRUE(result.isRight());
    ASSERT_EQ(SporeDataVariant::t_int, result.getRight().type);
    ASSERT_EQ(15, result.getRight().as_integer);
}
