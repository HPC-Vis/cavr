#include <cavr/util/string.h>

namespace cavr {

namespace util {

class Paths {
public:
  static std::vector<std::string> getConfigPaths();
  static std::vector<std::string> getSchemaPaths();
  static std::vector<std::string> getCavrPaths();
private:
};

} // namespace util

} // namespace cavr
