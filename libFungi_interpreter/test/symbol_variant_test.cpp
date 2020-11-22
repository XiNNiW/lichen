#include <gtest/gtest.h>
#include "../src/interpreter/symbol.h"

TEST(SymbolVariantTest, NullSymbolTest){
    SymbolVariant s1 = SymbolVariant();
    SymbolVariant nullSym = SymbolVariant::voidSymbol();
    EXPECT_EQ(SymbolVariant::t_unknown, s1.type);
    EXPECT_EQ(SymbolVariant::t_unknown, nullSym.type);

}

TEST(SymbolVariantTest, PropertiesTest){
    EXPECT_TRUE(std::is_move_constructible<SymbolVariant>::value);
    EXPECT_TRUE(std::is_move_assignable<SymbolVariant>::value);

    EXPECT_TRUE(std::is_copy_assignable<SymbolVariant>::value);
    EXPECT_TRUE(std::is_copy_constructible<SymbolVariant>::value);
    EXPECT_TRUE(std::is_default_constructible<SymbolVariant>::value);
    EXPECT_TRUE(std::is_destructible<SymbolVariant>::value);
    
}

TEST(SymbolVariantTest, CopyConstructor){
    
    std::cout<<"BUILT IN TEST\n";
    SymbolVariant builtIn1 = SymbolVariant(BuiltInSymbol("foo", 1));
    SymbolVariant builtIn2 = SymbolVariant(builtIn1);
    ASSERT_EQ(SymbolVariant::t_built_in,builtIn1.type);
    ASSERT_EQ(SymbolVariant::t_built_in,builtIn2.type);
    ASSERT_EQ(builtIn1.as_built_in.name,builtIn2.as_built_in.name);
    ASSERT_EQ(builtIn1.as_built_in.scopeLevel,builtIn2.as_built_in.scopeLevel);
    
    std::cout<<"DATA TEST\n";
    SymbolVariant data1 = SymbolVariant(DataVariableSymbol("foo", 1));
    SymbolVariant data2 = SymbolVariant(data1);
    ASSERT_EQ(SymbolVariant::t_data_variable,data1.type);
    ASSERT_EQ(SymbolVariant::t_data_variable,data2.type);
    ASSERT_EQ(data1.as_data_var.name,data2.as_data_var.name);
    ASSERT_EQ(data1.as_data_var.scopeLevel,data2.as_data_var.scopeLevel);

    std::cout<<"ARGS TEST\n";
    SymbolVariant args1 = SymbolVariant(ArgumentsListSymbol({"foo","bang"}));
    SymbolVariant args2 = SymbolVariant(args1);
    ASSERT_EQ(SymbolVariant::t_arguments_list,args1.type);
    ASSERT_EQ(SymbolVariant::t_arguments_list,args2.type);
    ASSERT_EQ(args1.as_arg_list.list,args2.as_arg_list.list);

    SymbolVariant id1 = SymbolVariant(IdentifierSymbol("fub"));
    SymbolVariant id2 = SymbolVariant(id1);
    ASSERT_EQ(SymbolVariant::t_identifier,id1.type);
    ASSERT_EQ(SymbolVariant::t_identifier,id2.type);
    ASSERT_EQ(id1.as_identifier.name,id2.as_identifier.name);


    BlockNode block = BlockNode(new StatementsNode(new IdentifierNode("NAME")));

    SymbolVariant func1 = SymbolVariant(FunctionVariableSymbol("foo", 1, std::vector<std::string>({"arg1", "arg2"}), block));
    SymbolVariant func2 = SymbolVariant(func1);
    ASSERT_EQ(SymbolVariant::t_function_variable,func1.type);
    ASSERT_EQ(SymbolVariant::t_function_variable,func2.type);
    ASSERT_EQ("foo",func2.as_built_in.name);
    ASSERT_EQ(func1.as_func_var.name,func2.as_built_in.name);
    ASSERT_EQ(1,func2.as_func_var.scopeLevel);
    ASSERT_EQ(func1.as_func_var.scopeLevel,func2.as_func_var.scopeLevel);
    ASSERT_EQ(2,func2.as_func_var.formalParams.size());
    ASSERT_EQ(func1.as_func_var.formalParams.size(),func2.as_func_var.formalParams.size());
    ASSERT_EQ("arg1",func2.as_func_var.formalParams[0]);
    ASSERT_EQ(func1.as_func_var.formalParams[0],func2.as_func_var.formalParams[0]);
    ASSERT_EQ("arg2",func2.as_func_var.formalParams[1]);
    ASSERT_EQ(func1.as_func_var.formalParams[1],func2.as_func_var.formalParams[1]);

    SymbolVariant null1 = SymbolVariant::voidSymbol();
    SymbolVariant null2 = SymbolVariant(null1);
    ASSERT_EQ(SymbolVariant::t_unknown,null1.type);
    ASSERT_EQ(SymbolVariant::t_unknown,null2.type);

}

