#include <cavr/config/configuration_specification.h>
#include "gtest/gtest.h"

using cavr::config::ConfigurationSpecification;
using cavr::config::ParameterSpecification;
using cavr::config::ParameterType;

TEST(configuration_specification, configuration_specification) {
  ConfigurationSpecification cs;
  ParameterSpecification ps(ParameterType::kTransform, "foo", false);
  EXPECT_TRUE(cs.addParameter(ps));
  EXPECT_FALSE(cs.addParameter(ps));
  ParameterSpecification qs(ParameterType::kNumber, "foo", true);
  EXPECT_FALSE(cs.addParameter(qs));
  const auto& parameters = cs.getMap();
  EXPECT_EQ(1, parameters.size());
  EXPECT_EQ(1, parameters.count("foo"));
  EXPECT_EQ(ParameterType::kTransform, parameters.find("foo")->second.type());
  EXPECT_EQ("foo", parameters.find("foo")->second.name());
  EXPECT_FALSE(parameters.find("foo")->second.required());
}
