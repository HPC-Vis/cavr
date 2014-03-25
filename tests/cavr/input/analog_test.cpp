#include <cavr/input/analog.h>
#include <gtest/gtest.h>
using cavr::input::Analog;

TEST(analog, analog) {
  Analog a(1, 5.0);
  EXPECT_DOUBLE_EQ(1.0, a.getMin());
  EXPECT_DOUBLE_EQ(5.0, a.getMax());
  EXPECT_DOUBLE_EQ(1.0, a.getValue());
  a.setMin(3.0);
  a.setMax(4.0);
  EXPECT_DOUBLE_EQ(3.0, a.getMin());
  EXPECT_DOUBLE_EQ(4.0, a.getMax());
  EXPECT_DOUBLE_EQ(3.0, a.getValue());
  a.setValue(3.5);
  EXPECT_DOUBLE_EQ(3.5, a.getValue());
  a.setValue(5.0);
  EXPECT_DOUBLE_EQ(4.0, a.getValue());
}
