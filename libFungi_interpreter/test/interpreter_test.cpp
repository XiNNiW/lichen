#include <gtest/gtest.h>
#include "../src/interpreter/interpreter.h"
#include "../src/lang/sporelang_syntaxtree.h"

TEST(InterpreterTest, BinaryOp_Multiply) {
    FungiInterpreter* interpreter = new FungiInterpreter();
    MultiplyOperatorNode* multiplicationInstruction 
        = new MultiplyOperatorNode(
            new IntegerNode(5),
            new IntegerNode(3)
        );
    interpreter->inspect(multiplicationInstruction);
    ASSERT_EQ(0, 1);
    delete interpreter, multiplicationInstruction;
}
