#include <gtest/gtest.h>
#include "../src/interpreter/interpreter.h"
#include "../src/lang/sporelang_syntaxtree.h"
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include "../src/interpreter/data_varient.h"
#include "error.h"
#include <iostream>

typedef SporeDataVariant SPV;
// typedef SporeDataVariant<int,double,std::string,std::function<double(int)>> SPV;

TEST(DataVariantTest, IsTaggedUnionOfTemplateArgs) {
    std::vector<SporeDataVariant> foo(
        {
            SporeDataVariant(0),
            SporeDataVariant(1.1),
            SporeDataVariant(true),
            SporeDataVariant("goob"),
            SporeDataVariant(std::string("goober")),
            SporeDataVariant(std::vector<SporeDataVariant>({SporeDataVariant(42)})),
            SporeDataVariant(Signal<double,int>(std::function<double(int)>([](int t){return 0;}))),
            SporeDataVariant(std::function<Either<SporeError,SporeDataVariant>(SporeDataVariant)>([](const SporeDataVariant& f){
                return Either<SporeError,SporeDataVariant>::rightOf(f);
            }))

        }
    );
    ASSERT_EQ(8, foo.size());
    EXPECT_EQ(SporeDataVariant::t_int ,foo[0].type);
    EXPECT_EQ(0 ,foo[0].as_integer);
    EXPECT_EQ(SporeDataVariant::t_float ,foo[1].type);
    EXPECT_EQ(1.1 ,foo[1].as_float);
    EXPECT_EQ(SporeDataVariant::t_bool ,foo[2].type);
    EXPECT_EQ(true ,foo[2].as_bool);
    EXPECT_EQ(SporeDataVariant::t_string ,foo[3].type);
    EXPECT_EQ("goob" ,foo[3].as_string);
    EXPECT_EQ(SporeDataVariant::t_string ,foo[4].type);
    EXPECT_EQ("goober" ,foo[4].as_string);
    EXPECT_EQ(SporeDataVariant::t_list ,foo[5].type);
    EXPECT_EQ(1 ,foo[5].as_list.size());
    EXPECT_EQ(SporeDataVariant::t_int ,foo[5].as_list[0].type);
    EXPECT_EQ(42 ,foo[5].as_list[0].as_integer);
    EXPECT_EQ(SporeDataVariant::t_signal ,foo[6].type);
    EXPECT_EQ(0, foo[6].as_signal(0));
    EXPECT_EQ(SporeDataVariant::t_lambda ,foo[7].type);
    EXPECT_TRUE(foo[7].as_lambda(SporeDataVariant(0)).isRight());
    EXPECT_EQ(0 ,foo[7].as_lambda(SporeDataVariant(0)).getRight().as_integer);

    // std::unordered_map<SporeDataVariant,SporeDataVariant,FakeHash> myMap({SporeDataVariant("goob"),SporeDataVariant(42)});
}




TEST(DataVariantTest, Properties_AndStaticAsserts){
    EXPECT_TRUE(std::is_move_constructible<SporeDataVariant>::value);
    EXPECT_TRUE(std::is_move_assignable<SporeDataVariant>::value);
    // EXPECT_TRUE(std::is_nothrow_move_assignable<SporeDataVariant>::value);

    EXPECT_TRUE(std::is_copy_assignable<SporeDataVariant>::value);
    EXPECT_TRUE(std::is_copy_constructible<SporeDataVariant>::value);
    EXPECT_TRUE(std::is_default_constructible<SporeDataVariant>::value);
    EXPECT_TRUE(std::is_destructible<SporeDataVariant>::value);


}


