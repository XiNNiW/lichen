#include <gtest/gtest.h>
#include "../src/interpreter/interpreter.h"
#include "../src/lang/sporelang_syntaxtree.h"
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include "../src/interpreter/data_varient.h"
#include <iostream>

typedef SporeDataVariant SPV;
// typedef SporeDataVariant<int,double,std::string,std::function<double(int)>> SPV;

TEST(VarientTest, IsTaggedUnionOfTemplateArgs) {
    std::vector<SporeDataVariant> foo(
        {
            SporeDataVariant(0),
            SporeDataVariant(1.1),
            SporeDataVariant(true),
            SporeDataVariant("goob"),
            SporeDataVariant(std::string("goober")),
            SporeDataVariant(std::vector<SporeDataVariant>({SporeDataVariant(42)})),
            SporeDataVariant(Signal<double,int>(std::function<double(int)>([](int t){return 0;}))),
            SporeDataVariant(std::function<SporeDataVariant(SporeDataVariant)>([](const SporeDataVariant& f){return f;}))

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
    EXPECT_EQ(0 ,foo[7].as_lambda(SporeDataVariant(0)).as_integer);

    // std::unordered_map<SporeDataVariant,SporeDataVariant,FakeHash> myMap({SporeDataVariant("goob"),SporeDataVariant(42)});
}




TEST(VarientTest, Properties_AndStaticAsserts){
    EXPECT_TRUE(std::is_move_constructible<SporeDataVariant>::value);
    EXPECT_TRUE(std::is_move_assignable<SporeDataVariant>::value);
    // EXPECT_TRUE(std::is_nothrow_move_assignable<SporeDataVariant>::value);

    EXPECT_TRUE(std::is_copy_assignable<SporeDataVariant>::value);
    EXPECT_TRUE(std::is_copy_constructible<SporeDataVariant>::value);
    EXPECT_TRUE(std::is_default_constructible<SporeDataVariant>::value);
    EXPECT_TRUE(std::is_destructible<SporeDataVariant>::value);


}


TEST(VarientTest, CopyConstructor){
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

