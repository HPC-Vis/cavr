#include <cavr/input/switch.h>
#include "gtest/gtest.h"
using cavr::input::Switch;

TEST(switch_input, switch_input) {
  Switch s(3);
  EXPECT_EQ(0, s.getState());
  EXPECT_EQ(3, s.getNumberOfStates());
  EXPECT_TRUE(s.getStateName(0).empty());
  EXPECT_TRUE(s.getStateName(1).empty());
  EXPECT_TRUE(s.getStateName(2).empty());
  EXPECT_TRUE(s.getStateDescription(0).empty());
  EXPECT_TRUE(s.getStateDescription(1).empty());
  EXPECT_TRUE(s.getStateDescription(2).empty());
  s.setStateName(0, "ant");
  s.setStateName(1, "bear");
  s.setStateName(2, "cat");
  s.setStateName(3, "dog");
  EXPECT_EQ("ant", s.getStateName(0));
  EXPECT_EQ("bear", s.getStateName(1));
  EXPECT_EQ("cat", s.getStateName(2));
  EXPECT_EQ("OUT OF RANGE", s.getStateName(3));
  s.setStateDescription(0, "alpha");
  s.setStateDescription(1, "beta");
  s.setStateDescription(2, "gamma");
  s.setStateDescription(3, "delta");
  EXPECT_EQ("alpha", s.getStateDescription(0));
  EXPECT_EQ("beta", s.getStateDescription(1));
  EXPECT_EQ("gamma", s.getStateDescription(2));
  EXPECT_EQ("OUT OF RANGE", s.getStateDescription(3));
  s.setState(2);
  EXPECT_EQ(2, s.getState());
}
