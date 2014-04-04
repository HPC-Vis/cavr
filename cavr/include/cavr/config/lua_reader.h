#pragma once
#include <glog/logging.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <string>

#include <cavr/util/lua_state.h>
#include <cavr/util/string.h>

namespace cavr {

namespace config {

class LuaReader {
public:
  static LuaReader* createFromBuffer(const std::string& buffer);
  static LuaReader* createFromFile(const std::string& path);
  template<typename T>
  bool get(const std::string& path, T& value);
  bool getKeys(const std::string& path, std::vector<std::string>& keys);
  ~LuaReader();
private:
  LuaReader();
  util::LuaState lua_state_;
};

template<typename T>
bool LuaReader::get(const std::string& path, T& value) {
  std::vector<std::string> path_parts;
  util::String::split(path, ".", path_parts);
  if (path_parts.empty()) {
    LOG(ERROR) << "Empty path.";
    return false;
  }
  std::string var_name = path_parts.back();
  path_parts.pop_back();
  for (const auto& part : path_parts) {
    if (!part.empty() && !lua_state_.pushTable(part)) {
      LOG(ERROR) << "Invalid path: " << path;
      lua_state_.reset();
      return false;
    }
  }
  bool result = lua_state_.getValue(var_name, value);
  lua_state_.reset();
  if (!result) {
    LOG(ERROR) << "Invalid variable: " << path;
  }
  return result;
}

} // namespace config

} // namespace cavr
