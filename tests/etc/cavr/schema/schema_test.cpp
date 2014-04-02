#include <cavr/util/file.h>
#include <cavr/util/paths.h>
#include <cavr/config/configuration_specification.h>
#include "gtest/gtest.h"
using cavr::config::ConfigurationSpecification;
using cavr::util::File;
using cavr::util::Paths;

TEST(schema_files, input) {
  auto check_input = [=](const std::string& name) {
    std::vector<std::string> search_paths = Paths::getSchemaPaths();
    std::string file_path;
    std::string file_name = "input/" + name + ".lua";
    ASSERT_TRUE(File::find(file_name, search_paths, file_path)) <<
      "Could not find file " << file_name;
    ConfigurationSpecification* spec =
      ConfigurationSpecification::createFromLuaFile(file_path, name);
    ASSERT_NE(nullptr, spec) << "Invalid schema for " << name;
    delete spec;
  };
  check_input("analog");
  check_input("sixdof");
}

TEST(schema_files, plugin) {
  auto check_plugin = [=](const std::string& name) {
    std::vector<std::string> search_paths = Paths::getSchemaPaths();
    std::string file_path;
    std::string file_name = "plugins/" + name + ".lua";
    ASSERT_TRUE(File::find(file_name, search_paths, file_path)) <<
      "Could not find file " << file_name;
    ConfigurationSpecification* spec =
      ConfigurationSpecification::createFromLuaFile(file_path, name);
    ASSERT_NE(nullptr, spec) << "Invalid schema for " << name;
    delete spec;
  };
  check_plugin("x11");
}
