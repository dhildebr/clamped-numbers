#include "gtest/gtest.h"
#include "clamped_numbers.hh"

namespace
{
  using namespace clamped;
  
  TEST(BasicNumberTests, Getters)
  {
    BasicClampedNumber<int> num(2, -10, 10);
    EXPECT_EQ(num.value(), 2) << "BasicClampedNumber::value() does not report correct starting value.";
    EXPECT_EQ(num.minValue(), -10) << "BasicClampedNumber::minValue() does not report correct minimum value.";
    EXPECT_EQ(num.maxValue(), 10) << "BasicClampedNumber::maxValue() does not report correct maximum value.";
  }
  
  TEST(BasicNumberTests, Setters)
  {
    BasicClampedNumber<int> num(2, -10, 10);
    num.value(5);
    num.minValue(-20);
    num.maxValue(20);
    
    EXPECT_EQ(num.value(), 5) << "Post-modification current value is incorrect.";
    EXPECT_EQ(num.minValue(), -20) << "Post-modification minimum value is incorrect.";
    EXPECT_EQ(num.maxValue(), 20) << "Post-modification maximum value is incorrect.";
  }
  
  TEST(BasicNumberTests, ConstructorStretchedBounds)
  {
    BasicClampedNumber<int> num(0, 1, -1);
    ASSERT_EQ(num.value(), 0) << "BasicClampedNumber::value() does not report correct starting value.";
    EXPECT_EQ(num.minValue(), 0) << "Number minimum should stretch to fit starting value.";
    EXPECT_EQ(num.maxValue(), 0) << "Number maximum should stretch to fit starting value.";
  }
  
  TEST(BasicNumberTests, SettersStretchedBounds)
  {
    BasicClampedNumber<int> num(0, -10, 10);
    num.minValue(5);
    num.maxValue(-5);
    
    ASSERT_EQ(num.value(), 0) << "BasicClampedNumber::value() does not report correct starting value.";
    EXPECT_EQ(num.minValue(), 0) << "Number minimum should not be settable above current value.";
    EXPECT_EQ(num.maxValue(), 0) << "Number maximum should not be settable below current value.";
  }
  
  TEST(BasicNumberTests, OperatorEqSameBoundsRhsClamped)
  {
    
  }
  
  TEST(BasicNumberTests, OperatorEqDifferentBoundsRhsClamped)
  {
    
  }
  
  TEST(BasicNumberTests, OperatorNeSameBoundsRhsClamped)
  {
    
  }
  
  TEST(BasicNumberTests, OperatorNeDifferentBoundsRhsClamped)
  {
    
  }
  
  TEST(BasicNumberTests, OperatorLtSameBoundsRhsClamped)
  {
    
  }
  
  TEST(BasicNumberTests, OperatorLtDifferentBoundsRhsClamped)
  {
    
  }
  
  TEST(BasicNumberTests, OperatorLeSameBoundsRhsClamped)
  {
    
  }
  
  TEST(BasicNumberTests, OperatorLeDifferentBoundsRhsClamped)
  {
    
  }
  
  TEST(BasicNumberTests, OperatorGtSameBoundsRhsClamped)
  {
    
  }
  
  TEST(BasicNumberTests, OperatorGtDifferentBoundsRhsClamped)
  {
    
  }
  
  TEST(BasicNumberTests, OperatorGeSameBoundsRhsClamped)
  {
    
  }
  
  TEST(BasicNumberTests, OperatorGeDifferentBoundsRhsClamped)
  {
    
  }
}
