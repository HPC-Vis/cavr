#include <cavr/util/environment.h>
#include <unistd.h>

namespace cavr {

namespace util {

std::string Environment::hostname() {
  char hostname[512];
  gethostname(hostname, 512);
  return std::string(hostname);
}

std::string Environment::cwd() {
  char* c = get_current_dir_name();
  std::string result(c);
  delete c;
  return result;
}

} // namespace util

} // namespace cavr
