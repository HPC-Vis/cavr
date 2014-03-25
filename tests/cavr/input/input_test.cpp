#include <cavr/input/input.h>
#include "gtest/gtest.h"
using cavr::input::Input;

class DerivedInput : public Input {
};

TEST(input, input) {
  DerivedInput* input = new DerivedInput();
  EXPECT_TRUE(input->getName().empty());
  EXPECT_TRUE(input->getDescription().empty());
  input->setName("test");
  EXPECT_EQ("test", input->getName());
  input->setDescription("blar har har");
  EXPECT_EQ("blar har har", input->getDescription());
}
