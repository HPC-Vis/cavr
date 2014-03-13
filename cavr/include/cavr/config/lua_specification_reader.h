#pragma once
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <string>

#include <cavr/config/configuration_specification.h>

namespace cavr {

namespace config {

class LuaSpecificationReader {
public:
  LuaSpecificationReader();
  bool execute(const std::string& script, const std::string& chunk_name);
  ConfigurationSpecification* readConfig(const std::string& name);
  ~LuaSpecificationReader();
private:
  lua_State* lua_state_;
};

} // namespace config

} // namespace cavr
