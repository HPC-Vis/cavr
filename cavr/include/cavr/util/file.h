#pragma once
#include <string>
#include <vector>

namespace cavr {

namespace util {

class File {
public:
  /**
    Loads an entire file into a string buffer.
    
    @param path The path to the file to load.
    @param buffer The string to load the file into.
    @return true on success; false otherwise.
  */
  static bool loadIntoString(const std::string& path, std::string& buffer);

  /**
    Finds the path to a file using a prioritized list of search paths.

    @param name The name of the file to find; can itself be a path.
    @param search_paths The list of directories to search.
    @param result_path On success, contains the complete path to the file.
    @return true if the file is found; false otherwise.
  */
  static bool find(const std::string& name,
                   const std::vector<std::string>& search_paths,
                   std::string& result_path);

  /**
    Returns true if the path is a file.

    @param path The path to a file.
    @return true if the file exists; false otherwise.
  */
  static bool exists(const std::string& path);
};

} // namespace util

} // namespace cavr
