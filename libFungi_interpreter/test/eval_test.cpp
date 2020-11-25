#include <gtest/gtest.h>
#include "../src/eval.h"

TEST(EvalTest, FunctionalTest_Multiplication){
    Either<SporeError,Signal<double,int>> result = _eval("5*4\n\n");

    ASSERT_TRUE(result.isLeft());
    ASSERT_EQ("final return value not a Signal...was INT instead.",result.getLeft().message);

}

TEST(EvalTest, FunctionalTest_Assignment){
    Either<SporeError,Signal<double,int>> result = _eval("fred = 5*3\nfred\n\n\n");

    ASSERT_TRUE(result.isRight());

}


TEST(EvalTest, FunctionalTest_Algae_sinOsc){
    Either<SporeError,Signal<double,int>> result = _eval("osc(440.0)\n\n");

    ASSERT_TRUE(result.isRight());

}
