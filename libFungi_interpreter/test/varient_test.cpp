#include <gtest/gtest.h>
#include "../src/interpreter/interpreter.h"
#include "../src/lang/sporelang_syntaxtree.h"
#include <string>
#include <vector>
#include <functional>
#include "../src/interpreter/data_varient.h"
#include <iostream>

typedef SporeDataVariant SPV;
// typedef SporeDataVariant<int,double,std::string,std::function<double(int)>> SPV;

// TEST(VarientTest, IsTaggedUnionOfTemplateArgs) {

//     SPV* v_i = new SPV(5);
//     ASSERT_EQ(SPV::t_int,v_i->type);
//     ASSERT_EQ(5,v_i->as_integer);

//     SPV* v_d = new SPV(5.3);
//     ASSERT_EQ(SPV::t_float,v_d->type);
//     ASSERT_EQ(5.3,v_d->as_float);

//     SPV* v_str = new SPV("hello");
//     ASSERT_EQ(SPV::t_string,v_str->type);
//     ASSERT_EQ("hello",v_str->as_string);


//     SPV* v_sig 
//         = new SPV(
//             std::function<double(int)>(
//                 [](int t){return 1.22;}
//             )
//         );


    
//     SPV* v_lam
//         = new SPV(
//             std::function<SPV(SPV)>(
//                 [](SPV arg){return SPV(3);}
//             )
//         );

    
//     // std::vector<SPV> expected_vector = std::vector<SPV>({SPV(9), SPV(9)});
//     SPV item1 = SPV("goober");

//     SPV item2 = SPV(9);

//     SPV item3 = item1;
//     std::cout << "made it halfway"<<std::endl;

//     // std::vector<SPV> expected_vector = std::vector<SPV>({item1, item2});
//     // SPV* v_list = new SPV(expected_vector);





   

//     ASSERT_EQ(SPV::t_signal,v_sig->type);
//     auto the_signal = v_sig->as_signal;
//     ASSERT_EQ(1.22,the_signal(0));

//     // ASSERT_EQ(SPV::t_list, v_list->type);
//     // std::vector<SPV> actual_list = v_list->as_list;
//     // ASSERT_EQ(2,actual_list.size());
//     // ASSERT_EQ(SPV::t_string,actual_list[0].type);
//     // ASSERT_EQ("goober",actual_list[0].as_string);
//     // ASSERT_EQ(SPV::t_int,actual_list[1].type);
//     // ASSERT_EQ(9,actual_list[1].as_integer);


// }

TEST(VarientTest, Properties){
    EXPECT_TRUE(std::is_move_constructible<SPV>::value);
    EXPECT_TRUE(std::is_move_assignable<SPV>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<SPV>::value);

    EXPECT_TRUE(std::is_copy_assignable<SPV>::value);
    EXPECT_TRUE(std::is_copy_constructible<SPV>::value);
    EXPECT_TRUE(std::is_default_constructible<SPV>::value);
    EXPECT_TRUE(std::is_destructible<SPV>::value);
}

// TEST(VarientTest, CopyConstructor){
//     std::vector<SPV> foo({SPV(0)});
//     std::cout << "in copy cons test" <<std::endl;
//     SPV int1 = SPV(1);
//     std::cout << "made int: "<< int1.as_integer <<std::endl;

//     std::cout << "(outer )made int: "<< int1.as_integer <<std::endl;

//     SPV int2 = SPV(int1);
//     // std::cout << "copied int: " << int2->as_integer<< std::endl;


//     ASSERT_EQ(SPV::t_int,int1.type);
//     // ASSERT_EQ(SPV::t_int,int2.type);
//     // ASSERT_EQ(int1.as_integer,int2.as_integer);

//     std::cout<< "about to make float"<<std::endl;
//     SPV float1 = SPV(1.22);
//     std::cout << "made float" <<std::endl;

//     // SPV float2 = float1;
//     // std::cout << "copied float" <<std::endl;

//     // ASSERT_EQ(SPV::t_float,float1.type);
//     // ASSERT_EQ(SPV::t_float,float2.type);
//     // ASSERT_EQ(float1.as_float,float2.as_float);

//     // SPV string1 = SPV("goober");
//     // SPV string2 = string1;
//     // ASSERT_EQ(SPV::t_string,string1.type);
//     // ASSERT_EQ(SPV::t_string,string2.type);
//     // ASSERT_EQ(string1.as_string,string2.as_string);

//     // SPV signal1 = std::function<double(int)>([](int t){return 5.4;});
//     // SPV signal2 = signal1;
//     // ASSERT_EQ(SPV::t_signal,signal1.type);
//     // ASSERT_EQ(SPV::t_signal,signal2.type);
//     // ASSERT_EQ(signal1.as_signal(0),signal2.as_signal(0));

//     // SPV list1 = SPV(std::vector<SPV>({int1,string1,signal1}));
//     // SPV list2 = list1;
//     // ASSERT_EQ(SPV::t_list,list1.type);
//     // ASSERT_EQ(SPV::t_list,list2.type);
//     // ASSERT_EQ(list1.as_list.size(),list2.as_list.size());
//     // ASSERT_EQ(3, list1.as_list.size());
//     // ASSERT_EQ(list1.as_list[0].as_integer,list2.as_list[0].as_integer);
//     // ASSERT_EQ(list1.as_list[0].as_string,list2.as_list[0].as_string);
//     // ASSERT_EQ(list1.as_list[0].as_signal(0),list2.as_list[0].as_signal(0));
//     // SPV map1 = SPV(std::map<SPV, SPV>({string1,float2}));
//     // SPV map2 = map1;
//     // ASSERT_EQ(SPV::t_map, map1.type);
//     // ASSERT_EQ(SPV::t_map, map2.type);
//     // ASSERT_EQ(map1.as_map.size(), map2.as_map.size());
//     // ASSERT_EQ(map1.as_map[string1].as_float, map2.as_map[string1].as_float);

// }

