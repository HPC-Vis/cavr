#pragma once
#include <string>

namespace cavr {

namespace util {

struct Environment {
  static std::string hostname();
  static std::string cwd();
};

} // namespace util

} // namespace cavr
