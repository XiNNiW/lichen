#include <gtest/gtest.h>
#include "../src/interpreter/scoped_symbol_table.h"

TEST(ScopedSymbolTableTest, PropertiesTest){
    EXPECT_TRUE(std::is_move_constructible<ScopedSymbolTable>::value);
    EXPECT_TRUE(std::is_move_assignable<ScopedSymbolTable>::value);

    EXPECT_TRUE(std::is_copy_assignable<ScopedSymbolTable>::value);
    EXPECT_TRUE(std::is_copy_constructible<ScopedSymbolTable>::value);
    EXPECT_TRUE(std::is_destructible<ScopedSymbolTable>::value);

}

TEST(ScopedSymbolTableTest, NoEncolosingScope){
    ScopedSymbolTable t = ScopedSymbolTable("name", 1);
    EXPECT_EQ("name", t.scopeName);
    EXPECT_EQ(1, t.scopeLevel);

    SymbolVariant sym1 = SymbolVariant(BuiltInSymbol("name1",55));
    SymbolVariant sym2 = SymbolVariant(DataVariableSymbol("name2",63));
    BlockNode block = BlockNode(new StatementsNode(new IdentifierNode("NAME")));
    SymbolVariant sym3 = SymbolVariant(FunctionVariableSymbol("name3",std::vector<std::string>({"arg1"}),BlockNode(block)));
    t.insert(sym1);
    t.insert(sym2);
    t.insert(sym3);

    ASSERT_EQ(SymbolVariant::t_built_in, t.lookup("name1").type);
    EXPECT_EQ("name1", t.lookup("name1").as_built_in.name);
    EXPECT_EQ(1, t.lookup("name1").as_built_in.scopeLevel);

    ASSERT_EQ(SymbolVariant::t_data_variable,t.lookup("name2").type);
    EXPECT_EQ("name2", t.lookup("name2").as_data_var.name);
    EXPECT_EQ(1, t.lookup("name2").as_data_var.scopeLevel);

    ASSERT_EQ(SymbolVariant::t_function_variable,t.lookup("name3").type);
    EXPECT_EQ("name3", t.lookup("name3").as_func_var.name);
    EXPECT_EQ(1, t.lookup("name3").as_func_var.scopeLevel);
    EXPECT_EQ(1, t.lookup("name3").as_func_var.formalParams.size());
    EXPECT_EQ("arg1", t.lookup("name3").as_func_var.formalParams[0]);


}