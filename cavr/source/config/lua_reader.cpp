#include <cavr/config/lua_reader.h>
#include <cavr/util/file.h>

namespace cavr {

namespace config {

LuaReader::LuaReader() {
}

bool LuaReader::getKeys(const std::string& path,
                        std::vector<std::string>& keys) {
  std::vector<std::string> path_parts;
  util::String::split(path, ".", path_parts);
  for (const auto& part : path_parts) {
    if (!part.empty() && !lua_state_.pushTable(part)) {
      LOG(ERROR) << "Invalid path: " << path;
      lua_state_.reset();
      return false;
    }
  }
  bool result = lua_state_.readKeys(keys);
  lua_state_.reset();
  return result;
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
  LuaReader* reader = new LuaReader();
  if (!reader->lua_state_.loadFile(path)) {
    delete reader;
    return nullptr;
  }
  return reader;
}

} // namespace config

} // namespace cavr
