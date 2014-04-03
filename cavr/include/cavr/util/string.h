#pragma once
#include <algorithm>
#include <cstring>
#include <limits>
#include <string>
#include <vector>

namespace cavr {

namespace util {

namespace string {

template<typename... T>
struct are_references {
  static const bool value = false;
};

template<>
struct are_references<> {
  static const bool value = true;
};

template<typename T, typename... U>
struct are_references<T, U...> {
  static const bool value =
    std::is_same<T, std::string&>::value && are_references<U...>::value;
};

} // namespace string

class String {
public:
  static int split(const std::string& s, const char* delimiter) {
    return 0;
  }

  template<typename... S,
           typename =
             typename std::enable_if<string::are_references<S...>::value>::type>
  static int split(const std::string& s,
                   const char* delimiter,
                   S&&... strings) {
    return split(s, delimiter, size_t(0), std::forward<S>(strings)...);
  }

  template<typename... S>
  static int split(const std::string& s,
                   const std::string& delimiter,
                   S&&... strings) {
    return split(s, delimiter.c_str(), std::forward<S>(strings)...);
  }

  template<typename... S,
           typename =
             typename std::enable_if<string::are_references<S...>::value>::type>
  static int split(const std::string& s,
                   const char* delimiter,
                   size_t start_pos,
                   std::string& first,
                   S&&... strings) {
    if (sizeof...(S) == 0) {
      first = s.substr(start_pos);
      return 0;
    }
    size_t split_pos = s.find(delimiter, start_pos);
    if (std::string::npos == split_pos) {
      first = s.substr(start_pos);
      return split("", delimiter, size_t(0), std::forward<S>(strings)...);
    }
    first = s.substr(start_pos, split_pos - start_pos);
    return 1 + split(s,
                     delimiter,
                     split_pos + strlen(delimiter),
                     std::forward<S>(strings)...);
  }

  static void split(const std::string& s,
                    const char* delimiter,
                    std::vector<std::string>& results,
                    int max_splits = std::numeric_limits<int>::max()) {
    size_t delimiter_length = strlen(delimiter);
    size_t start_pos = 0;
    for (int i = 0; i < max_splits; ++i) {
      size_t end_pos = s.find(delimiter, start_pos);
      if (std::string::npos == end_pos) {
        break;
      }
      results.push_back(s.substr(start_pos, end_pos - start_pos));
      start_pos = end_pos + delimiter_length;
    }
    results.push_back(s.substr(start_pos));
  }

  static int rsplit(const std::string& s,
                    const char* delimiter, 
                    size_t& start_pos,
                    int depth) {
    return 0;
  }

  template<typename... S,
           typename =
             typename std::enable_if<string::are_references<S...>::value>::type>
  static int rsplit(const std::string& s,
                    const char* delimiter,
                    S&&... strings) {
    size_t start_pos = s.length() - 1;
    return rsplit(s,
                  delimiter,
                  start_pos,
                  0,
                  std::forward<S>(strings)...);
  }

  template<typename... S,
           typename =
             typename std::enable_if<string::are_references<S...>::value>::type>
  static int rsplit(const std::string& s,
                    const char* delimiter,
                    size_t& start_pos,
                    int depth,
                    std::string& first,
                    S&&... strings) {
    int total_splits = 0;
    if (sizeof...(S) > 0) {
      total_splits = rsplit(s,
                            delimiter,
                            start_pos,
                            depth + 1,
                            std::forward<S>(strings)...);
    }
    if (std::string::npos == start_pos) {
      first = "";
      return total_splits;
    }
    size_t end_pos = start_pos;
    size_t delimiter_length = strlen(delimiter);
    start_pos = s.rfind(delimiter, end_pos);
    if (std::string::npos == start_pos || 0 == depth) {
      first = s.substr(0, end_pos + 1);
      return total_splits;
    } else {
      first = s.substr(start_pos + delimiter_length,
                       end_pos - start_pos - delimiter_length + 1);
      if (0 != start_pos) {
        --start_pos;
      }
      return total_splits + 1;
    }
  }

  static void rsplit(const std::string& s,
                     const char* delimiter,
                     std::vector<std::string>& results,
                     int max_splits = std::numeric_limits<int>::max()) {
    size_t delimiter_length = strlen(delimiter);
    size_t start_pos = s.length();
    for (int i = 0; i < max_splits; ++i) {
      size_t end_pos = s.rfind(delimiter, start_pos);
      if (std::string::npos == end_pos) {
        break;
      }
      results.push_back(s.substr(end_pos + delimiter_length,
                                 start_pos - end_pos - delimiter_length + 1));
      start_pos = end_pos + delimiter_length;
      if (0 == end_pos) {
        std::reverse(results.begin(), results.end());
        return;
      }
      start_pos = end_pos - 1;
    }
    results.push_back(s.substr(0, start_pos + 1));
    std::reverse(results.begin(), results.end());
  }
};

} // namespace util

} // namespace cavr
