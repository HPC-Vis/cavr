#include <cavr/config/lua_reader.h>
#include <cavr/util/file.h>

namespace cavr {

namespace config {

LuaReader::LuaReader() {
}

bool LuaReader::getKeys(const std::string& path,
                        std::vector<std::string>& keys) {
  return true;
}

LuaReader::~LuaReader() {
}

LuaReader* LuaReader::createFromBuffer(const std::string& buffer) {
  LuaReader* reader = new LuaReader();
  if (!reader->lua_state_.loadBuffer(buffer)) {
    delete reader;
    return nullptr;
  }
  return reader;
}

LuaReader* LuaReader::createFromFile(const std::string& path) {
  std::string buffer;
  if (!util::File::loadIntoString(path, buffer)) {
    LOG(ERROR) << "Failed to create LuaReader from file " << path;
    return nullptr;
  }
  return createFromBuffer(buffer);
}

} // namespace config

} // namespace cavr
