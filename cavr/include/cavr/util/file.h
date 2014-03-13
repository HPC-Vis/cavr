#pragma once
#include <string>

namespace cavr {

namespace util {

class File {
public:
  static bool loadIntoString(const std::string& path, std::string& buffer);
};

} // namespace util

} // namespace cavr
