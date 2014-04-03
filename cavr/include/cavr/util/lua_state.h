#pragma once
#include <cavr/config/config.h>
#include <cavr/input/marker.h>
#include <type_traits>

#include <glog/logging.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

struct swig_type_info;

namespace cavr {

namespace util {

class LuaState {
public:
  LuaState();
  bool loadBuffer(const std::string& buffer);
  bool loadFile(const std::string& path);
  bool pushTable(const std::string& name);
  bool popTable(const std::string& name);
  bool pushValue(const std::string& key);
  bool popValue(const std::string& key);
  void reset();
  template<typename T,
           typename =
             typename std::enable_if<std::is_arithmetic<T>::value>::type>
  bool readValue(T& value);
  bool readValue(std::string& value);
  bool readValue(bool& value);
  bool readValue(config::transform& value);
  bool readValue(std::vector<std::string>& value);
  bool readValue(input::Marker*& value);
  bool readKeys(std::vector<std::string>& keys);
  template<typename T> 
  bool getValue(const std::string& key, T& value);
  lua_State* getState();
  int getStackDepth() const;
  swig_type_info* getTypeInfo(const std::string& type);
  ~LuaState();
private:
  lua_State* L_;
  int stack_depth_;
  struct swig_type_info* transform_type_info_;
  struct swig_type_info* vector_type_info_;
  struct swig_type_info* sixdof_marker_type_info_;
};

template<typename T, typename>
bool LuaState::readValue(T& value) {
  if (!lua_isnumber(L_, -1)) {
    LOG(ERROR) << "value is not a number.";
    return false;
  }
  value = (T)lua_tonumber(L_, -1);
  return true;
}

template<typename T>
bool LuaState::getValue(const std::string& key, T& value) {
  if (!pushValue(key)) {
    LOG(ERROR) << key << " does not exist.";
    return false;
  }
  if (!readValue(value)) {
    LOG(ERROR) << "Failed to read key " << key;
    popValue(key);
    return false;
  }
  popValue(key);
  return true;
}

} // namespace util

} // namespace cavr
