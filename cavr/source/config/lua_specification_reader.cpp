#include <cavr/config/lua_specification_reader.h>
#include <glog/logging.h>

namespace cavr {

namespace config {

LuaSpecificationReader::LuaSpecificationReader() {
  lua_state_ = lua_open();
  luaL_openlibs(lua_state_);
}

bool LuaSpecificationReader::execute(const std::string& script,
                                     const std::string& chunk_name) {
  if (0 != luaL_loadbuffer(lua_state_,
                           script.c_str(),
                           script.length(),
                           chunk_name.c_str())) {
    LOG(ERROR) << "Could not load Lua configuration script.";
    LOG(ERROR) << lua_tostring(lua_state_, -1);
    return false;
  }
  if (0 != lua_pcall(lua_state_, 0, 0, 0)) {
    LOG(ERROR) << "Could not execute Lua configuration script.";
    LOG(ERROR) << lua_tostring(lua_state_, -1);
    return false;
  }
  return true;
}

} // namespace config

} // namespace cavr
