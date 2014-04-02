#include <cavr/config/configuration_specification.h>
#include <cavr/config/config.h>
#include "gtest/gtest.h"

using cavr::config::ConfigurationSpecification;
using cavr::config::ParameterSpecification;
using cavr::config::Parameter;
using cavr::config::ParameterType;

TEST(configuration_specification, configuration_specification) {
  ConfigurationSpecification cs;
  Parameter<cavr::config::transform> p("foo", false);
  EXPECT_TRUE(cs.addParameter(&p));
  EXPECT_FALSE(cs.addParameter(&p));
  Parameter<double> q("foo", true);
  EXPECT_FALSE(cs.addParameter(&q));
  const auto& parameters = cs.getMap();
  EXPECT_EQ(1, parameters.size());
  EXPECT_EQ(1, parameters.count("foo"));
  EXPECT_EQ(ParameterType::kTransform, parameters.find("foo")->second->type());
  EXPECT_EQ("foo", parameters.find("foo")->second->name());
  EXPECT_FALSE(parameters.find("foo")->second->required());
}

TEST(configuration_specification, from_buffer) {
  const char* buffer =
    "test = {"
    "  a = { type = \"number\"; required = false; default = 4.0; };"
    "  b = { type = \"string\"; required = true; };"
    "  c = { type = \"transform\";"
    "        required = false;"
    "        default = cavr.identity();"
    "      };"
    "}";
  ConfigurationSpecification* spec =
    ConfigurationSpecification::createFromLuaBuffer(buffer, "test");
  ASSERT_NE(nullptr, spec);
  const auto& parameter_map = spec->getMap();
  EXPECT_EQ(3, parameter_map.size());
  EXPECT_EQ(1, parameter_map.count("a"));
  EXPECT_EQ(1, parameter_map.count("b"));
  EXPECT_EQ(1, parameter_map.count("c"));
  EXPECT_EQ(0, parameter_map.count("test"));
  ParameterSpecification* a = parameter_map.find("a")->second;
  ParameterSpecification* b = parameter_map.find("b")->second;
  ParameterSpecification* c = parameter_map.find("c")->second;
  EXPECT_EQ(cavr::config::kNumber, a->type());
  EXPECT_FALSE(a->required());
  EXPECT_EQ("a", a->name());
  EXPECT_EQ(cavr::config::kString, b->type());
  EXPECT_TRUE(b->required());
  EXPECT_EQ("b", b->name());
  EXPECT_EQ(cavr::config::kTransform, c->type());
  EXPECT_FALSE(c->required());
  EXPECT_EQ("c", c->name());
  cavr::config::transform t;
  EXPECT_TRUE(c->getDefault(t));
  EXPECT_DOUBLE_EQ(1.0, t.matrix()->v[ 0]);
  EXPECT_DOUBLE_EQ(0.0, t.matrix()->v[ 1]);
  EXPECT_DOUBLE_EQ(0.0, t.matrix()->v[ 2]);
  EXPECT_DOUBLE_EQ(0.0, t.matrix()->v[ 3]);
  EXPECT_DOUBLE_EQ(0.0, t.matrix()->v[ 4]);
  EXPECT_DOUBLE_EQ(1.0, t.matrix()->v[ 5]);
  EXPECT_DOUBLE_EQ(0.0, t.matrix()->v[ 6]);
  EXPECT_DOUBLE_EQ(0.0, t.matrix()->v[ 7]);
  EXPECT_DOUBLE_EQ(0.0, t.matrix()->v[ 8]);
  EXPECT_DOUBLE_EQ(0.0, t.matrix()->v[ 9]);
  EXPECT_DOUBLE_EQ(1.0, t.matrix()->v[10]);
  EXPECT_DOUBLE_EQ(0.0, t.matrix()->v[11]);
  EXPECT_DOUBLE_EQ(0.0, t.matrix()->v[12]);
  EXPECT_DOUBLE_EQ(0.0, t.matrix()->v[13]);
  EXPECT_DOUBLE_EQ(0.0, t.matrix()->v[14]);
  EXPECT_DOUBLE_EQ(1.0, t.matrix()->v[15]);
}

TEST(configuration_specification, bad_buffer) {
  const char* bad_type =
    "test = {"
    "  a = { type = \"number\"; required = false; };"
    "  b = { type = \"string\"; required = true; };"
    "  c = { type = \"double\"; required = false; };"
    "}";
  ConfigurationSpecification* bad_type_spec =
    ConfigurationSpecification::createFromLuaBuffer(bad_type, "test");
  ASSERT_EQ(nullptr, bad_type_spec);

  const char* missing_requirement =
    "test = {"
    "  a = { type = \"number\"; required = false; };"
    "  b = { type = \"string\"; required = true; };"
    "  c = { type = \"transform\"; requirez = false; };"
    "}";
  ConfigurationSpecification* missing_requirement_spec =
    ConfigurationSpecification::createFromLuaBuffer(missing_requirement, 
                                                    "test");
  ASSERT_EQ(nullptr, missing_requirement_spec);
}
