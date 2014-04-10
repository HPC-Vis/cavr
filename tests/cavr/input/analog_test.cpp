#include <cavr/input/analog.h>
#include <gtest/gtest.h>
using cavr::input::Analog;

TEST(analog, analog) {
  Analog a;
  EXPECT_DOUBLE_EQ(0.0, a.getValue());
  a.setValue(2.0);
  EXPECT_DOUBLE_EQ(0.0, a.getValue());
  a.sync();
  EXPECT_DOUBLE_EQ(1.0, a.getValue());
  a.setValue(-2.0);
  EXPECT_DOUBLE_EQ(1.0, a.getValue());
  a.sync();
  EXPECT_DOUBLE_EQ(-1.0, a.getValue());
}
