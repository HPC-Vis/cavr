#include <cavr/util/paths.h>

namespace cavr {

namespace util {

std::vector<std::string> Paths::getConfigPaths() {
  std::vector<std::string> paths;
  paths.push_back(".");
  std::vector<std::string> cavr_paths = getCavrPaths();
  paths.insert(paths.begin(), cavr_paths.begin(), cavr_paths.end());
  paths.push_back("/usr/local/etc/cavr");
#ifdef CAVR_ETC_PATH
  paths.push_back(CAVR_ETC_PATH);
#endif
  return paths;
}

std::vector<std::string> Paths::getSchemaPaths() {
  std::vector<std::string> paths;
  paths.push_back("/usr/local/etc/cavr/schema");
#ifdef CAVR_ETC_PATH
  paths.push_back(std::string(CAVR_ETC_PATH) + "/schema");
#endif
  return paths;
}

std::vector<std::string> Paths::getCavrPaths() {
  std::vector<std::string> paths;
  const char* env_path = std::getenv("CAVR_PATH");
  if (env_path) {
    String::split(std::string(env_path), ":", paths);
  }
  return paths;
}

} // namespace util

} // namespace cavr
