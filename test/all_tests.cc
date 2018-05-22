#include "gtest/gtest.h"
#include "clamped_numbers_test.cc"

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
