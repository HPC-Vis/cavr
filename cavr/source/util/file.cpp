#include <fstream>
#include <glog/logging.h>

#include <cavr/util/file.h>

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

} // namespace util

} // namespace cavr
