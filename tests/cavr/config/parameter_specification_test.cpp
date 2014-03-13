#include <cavr/config/parameter_specification.h>
#include "gtest/gtest.h"

using cavr::config::ParameterSpecification;
using cavr::config::ParameterType;

TEST(parameter_specification, parameter_specification) {
  ParameterSpecification ps(ParameterType::kNumber, "test", false);
  EXPECT_EQ(ps.type(), ParameterType::kNumber);
  EXPECT_EQ(ps.name(), "test");
  EXPECT_FALSE(ps.required());
  ParameterSpecification qs = ps;
  EXPECT_EQ(qs.type(), ParameterType::kNumber);
  EXPECT_EQ(qs.name(), "test");
  EXPECT_FALSE(qs.required());
}
