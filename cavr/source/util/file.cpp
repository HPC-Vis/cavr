#include <fstream>
#include <glog/logging.h>

#include <cavr/util/file.h>
#include <cavr/util/string.h>

namespace cavr {

namespace util {

bool File::loadIntoString(const std::string& path, std::string& buffer) {
  std::filebuf file;
  if (nullptr == file.open(path, std::ios::in)) {
    LOG(ERROR) << "Failed to open file " << path;
    return false;
  }
  std::istream stream(&file);
  stream.seekg(0, stream.end);
  int length = stream.tellg();
  stream.seekg(0, stream.beg);
  buffer.reserve(length);
  buffer.assign(std::istreambuf_iterator<char>(stream),
                std::istreambuf_iterator<char>());
  file.close();
  return true;
}

bool File::find(const std::string& name,
                const std::vector<std::string>& search_paths,
                std::string& result_path) {
  for (const auto& path : search_paths) {
    std::string p = path + "/" + name;
    if (exists(p)) {
      result_path = p;
      return true;
    }
  }
  return false;
}

bool File::exists(const std::string& path) {
  std::filebuf file;
  return nullptr != file.open(path, std::ios::in);
}

} // namespace util

} // namespace cavr
