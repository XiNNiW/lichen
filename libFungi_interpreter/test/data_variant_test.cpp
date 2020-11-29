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
    EXPECT_EQ(SporeDataType::t_int ,foo[0].type);
    EXPECT_EQ(0 ,foo[0].as_integer);
    EXPECT_EQ(SporeDataType::t_float ,foo[1].type);
    EXPECT_EQ(1.1 ,foo[1].as_float);
    EXPECT_EQ(SporeDataType::t_bool ,foo[2].type);
    EXPECT_EQ(true ,foo[2].as_bool);
    EXPECT_EQ(SporeDataType::t_string ,foo[3].type);
    EXPECT_EQ("goob" ,foo[3].as_string);
    EXPECT_EQ(SporeDataType::t_string ,foo[4].type);
    EXPECT_EQ("goober" ,foo[4].as_string);
    EXPECT_EQ(SporeDataType::t_list ,foo[5].type);
    EXPECT_EQ(1 ,foo[5].as_list.size());
    EXPECT_EQ(SporeDataType::t_int ,foo[5].as_list[0].type);
    EXPECT_EQ(42 ,foo[5].as_list[0].as_integer);
    EXPECT_EQ(SporeDataType::t_signal ,foo[6].type);
    EXPECT_EQ(0, foo[6].as_signal(0));
    EXPECT_EQ(SporeDataType::t_lambda ,foo[7].type);
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
    ASSERT_EQ(SporeDataType::t_int,int1.type);
    ASSERT_EQ(SporeDataType::t_int,int2.type);
    ASSERT_EQ(int1.as_integer,int2.as_integer);

    SPV float1 = SPV(1.22);
    SPV float2 = float1;
    ASSERT_EQ(SporeDataType::t_float,float1.type);
    ASSERT_EQ(SporeDataType::t_float,float2.type);
    ASSERT_EQ(float1.as_float,float2.as_float);

    SPV string1 = SPV("goober");
    SPV string2 = string1;
    ASSERT_EQ(SporeDataType::t_string,string1.type);
    ASSERT_EQ(SporeDataType::t_string,string2.type);
    ASSERT_EQ(string1.as_string,string2.as_string);

    SPV signal1 = SPV(Signal<double,int>(std::function<double(int)>([](int t){return 5.4;})));
    SPV signal2 = signal1;
    ASSERT_EQ(SporeDataType::t_signal,signal1.type);
    ASSERT_EQ(SporeDataType::t_signal,signal2.type);
    ASSERT_EQ(signal1.as_signal(0),signal2.as_signal(0));

    SPV list1 = SPV(std::vector<SPV>({int1,string1,signal1}));
    SPV list2 = list1;
    ASSERT_EQ(SporeDataType::t_list,list1.type);
    ASSERT_EQ(SporeDataType::t_list,list2.type);
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
    ASSERT_EQ(SporeDataType::t_string, list[4].type);
    ASSERT_EQ(lvalData.type, list[4].type);
    EXPECT_EQ(lvalData.as_string, list[4].as_string);

    std::map<std::string, SporeDataVariant> amap = std::map<std::string, SporeDataVariant>();
    amap["poo"] = lvalData;
    ASSERT_EQ(SporeDataType::t_string, amap["poo"].type);
    EXPECT_EQ(lvalData.type, amap["poo"].type);
    EXPECT_EQ("im using a var", amap["poo"].as_string);
    EXPECT_EQ(lvalData.as_string, amap["poo"].as_string);
}

void checkMath(SporeDataVariant expected, Either<SporeError,SporeDataVariant> actual){
    ASSERT_TRUE(actual.isRight());
    ASSERT_EQ(expected.type, actual.getRight().type);
    ASSERT_EQ(expected, actual.getRight());
}

void checkSignalMath(double expectedAtZero, Either<SporeError,SporeDataVariant> actual){
    ASSERT_TRUE(actual.isRight());
    ASSERT_EQ(SporeDataType::t_signal, actual.getRight().type);
    ASSERT_EQ(expectedAtZero, actual.getRight().as_signal._sig(0));
}

void checkError(std::string expectedError, Either<SporeError,SporeDataVariant> actual){
    ASSERT_TRUE(actual.isLeft());
    ASSERT_EQ(expectedError, actual.getLeft().message);
}

