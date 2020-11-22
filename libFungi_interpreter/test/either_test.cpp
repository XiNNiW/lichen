#include <gtest/gtest.h>
#include "../src/interpreter/either.h"

TEST(EitherTest_classAPI, RightConstructsRight) { 
    Either<char,int> intOrString = Either<char,int>::rightOf(5);
    ASSERT_EQ(5, intOrString.getRight());

    ASSERT_TRUE(intOrString.isRight());
    ASSERT_FALSE(intOrString.isLeft());
}

TEST(EitherTest_classAPI, LeftConstructsLeft) { 
    Either<std::string, int> intOrString = Either<std::string, int>::leftOf("a");
    ASSERT_EQ("a", intOrString.getLeft());

    ASSERT_FALSE(intOrString.isRight());
    ASSERT_TRUE(intOrString.isLeft());
}

TEST(EitherTest_classAPI, FlatMapOperatesIfRight){
    Either<std::string,int> right = Either<std::string,int>::rightOf(5);
    Either<std::string,int> nextRight = right.flatMap([](int a){return Either<std::string,int>::rightOf(a+3);});
    ASSERT_TRUE(nextRight.isRight());
    ASSERT_FALSE(nextRight.isLeft());
    ASSERT_EQ(8,nextRight.getRight());
}

TEST(EitherTest_classAPI, FlatMapOperatesIfRight_stdFunction){
    Either<std::string,int> right = Either<std::string,int>::rightOf(5);
    Either<std::string,int> nextRight = right.flatMap(
        std::function<Either<std::string,int>(int)>([](int a){return Either<std::string,int>::rightOf(a+3);})
    );
    ASSERT_TRUE(nextRight.isRight());
    ASSERT_FALSE(nextRight.isLeft());
    ASSERT_EQ(8,nextRight.getRight());
}

TEST(EitherTest_classAPI, FlatPassesIfLeft){
    Either<std::string,int> left = Either<std::string,int>::leftOf("error");
    Either<std::string,int> sameLeft = left.flatMap([](int a){return Either<std::string,int>::rightOf(a+3);});
    ASSERT_FALSE(sameLeft.isRight());
    ASSERT_TRUE(sameLeft.isLeft());
    ASSERT_EQ(left.getLeft(),sameLeft.getLeft());
    ASSERT_EQ("error",sameLeft.getLeft());
}

TEST(EitherTest_classAPI, FlatPassesIfLeft_stdFunction){
    Either<std::string,int> left = Either<std::string,int>::leftOf("error");
    Either<std::string,int> sameLeft = left.flatMap(
        std::function<Either<std::string,int>(int)>([](int a){return Either<std::string,int>::rightOf(a+3);})
    );
    ASSERT_FALSE(sameLeft.isRight());
    ASSERT_TRUE(sameLeft.isLeft());
    ASSERT_EQ(left.getLeft(),sameLeft.getLeft());
    ASSERT_EQ("error",sameLeft.getLeft());
}

TEST(EitherTest, RightConstructsRight) { 
    Either<char,int> intOrString = Right<char,int>(5);
    ASSERT_EQ(5, getRight(intOrString));

    ASSERT_TRUE(isRight(intOrString));
    ASSERT_FALSE(isLeft(intOrString));
}

TEST(EitherTest, LeftConstructsLeft) { 
    Either<std::string, int> intOrString = Left<std::string, int>("a");
    ASSERT_EQ("a", getLeft(intOrString));

    ASSERT_FALSE(isRight(intOrString));
    ASSERT_TRUE(isLeft(intOrString));
}

TEST(EitherTest, FlatMapOperatesIfRight){
    Either<std::string,int> right = Right<std::string,int>(5);
    Either<std::string,int> nextRight = flatMap(right,[](int a){return Either<std::string,int>::rightOf(a+3);});
    ASSERT_TRUE(isRight(nextRight));
    ASSERT_FALSE(isLeft(nextRight));
    ASSERT_EQ(8,getRight(nextRight));
}

TEST(EitherTest, FlatMapOperatesIfRight_stdFunction){
    Either<std::string,int> right = Right<std::string,int>(5);
    Either<std::string,int> nextRight = flatMap(
        right,
        std::function<Either<std::string,int>(int)>([](int a){
            return Either<std::string,int>::rightOf(a+3);
        })
    );
    ASSERT_TRUE(isRight(nextRight));
    ASSERT_FALSE(isLeft(nextRight));
    ASSERT_EQ(8,getRight(nextRight));
}

TEST(EitherTest, FlatPassesIfLeft){
    Either<std::string,int> left = Left<std::string,int>("error");
    Either<std::string,int> sameLeft = flatMap(
        left,
        [](int a){
            return Either<std::string,int>::rightOf(a+3);
        }
    );
    ASSERT_FALSE(isRight(sameLeft));
    ASSERT_TRUE(isLeft(sameLeft));
    ASSERT_EQ(getLeft(left),getLeft(sameLeft));
    ASSERT_EQ("error",getLeft(sameLeft));
}

TEST(EitherTest, FlatPassesIfLeft_stdFunction){
    Either<std::string,int> left = Left<std::string,int>("error");
    Either<std::string,int> sameLeft = flatMap(
        left,
        std::function<Either<std::string,int>(int)>([](int a){
            return Either<std::string,int>::rightOf(a+3);
        })
    );
    ASSERT_FALSE(isRight(sameLeft));
    ASSERT_TRUE(isLeft(sameLeft));
    ASSERT_EQ(getLeft(left),getLeft(sameLeft));
    ASSERT_EQ("error",getLeft(sameLeft));
}


TEST(EitherTest, CopyCons){
    Either<std::string,int> right = Right<std::string,int>(5);
    right = flatMap(
        right,
        std::function<Either<std::string,int>(int)>([](int a){
            return Either<std::string,int>::rightOf(a+3);
        })
    );
}