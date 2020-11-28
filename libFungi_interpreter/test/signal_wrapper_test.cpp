#include <gtest/gtest.h>
#include "../src/interpreter/signal_wrapper.h"
#include "../../libAlgae_dsp/dsp.h"
#include <iostream>
using std::cout;


TEST(SignalWrapperTest, Properties_AndStaticAsserts){
    using S = Signal<double,int>;
    EXPECT_TRUE(std::is_move_constructible<S>::value);
    EXPECT_TRUE(std::is_move_assignable<S>::value);

    EXPECT_TRUE(std::is_copy_assignable<S>::value);
    EXPECT_TRUE(std::is_copy_constructible<S>::value);
    EXPECT_TRUE(std::is_default_constructible<S>::value);
    EXPECT_TRUE(std::is_destructible<S>::value);

}

TEST(SignalWrapperTest, CanContainSignalFunctors){
    auto freq = signals::math::constant<double>(440.0);
    auto oscillator = signals::oscillator::sinOsc<double,decltype(freq)>(48000,freq);
    Signal<double,int> osc = Signal<double,int>(oscillator);

    ASSERT_FLOAT_EQ(0.057564028,osc(0));

}