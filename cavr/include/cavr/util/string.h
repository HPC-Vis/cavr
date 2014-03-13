#include <string>

namespace cavr {

namespace util {

class String {
public:
  static int split(const std::string& s,
                   const char* delimiter) {
    return 0;
  }

  template<typename... Strings>
  static int split(const std::string& s,
                   const char* delimiter,
                   std::string& first,
                   Strings&&... strings) {
    if (sizeof...(Strings) == 0) {
      first = s;
      return 0;
    }
    size_t split_pos = s.find(delimiter);
    if (std::string::npos == split_pos) {
      first = s;
      return split("", delimiter, std::forward<Strings>(strings)...);
    }
    first = s.substr(0, split_pos);
    return 1 + split(s.substr(split_pos + 1),
                     delimiter,
                     std::forward<Strings>(strings)...);
  }

  template<typename... Strings>
  static int split(const std::string& s,
                   const std::string& delimiter,
                   Strings&&... strings) {
    return split(s, delimiter.c_str(), std::forward<Strings>(strings)...);
  }

  static int rsplit(const std::string& s, const char* delimiter) {
    return 0;
  }
};

} // namespace util

} // namespace cavr
