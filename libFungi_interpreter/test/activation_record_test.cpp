#include <gtest/gtest.h>
#include "../src/interpreter/activation_record.h"

TEST(ActivationRecordTest, PropertiesTest){
    EXPECT_TRUE(std::is_move_constructible<ActivationRecord>::value);
    EXPECT_TRUE(std::is_move_assignable<ActivationRecord>::value);
    EXPECT_TRUE(std::is_nothrow_move_assignable<ActivationRecord>::value);

    EXPECT_TRUE(std::is_copy_assignable<ActivationRecord>::value);
    EXPECT_TRUE(std::is_copy_constructible<ActivationRecord>::value);
    EXPECT_TRUE(std::is_destructible<ActivationRecord>::value);

}

TEST(ActivationRecordTest, ObjectIO){
    ActivationRecord record = ActivationRecord("name", ActivationRecord::t_program, 1);
    ASSERT_EQ("name", record.name);
    ASSERT_EQ(ActivationRecord::t_program, record.type);
    ASSERT_EQ(1, record.nestingLevel);

    SporeDataVariant data1 = SporeDataVariant(42);
    SporeDataVariant data2 = SporeDataVariant("goob");
    record.set("data1",data1);
    record.set("data2",data2);

    EXPECT_EQ(SporeDataVariant::t_int, record.get("data1").type);
    EXPECT_EQ(42, record.get("data1").as_integer);

    EXPECT_EQ(SporeDataVariant::t_string, record.get("data2").type);
    EXPECT_EQ("goob", record.get("data2").as_string);
}