#pragma once
#include <glog/logging.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <string>

namespace cavr {

namespace config {

class LuaReader {
public:
  static LuaReader* createFromBuffer(const std::string& buffer);
  static LuaReader* createFromFile(const std::string& path);
  template<typename T>
  bool get(const std::string& path, T& value);
  ~LuaReader();
private:
  LuaReader();
  bool loadBuffer_(const std::string& buffer);
  template<typename T>
  bool getValue_(T& value);
  bool getValue_(std::string& value);
  lua_State* L_;
};

template<typename T>
bool LuaReader::get(const std::string& path, T& value) {
  size_t begin_pos = 0;
  size_t end_pos = path.find(".");
  int lua_depth = 0;
  while(end_pos != std::string::npos) {
    std::string table_name = path.substr(begin_pos, end_pos - begin_pos);
    if (0 == lua_depth) {
      lua_getglobal(L_, table_name.c_str());
      ++lua_depth;
    } else { // we're in a table already that's currently at -1
      lua_pushstring(L_, table_name.c_str());
      ++lua_depth;
      lua_gettable(L_, -2);
    }
    if (!lua_istable(L_, -1)) {
      LOG(ERROR) << table_name << " is not a table in path " <<
        path.substr(0, begin_pos);
      lua_pop(L_, lua_depth);
      return false;
    }
    begin_pos = end_pos + 1;
    end_pos = path.find(".", begin_pos);
  }
  std::string key = path.substr(begin_pos);
  if (0 == lua_depth) {
    lua_getglobal(L_, key.c_str());
  } else {
    lua_pushstring(L_, key.c_str());
    lua_gettable(L_, -2);
  }
  ++lua_depth;
  bool result = getValue_(value);
  lua_pop(L_, lua_depth);
  if (!result) {
    LOG(ERROR) << "failed to find value " << path;
  }
  return result;
}

template<typename T>
bool LuaReader::getValue_(T& value) {
  if (!lua_isnumber(L_, -1)) {
    LOG(ERROR) << "var is not a number.";
    return false;
  }
  value = (T)lua_tonumber(L_, -1);
  return true;
}

} // namespace config

} // namespace cavr
