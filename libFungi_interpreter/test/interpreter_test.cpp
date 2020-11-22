#include <gtest/gtest.h>
#include "../src/interpreter/interpreter.h"
#include "../src/lang/sporelang_syntaxtree.h"
#include "../src/interpreter/data_varient.h"
#include <iostream>
using std::cout;

TEST(InterpreterTest, Assignment_Expression) {

    FungiInterpreter* interpreter = new FungiInterpreter();
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

    Either<RuntimeError, SporeDataVariant> errorOrResult = interpreter->curretResult;
    ASSERT_TRUE(errorOrResult.isRight());
    SporeDataVariant resultSymbol = errorOrResult.getRight();
    ASSERT_EQ(SporeDataVariant::t_unknown, resultSymbol.type);

    // ASSERT_EQ(SporeDataVariant::t_data_variable, interpreter->globalScope.lookupInCurrentScopeOnly("foo").type)

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