TEST(SymbolVariantTest, Equality){
    SymbolVariant s1 = SymbolVariant(BuiltInSymbol("INTEGER",1));
    SymbolVariant s2 = SymbolVariant(BuiltInSymbol("INTEGER",1));
    ASSERT_TRUE(s1==s2);
    s2 = SymbolVariant(BuiltInSymbol("INTEGER",2));
    ASSERT_FALSE(s1==s2);
    s2 = SymbolVariant(BuiltInSymbol("STRING",1));
    ASSERT_FALSE(s1==s2);

    BlockNode block = BlockNode(new StatementsNode(new IdentifierNode("NAME")));
    ASSERT_TRUE(FunctionVariableSymbol("OSC",1,{"arg1","arg2"},block)==FunctionVariableSymbol("OSC",1,{"arg1","arg2"},block));
    ASSERT_FALSE(FunctionVariableSymbol("ENV",1,{"arg1","arg2"},block)==FunctionVariableSymbol("OSC",1,{"arg1","arg2"},block));
    ASSERT_FALSE(FunctionVariableSymbol("OSC",2,{"arg1","arg2"},block)==FunctionVariableSymbol("OSC",1,{"arg1","arg2"},block));
    ASSERT_FALSE(FunctionVariableSymbol("OSC",1,{"blarg1","arg2"},block)==FunctionVariableSymbol("OSC",1,{"arg1","arg2"},block));
    
    ASSERT_TRUE(DataVariableSymbol("NAME",1)==DataVariableSymbol("NAME",1));
    ASSERT_FALSE(DataVariableSymbol("NAME1",1)==DataVariableSymbol("NAME",1));
    ASSERT_FALSE(DataVariableSymbol("NAME",2)==DataVariableSymbol("NAME",1));

}

TEST(SymbolVariantTest, CanBePlacedSafelyInContainers){
    std::vector<SymbolVariant> list = std::vector<SymbolVariant>({SymbolVariant(BuiltInSymbol("INTEGER",1))});
    list.push_back(SymbolVariant(BuiltInSymbol("FLOAT",1)));
    list.push_back(SymbolVariant(BuiltInSymbol("STRING",1)));
    SymbolVariant lvalData = SymbolVariant(BuiltInSymbol("SIGNAL",1));
    list.push_back(lvalData);

    EXPECT_EQ(4, list.size());
    ASSERT_EQ(SymbolVariant::t_built_in, list[3].type);
    ASSERT_EQ(lvalData.type, list[3].type);
    EXPECT_TRUE(lvalData.as_built_in==list[3].as_built_in);

    std::map<std::string, SymbolVariant> amap = std::map<std::string, SymbolVariant>();
    amap["poo"] = lvalData;
    ASSERT_EQ(SymbolVariant::t_built_in, amap["poo"].type);
    EXPECT_EQ(lvalData.type, amap["poo"].type);
    EXPECT_EQ("SIGNAL", amap["poo"].as_built_in.name);
    EXPECT_EQ(lvalData.as_built_in.name, amap["poo"].as_built_in.name);
}

