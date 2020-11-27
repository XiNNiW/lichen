#include <gtest/gtest.h>
#include "../src/interpreter/interpreter.h"
#include "../src/lang/sporelang_syntaxtree.h"
#include "../src/interpreter/data_varient.h"
#include "../src/interpreter/error.h"
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
    
    interpreter->inspect(instructions);
    cout<<"inspected tree\n";

    Either<SporeError, SporeDataVariant> errorOrResult = interpreter->currentResult;
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
    RootNode* instructions 
        = new RootNode(new StatementsNode(new AssignmentNode(
            "foo",
            new MultiplyOperatorNode(
                new IntegerNode(5),
                new IntegerNode(3)
            )
        )));
    cout<<"made AST\n";
    
    interpreter->inspect(instructions);
    cout<<"inspected tree\n";

    Either<SporeError, SporeDataVariant> errorOrResult = interpreter->currentResult;
    ASSERT_TRUE(errorOrResult.isRight());
    SporeDataVariant resultSymbol = errorOrResult.getRight();
    ASSERT_EQ(SporeDataVariant::t_unknown, resultSymbol.type);

    ASSERT_EQ(SporeDataVariant::t_int, interpreter->callStack.peek().get("foo").type);
    ASSERT_EQ(15, interpreter->callStack.peek().get("foo").as_integer);

}

TEST(InterpreterTest, BinaryOp_Multiply) {
    FungiInterpreter* interpreter = new FungiInterpreter();
    RootNode* multiplicationInstruction 
        = new RootNode(new StatementsNode(new MultiplyOperatorNode(
            new IntegerNode(5),
            new IntegerNode(3)
        )));
    interpreter->inspect(multiplicationInstruction);
    Either<SporeError,SporeDataVariant> result = interpreter->currentResult;
    ASSERT_TRUE(result.isRight());
    ASSERT_EQ(SporeDataVariant::t_int, result.getRight().type);
    ASSERT_EQ(15, result.getRight().as_integer);
}


TEST(InterpreterTest, Assignment_Lambda) {

    FungiInterpreter interpreter = FungiInterpreter();
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

    Either<SporeError, SporeDataVariant> errorOrResult = interpreter.currentResult;
    ASSERT_TRUE(errorOrResult.isRight());
    SporeDataVariant result = errorOrResult.getRight();
    ASSERT_EQ(SporeDataVariant::t_unknown, result.type);

    ASSERT_EQ(SporeDataVariant::t_lambda, interpreter.callStack.peek().get("foo").type);

}

TEST(InterpreterTest, FunctionCall) {

    FungiInterpreter interpreter = FungiInterpreter();
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

    Either<SporeError, SporeDataVariant> errorOrResult = interpreter.currentResult;
    if(errorOrResult.isLeft())
        std::cout << "error returned: " << errorOrResult.getLeft().message<<"\n";
    ASSERT_TRUE(errorOrResult.isRight());
    SporeDataVariant result = errorOrResult.getRight();
    ASSERT_EQ(SporeDataVariant::t_unknown, result.type);

    ASSERT_EQ(SporeDataVariant::t_int, interpreter.callStack.peek().get("hannah").type);
    ASSERT_EQ(30, interpreter.callStack.peek().get("hannah").as_integer);

}
