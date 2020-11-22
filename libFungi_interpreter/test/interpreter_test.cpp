#include <gtest/gtest.h>
#include "../src/interpreter/interpreter.h"
#include "../src/lang/sporelang_syntaxtree.h"
#include "../src/interpreter/data_varient.h"

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
