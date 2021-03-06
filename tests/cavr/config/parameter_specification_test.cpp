#include <cavr/config/parameter_specification.h>
#include "gtest/gtest.h"

using cavr::config::ParameterSpecification;
using cavr::config::ParameterType;
using cavr::config::Parameter;

TEST(parameter_specification, parameter_specification) {
  Parameter<double> ps("test", false);
  EXPECT_EQ(ps.type(), ParameterType::kNumber);
  EXPECT_EQ(ps.name(), "test");
  EXPECT_FALSE(ps.required());
  ParameterSpecification* qs = ps.copy();
  EXPECT_EQ(qs->type(), ParameterType::kNumber);
  EXPECT_EQ(qs->name(), "test");
  EXPECT_FALSE(qs->required());
}

TEST(parameter_specification_any_of,test)
{
  Parameter<double> ps("test", false);
  EXPECT_EQ(ps.type(), ParameterType::kNumber);
  EXPECT_EQ(ps.name(), "test");
  EXPECT_FALSE(ps.required());
  ParameterSpecification* qs = ps.copy();
  EXPECT_EQ(qs->type(), ParameterType::kNumber);
  EXPECT_EQ(qs->name(), "test");
  EXPECT_FALSE(qs->required());
}