TEST(DataVariantTest, CopyConstructor){
    SPV int1 = SPV(1);
    SPV int2 = SPV(int1);
    ASSERT_EQ(SPV::t_int,int1.type);
    ASSERT_EQ(SPV::t_int,int2.type);
    ASSERT_EQ(int1.as_integer,int2.as_integer);

    SPV float1 = SPV(1.22);
    SPV float2 = float1;
    ASSERT_EQ(SPV::t_float,float1.type);
    ASSERT_EQ(SPV::t_float,float2.type);
    ASSERT_EQ(float1.as_float,float2.as_float);

    SPV string1 = SPV("goober");
    SPV string2 = string1;
    ASSERT_EQ(SPV::t_string,string1.type);
    ASSERT_EQ(SPV::t_string,string2.type);
    ASSERT_EQ(string1.as_string,string2.as_string);

    SPV signal1 = SPV(Signal<double,int>(std::function<double(int)>([](int t){return 5.4;})));
    SPV signal2 = signal1;
    ASSERT_EQ(SPV::t_signal,signal1.type);
    ASSERT_EQ(SPV::t_signal,signal2.type);
    ASSERT_EQ(signal1.as_signal(0),signal2.as_signal(0));

    SPV list1 = SPV(std::vector<SPV>({int1,string1,signal1}));
    SPV list2 = list1;
    ASSERT_EQ(SPV::t_list,list1.type);
    ASSERT_EQ(SPV::t_list,list2.type);
    ASSERT_EQ(list1.as_list.size(),list2.as_list.size());
    ASSERT_EQ(3, list1.as_list.size());
    ASSERT_EQ(list1.as_list[0].as_integer,list2.as_list[0].as_integer);
    ASSERT_EQ(list1.as_list[1].as_string,list2.as_list[1].as_string);
    ASSERT_EQ(list1.as_list[2].as_signal(0),list2.as_list[2].as_signal(0));
    // SPV map1 = SPV(std::map<SPV, SPV>({string1,float2}));
    // SPV map2 = map1;
    // ASSERT_EQ(SPV::t_map, map1.type);
    // ASSERT_EQ(SPV::t_map, map2.type);
    // ASSERT_EQ(map1.as_map.size(), map2.as_map.size());
    // ASSERT_EQ(map1.as_map[string1].as_float, map2.as_map[string1].as_float);

}


TEST(DataVariantTest, CanBePlacedSafelyInContainers){
    std::vector<SporeDataVariant> list = std::vector<SporeDataVariant>({SporeDataVariant(0),SporeDataVariant("goob")});
    list.push_back(SporeDataVariant(42));
    list.push_back(SporeDataVariant("goob"));
    SporeDataVariant lvalData = SporeDataVariant("im using a var");
    list.push_back(lvalData);

    EXPECT_EQ(5, list.size());
    ASSERT_EQ(SporeDataVariant::t_string, list[4].type);
    ASSERT_EQ(lvalData.type, list[4].type);
    EXPECT_EQ(lvalData.as_string, list[4].as_string);

    std::map<std::string, SporeDataVariant> amap = std::map<std::string, SporeDataVariant>();
    amap["poo"] = lvalData;
    ASSERT_EQ(SporeDataVariant::t_string, amap["poo"].type);
    EXPECT_EQ(lvalData.type, amap["poo"].type);
    EXPECT_EQ("im using a var", amap["poo"].as_string);
    EXPECT_EQ(lvalData.as_string, amap["poo"].as_string);
}