TEST(DataVariantTest, Addition){
    using SPV = SporeDataVariant;
    using E = Either<SporeError,SporeDataVariant>;
    checkMath(SPV(9),SPV(5)+SPV(4));
    checkMath(SPV(9.0),SPV(5)+SPV(4.0));
    checkMath(SPV(9),SPV(5.0)+SPV(4));
    checkMath(SPV(5),SPV(5)+SPV(false));
    checkMath(SPV(5),SPV(true)+SPV(4));
    checkMath(SPV(true),SPV(true)+SPV(false));

    auto sig = SporeDataVariant(std::function<double(int)>([](int t){return 4.0;}));
    checkSignalMath(9.0,SporeDataVariant(5)+sig);
    checkSignalMath(9.0,SporeDataVariant(5.0)+sig);
    checkSignalMath(9.0,SporeDataVariant(std::function<double(int)>([](int t){return 5.0;}))+SporeDataVariant(4));
    checkSignalMath(9.0,SporeDataVariant(std::function<double(int)>([](int t){return 5.0;}))+SporeDataVariant(4.0));
    checkSignalMath(0.0,SporeDataVariant(std::function<double(int)>([](int t){return 5.0;}))+SporeDataVariant(false));

    checkError("Not Supported", SPV(5)+SPV("Blood"));
    checkError("Not Supported", SPV(5.0)+SPV("Blood"));
    checkError("Not Supported", SPV(false)+SPV("Blood"));
    checkError("Not Supported", SPV("Blood")+SPV(4));
    checkError("Not Supported", SPV("Blood")+SPV(4.0));
    checkError("Not Supported", SPV("Blood")+SPV(false));
    checkError("Not Supported", SPV(4)+SPV(std::vector<SPV>{"fred",5}));
    checkError("Not Supported", SPV(4.0)+SPV(std::vector<SPV>{"fred",5}));
    checkError("Not Supported", SPV(false)+SPV(std::vector<SPV>{"fred",5}));
    checkError("Not Supported", SPV(std::vector<SPV>{"fred",5})+SPV(5));
    auto lambda = SPV(std::function<E(SPV)>([](const SPV& t){return E(SPV(4.0));}));
    checkError("Not Supported", sig+lambda);
    checkError("Not Supported", lambda+sig);
    checkError("Not Supported", SPV(5)+lambda);
    checkError("Not Supported", SPV(5.0)+lambda);
    checkError("Not Supported", SPV(false)+lambda);
    checkError("Not Supported", lambda+SPV(4.0));
    checkError("Not Supported", lambda+SPV(4));
    checkError("Not Supported", lambda+SPV(false));
   
}

TEST(DataVariantTest, Subtraction){
    using SPV = SporeDataVariant;
    using E = Either<SporeError,SporeDataVariant>;
    checkMath(SPV(1),SPV(5)-SPV(4));
    checkMath(SPV(1.0),SPV(5)-SPV(4.0));
    checkMath(SPV(1),SPV(5.0)-SPV(4));
    checkMath(SPV(5),SPV(5)-SPV(false));
    checkMath(SPV(-3),SPV(true)-SPV(4));
    checkMath(SPV(true),SPV(true)-SPV(false));

    auto sig = SporeDataVariant(std::function<double(int)>([](int t){return 4.0;}));
    checkSignalMath(1.0,SporeDataVariant(5)-sig);
    checkSignalMath(1.0,SporeDataVariant(5.0)-sig);
    checkSignalMath(1.0,SporeDataVariant(std::function<double(int)>([](int t){return 5.0;}))-SporeDataVariant(4));
    checkSignalMath(1.0,SporeDataVariant(std::function<double(int)>([](int t){return 5.0;}))-SporeDataVariant(4.0));
    checkSignalMath(0.0,SporeDataVariant(std::function<double(int)>([](int t){return 5.0;}))-SporeDataVariant(false));

    checkError("Not Supported", SPV(5)-SPV("Blood"));
    checkError("Not Supported", SPV(5.0)-SPV("Blood"));
    checkError("Not Supported", SPV(false)-SPV("Blood"));
    checkError("Not Supported", SPV("Blood")-SPV(4));
    checkError("Not Supported", SPV("Blood")-SPV(4.0));
    checkError("Not Supported", SPV("Blood")-SPV(false));
    checkError("Not Supported", SPV(4)-SPV(std::vector<SPV>{"fred",5}));
    checkError("Not Supported", SPV(4.0)-SPV(std::vector<SPV>{"fred",5}));
    checkError("Not Supported", SPV(false)-SPV(std::vector<SPV>{"fred",5}));
    checkError("Not Supported", SPV(std::vector<SPV>{"fred",5})-SPV(5));
    auto lambda = SPV(std::function<E(SPV)>([](const SPV& t){return E(SPV(4.0));}));
    checkError("Not Supported", sig-lambda);
    checkError("Not Supported", lambda-sig);
    checkError("Not Supported", SPV(5)-lambda);
    checkError("Not Supported", SPV(5.0)-lambda);
    checkError("Not Supported", SPV(false)-lambda);
    checkError("Not Supported", lambda-SPV(4.0));
    checkError("Not Supported", lambda-SPV(4));
    checkError("Not Supported", lambda-SPV(false));
   
}


