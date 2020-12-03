#include <gtest/gtest.h>
#include "../src/maybe.h"

// using Monad::Maybe;
// using Monad::Just;
// using Monad::map;
// using Monad::flatMap;
// using Monad::Nothing;
// using Monad::isNothing;
// using Monad::isSomthing;
// using Monad::getValue;
// using Monad::getOrElse;

TEST(MaybeTest_classAPI, NothingShouldContainNull) { 
    Maybe<int> maybeNothing = Maybe<int>::nothing();

    ASSERT_TRUE(maybeNothing.isNothing());

    Maybe<std::string> alsoNothing = Maybe<std::string>::nothing();

    ASSERT_TRUE(alsoNothing.isNothing());
}


TEST(MaybeTest_classAPI, MapOperatesIfSomthingReturnsMaybe){
    auto plus3 = [](int v) {return v+3;};
    Maybe<int> somthing = Maybe<int>::of(5);
    Maybe<int> maybeEight = somthing.map(plus3);
    ASSERT_EQ(5,somthing.getValue());
    ASSERT_FALSE(maybeEight.isNothing());
    ASSERT_EQ(8, maybeEight.getValue());

    Maybe<int> nothing = Maybe<int>::nothing();
    Maybe<int> alsoNothing = nothing.map(plus3);
    ASSERT_TRUE(nothing.isNothing());
    ASSERT_TRUE(alsoNothing.isNothing());
}

TEST(MaybeTest_classAPI, MapOperatesIfSomthingReturnsMaybe_std_function){
    Maybe<int> somthing = Maybe<int>::of(5);
    Maybe<int> maybeEight = somthing.map(std::function<int(int)>([](int v)->int{return v+3; }));
    ASSERT_EQ(5,somthing.getValue());
    ASSERT_FALSE(maybeEight.isNothing());
    ASSERT_EQ(8, maybeEight.getValue());

    Maybe<int> nothing = Maybe<int>::nothing();
    Maybe<int> alsoNothing = nothing.map(std::function<int(int)>([](int v)->int{return v+3; }));
    ASSERT_TRUE(nothing.isNothing());
    ASSERT_TRUE(alsoNothing.isNothing());
}

TEST(MaybeTest_classAPI, FlatMapFlattensMaybes){
    auto maybePlus3 = [](int a){return Maybe<int>::of(a+3);};
    Maybe<int> somthing = Maybe<int>::of(5);
    Maybe<int> maybeEight = somthing.flatMap(maybePlus3);
    ASSERT_EQ(5, somthing.getValue());
    ASSERT_EQ(8, maybeEight.getValue());

    Maybe<int> nothing = Maybe<int>::nothing();
    Maybe<int> alsoNothing = nothing.flatMap(maybePlus3);
    ASSERT_TRUE(nothing.isNothing());
    ASSERT_TRUE(alsoNothing.isNothing());

}

TEST(MaybeTest_classAPI, FlatMapFlattensMaybes_std_function){
    Maybe<int> somthing = Maybe<int>::of(5);
    Maybe<int> maybeEight = somthing.flatMap(std::function<Maybe<int>(int)>([](int a)->Maybe<int>{
        return Maybe<int>::of(a+3);
    }));
    ASSERT_EQ(5, somthing.getValue());
    ASSERT_EQ(8, maybeEight.getValue());

    Maybe<int> nothing = Maybe<int>::nothing();
    Maybe<int> alsoNothing = nothing.flatMap(std::function<Maybe<int>(int)>([](int a)->Maybe<int>{
        return Maybe<int>::of(a+3);
    }));
    ASSERT_TRUE(nothing.isNothing());
    ASSERT_TRUE(alsoNothing.isNothing());

}

TEST(MaybeTest_classAPI, GetOrElse){
    Maybe<std::string> fish = Maybe<std::string>::of("fish");
    ASSERT_EQ("fish", fish.getOrElse("bird"));

    Maybe<std::string> notFish = Maybe<std::string>::nothing();
    ASSERT_EQ("bird", notFish.getOrElse("bird"));

}

// 2nd api

TEST(MaybeTest, NothingShouldContainNull) { 

    Maybe<int> maybeNothing = Nothing<int>();

    ASSERT_TRUE(isNothing(maybeNothing));

    Maybe<std::string> alsoNothing = Nothing<std::string>();

    ASSERT_TRUE(isNothing(alsoNothing));
}

TEST(Maybe, MapOperatesIfSomthingReturnsMaybe){
    
    auto plus3 = [](int v) {return v+3;};
    Maybe<int> somthing = Just(5);
    Maybe<int> maybeEight = map(somthing, plus3);
    ASSERT_EQ(5,getValue(somthing));
    ASSERT_FALSE(isNothing(maybeEight));
    ASSERT_EQ(8, getValue(maybeEight));

    Maybe<int> nothing = Nothing<int>();
    Maybe<int> alsoNothing = map(nothing, plus3);
    ASSERT_TRUE(isNothing(nothing));
    ASSERT_TRUE(isNothing(alsoNothing));
}

TEST(Maybe, MapOperatesIfSomthingReturnsMaybe_std_function){
    Maybe<int> somthing = Just(5);
    Maybe<int> maybeEight = map(somthing, std::function<int(int)>([](int v)->int{return v+3; }));
    ASSERT_EQ(5,getValue(somthing));
    ASSERT_FALSE(isNothing(maybeEight));
    ASSERT_EQ(8, getValue(maybeEight));

    Maybe<int> nothing = Nothing<int>();
    Maybe<int> alsoNothing = map(nothing, std::function<int(int)>([](int v)->int{return v+3; }));
    ASSERT_TRUE(isNothing(nothing));
    ASSERT_TRUE(isNothing(alsoNothing));
}

TEST(Maybe, FlatMapFlattensMaybes){
    auto maybePlus3 = [](int a){return Just(a+3);};
    Maybe<int> somthing = Just(5);
    Maybe<int> maybeEight = flatMap(somthing, maybePlus3);
    ASSERT_EQ(5, getValue(somthing));
    ASSERT_EQ(8, getValue(maybeEight));

    Maybe<int> nothing = Nothing<int>();
    Maybe<int> alsoNothing = flatMap(nothing, maybePlus3);
    ASSERT_TRUE(isNothing(nothing));
    ASSERT_TRUE(isNothing(alsoNothing));

}

TEST(Maybe, FlatMapFlattensMaybes_std_function){
    Maybe<int> somthing = Just(5);
    Maybe<int> maybeEight = flatMap(somthing, std::function<Maybe<int>(int)>([](int a)->Maybe<int>{
        return Just(a+3);
    }));
    ASSERT_EQ(5, getValue(somthing));
    ASSERT_EQ(8, getValue(maybeEight));

    Maybe<int> nothing = Nothing<int>();
    Maybe<int> alsoNothing = flatMap(nothing, std::function<Maybe<int>(int)>([](int a)->Maybe<int>{
        return Just(a+3);
    }));
    ASSERT_TRUE(isNothing(nothing));
    ASSERT_TRUE(isNothing(alsoNothing));

}

TEST(Maybe, GetOrElse){
    Maybe<std::string> fish = Just(std::string("fish"));
    ASSERT_EQ("fish", getOrElse(fish, std::string("bird")));

    Maybe<std::string> notFish = Nothing<std::string>();
    ASSERT_EQ("bird", getOrElse(notFish, std::string("bird")));

}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}