TEST(DataVariantTest, Multiply){
    Either<SporeError, SporeDataVariant> intAndInt = SporeDataVariant(5)*SporeDataVariant(4);
    ASSERT_TRUE(intAndInt.isRight());
    ASSERT_EQ(SporeDataVariant::t_int, intAndInt.getRight().type);
    ASSERT_EQ(20, intAndInt.getRight().as_integer);


    Either<SporeError, SporeDataVariant> intAndFloat = SporeDataVariant(5)*SporeDataVariant(4.0);
    ASSERT_TRUE(intAndFloat.isRight());
    ASSERT_EQ(SporeDataVariant::t_float, intAndFloat.getRight().type);
    ASSERT_EQ(20.0, intAndFloat.getRight().as_float);
    

    Either<SporeError, SporeDataVariant> floatAndInt = SporeDataVariant(5.0)*SporeDataVariant(4);
    ASSERT_TRUE(floatAndInt.isRight());
    ASSERT_EQ(SporeDataVariant::t_int, floatAndInt.getRight().type);
    ASSERT_EQ(20, floatAndInt.getRight().as_integer);

    Either<SporeError, SporeDataVariant> intAndBool = SporeDataVariant(5)*SporeDataVariant(false);
    ASSERT_TRUE(intAndBool.isRight());
    ASSERT_EQ(SporeDataVariant::t_int, intAndBool.getRight().type);
    ASSERT_EQ(0, intAndBool.getRight().as_integer);

    Either<SporeError, SporeDataVariant> boolAndInt = SporeDataVariant(true)*SporeDataVariant(4);
    ASSERT_TRUE(boolAndInt.isRight());
    ASSERT_EQ(SporeDataVariant::t_int, boolAndInt.getRight().type);
    ASSERT_EQ(4, boolAndInt.getRight().as_integer);

    Either<SporeError, SporeDataVariant> intAndString = SporeDataVariant(5)*SporeDataVariant("Blood");
    ASSERT_TRUE(intAndString.isLeft());

    Either<SporeError, SporeDataVariant> stringAndInt = SporeDataVariant("Blood")*SporeDataVariant(4);
    ASSERT_TRUE(stringAndInt.isLeft());

    Either<SporeError, SporeDataVariant> listInt = SporeDataVariant(4)*SporeDataVariant(std::vector<SporeDataVariant>{"fred",5});
    ASSERT_TRUE(listInt.isLeft());


    Either<SporeError, SporeDataVariant> intList = SporeDataVariant(std::vector<SporeDataVariant>{"fred",5})*SporeDataVariant(5);
    ASSERT_TRUE(intList.isLeft());

    // ASSERT_EQ(SporeDataVariant::t_list, intList.type);
    // ASSERT_EQ(2, intList.as_list.size());
    // ASSERT_EQ(SporeDataVariant::t_string, intList.as_list[0].type);
    // ASSERT_EQ("fred", intList.as_list[0].as_string);
    // ASSERT_EQ(SporeDataVariant::t_int, intList.as_list[1].type);
    // ASSERT_EQ(20, intList.as_list[1].as_integer);

    Either<SporeError, SporeDataVariant> intSignal = SporeDataVariant(5)*SporeDataVariant(std::function<double(int)>([](int t){return 4.0;}));
    ASSERT_TRUE(intSignal.isLeft());
    // ASSERT_EQ(SporeDataVariant::t_signal, intSignal.type);
    // ASSERT_EQ(20.0, intSignal.as_signal(77));
    // ASSERT_EQ(20.0, intSignal.as_signal(7));
    
    Either<SporeError, SporeDataVariant> signalInt = SporeDataVariant(std::function<double(int)>([](int t){return 5.0;}))*SporeDataVariant(4);
    ASSERT_TRUE(signalInt.isLeft());
    // ASSERT_EQ(SporeDataVariant::t_signal, intSignal.type);
    // ASSERT_EQ(20.0, signalInt.as_signal(77));
    // ASSERT_EQ(20.0, signalInt.as_signal(7));

    Either<SporeError, SporeDataVariant> intLambda = SporeDataVariant(5)*SporeDataVariant(std::function<SporeDataVariant(SporeDataVariant)>([](const SporeDataVariant& t){return SporeDataVariant(4.0);}));
    ASSERT_TRUE(intLambda.isLeft());
    // ASSERT_EQ(SporeDataVariant::t_lambda, intLambda.type);
    // ASSERT_EQ(20.0, intLambda.as_lambda(77).as_float);
    // ASSERT_EQ(20.0, intLambda.as_lambda(7).as_float);
    
    Either<SporeError, SporeDataVariant> lambdaInt = SporeDataVariant(std::function<SporeDataVariant(SporeDataVariant)>([](const SporeDataVariant& t){return SporeDataVariant(5.0);}))*SporeDataVariant(4);
    ASSERT_TRUE(intLambda.isLeft());
    // ASSERT_EQ(SporeDataVariant::t_lambda, intLambda.type);
    // ASSERT_EQ(20.0, lambdaInt.as_lambda(77).as_float);
    // ASSERT_EQ(20.0, lambdaInt.as_lambda(7).as_float);
    
}