TEST(DataVariantTest, Multiplication){
    using SPV = SporeDataVariant;
    using E = Either<SporeError,SporeDataVariant>;
    checkMath(SPV(20),SPV(5)*SPV(4));
    checkMath(SPV(20.0),SPV(5)*SPV(4.0));
    checkMath(SPV(20),SPV(5.0)*SPV(4));
    checkMath(SPV(0),SPV(5)*SPV(false));
    checkMath(SPV(4),SPV(true)*SPV(4));
    checkMath(SPV(false),SPV(true)*SPV(false));

    auto sig = SporeDataVariant(std::function<double(int)>([](int t){return 4.0;}));
    checkSignalMath(20.0,SporeDataVariant(5)*sig);
    checkSignalMath(20.0,SporeDataVariant(5.0)*sig);
    checkSignalMath(20.0,SporeDataVariant(std::function<double(int)>([](int t){return 5.0;}))*SporeDataVariant(4));
    checkSignalMath(20.0,SporeDataVariant(std::function<double(int)>([](int t){return 5.0;}))*SporeDataVariant(4.0));
    checkSignalMath(0.0,SporeDataVariant(std::function<double(int)>([](int t){return 5.0;}))*SporeDataVariant(false));

    checkError("Not Supported", SPV(5)*SPV("Blood"));
    checkError("Not Supported", SPV(5.0)*SPV("Blood"));
    checkError("Not Supported", SPV(false)*SPV("Blood"));
    checkError("Not Supported", SPV("Blood")*SPV(4));
    checkError("Not Supported", SPV("Blood")*SPV(4.0));
    checkError("Not Supported", SPV("Blood")*SPV(false));
    checkError("Not Supported", SPV(4)*SPV(std::vector<SPV>{"fred",5}));
    checkError("Not Supported", SPV(4.0)*SPV(std::vector<SPV>{"fred",5}));
    checkError("Not Supported", SPV(false)*SPV(std::vector<SPV>{"fred",5}));
    checkError("Not Supported", SPV(std::vector<SPV>{"fred",5})*SPV(5));
    auto lambda = SPV(std::function<E(SPV)>([](const SPV& t){return E(SPV(4.0));}));
    checkError("Not Supported", sig*lambda);
    checkError("Not Supported", lambda*sig);
    checkError("Not Supported", SPV(5)*lambda);
    checkError("Not Supported", SPV(5.0)*lambda);
    checkError("Not Supported", SPV(false)*lambda);
    checkError("Not Supported", lambda*SPV(4.0));
    checkError("Not Supported", lambda*SPV(4));
    checkError("Not Supported", lambda*SPV(false));
   
}


TEST(DataVariantTest, Division){
    using SPV = SporeDataVariant;
    using E = Either<SporeError,SporeDataVariant>;
    checkMath(SPV(5),SPV(10)/SPV(2));
    checkMath(SPV(5.0),SPV(10)/SPV(2.0));
    checkMath(SPV(5),SPV(10.0)/SPV(2));
    checkMath(SPV(0),SPV(10)/SPV(false));
    checkMath(SPV(0),SPV(true)/SPV(2));
    checkMath(SPV(false),SPV(true)/SPV(false));

    auto sig = SporeDataVariant(std::function<double(int)>([](int t){return 2.0;}));
    checkSignalMath(5.0,SporeDataVariant(10)/sig);
    checkSignalMath(5.0,SporeDataVariant(10.0)/sig);
    checkSignalMath(5.0,SporeDataVariant(std::function<double(int)>([](int t){return 10.0;}))/SporeDataVariant(2));
    checkSignalMath(5.0,SporeDataVariant(std::function<double(int)>([](int t){return 10.0;}))/SporeDataVariant(2.0));
    checkSignalMath(0.0,SporeDataVariant(std::function<double(int)>([](int t){return 10.0;}))/SporeDataVariant(false));

    checkError("Not Supported", SPV(10)/SPV("Blood"));
    checkError("Not Supported", SPV(10.0)/SPV("Blood"));
    checkError("Not Supported", SPV(false)/SPV("Blood"));
    checkError("Not Supported", SPV("Blood")/SPV(2));
    checkError("Not Supported", SPV("Blood")/SPV(2.0));
    checkError("Not Supported", SPV("Blood")/SPV(false));
    checkError("Not Supported", SPV(2)/SPV(std::vector<SPV>{"fred",10}));
    checkError("Not Supported", SPV(2.0)/SPV(std::vector<SPV>{"fred",10}));
    checkError("Not Supported", SPV(false)/SPV(std::vector<SPV>{"fred",10}));
    checkError("Not Supported", SPV(std::vector<SPV>{"fred",10})/SPV(10));
    auto lambda = SPV(std::function<E(SPV)>([](const SPV& t){return E(SPV(2.0));}));
    checkError("Not Supported", sig/lambda);
    checkError("Not Supported", lambda/sig);
    checkError("Not Supported", SPV(10)/lambda);
    checkError("Not Supported", SPV(10.0)/lambda);
    checkError("Not Supported", SPV(false)/lambda);
    checkError("Not Supported", lambda/SPV(2.0));
    checkError("Not Supported", lambda/SPV(2));
    checkError("Not Supported", lambda/SPV(false));
   
}









