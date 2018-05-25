#include "gtest/gtest.h"
#include "clamped_numbers.hh"

namespace
{
  using namespace clamped;
  
  TEST(BasicNumber, Getters)
  {
    BasicClampedNumber<int> num(2, -10, 10);
    EXPECT_EQ(num.value(), 2);
    EXPECT_EQ(num.minValue(), -10);
    EXPECT_EQ(num.maxValue(), 10);
  }
}
