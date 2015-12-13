#include <cavr/input/input_manager.h>
#include <cavr/util/environment.h>

#include <cavr/util/lua_state.h>
#include <cavr/util/string.h>
#include <cavr/util/swigluart.h>

extern "C" int luaopen_cavr(lua_State* L);

namespace cavr {

namespace util {

LuaState::LuaState() {
  L_ = lua_open();
  luaL_openlibs(L_);
  luaopen_cavr(L_);
  transform_type_info_ = getTypeInfo("cavr::config::transform *");
  vector_type_info_ = getTypeInfo("cavr::config::vec *");
  sixdof_marker_type_info_ = getTypeInfo("cavr::config::sixdof_marker *");
  stack_depth_ = 0;
}

bool LuaState::loadBuffer(const std::string& buffer) {
  if (0 != luaL_loadbuffer(L_, buffer.c_str(), buffer.length(), "")) {
    LOG(ERROR) << "Could not load Lua file." << std::endl;
    LOG(ERROR) << lua_tostring(L_, -1);
    return false;
  }
  if (0 != lua_pcall(L_, 0, 0, 0)) {
    LOG(ERROR) << "Could not execute Lua file." << std::endl;
    LOG(ERROR) << lua_tostring(L_, -1);
    return false;
  }
  return true;
}

bool LuaState::loadFile(const std::string& path) {
  std::string path_to_file, file_name;
  util::String::rsplit(path, "/", path_to_file, file_name);
  std::string import_function =
    "import = function(s) dofile(\"" + path_to_file + "/" + "\" .. s) end";
  if (!loadBuffer(import_function)) {
    LOG(ERROR) << "Failed to load import function.";
    return false;
  }
  std::string hostname = util::Environment::hostname();
  std::string hostname_setting =
    "HOSTNAME = \"" + std::string(hostname) + "\";";
  if (!loadBuffer(hostname_setting)) {
    LOG(ERROR) <<" Failed to set hostname.";
    return false;
  }
  if (0 != luaL_loadfile(L_, path.c_str())) {
    LOG(ERROR) << "Could not load Lua file." << std::endl;
    LOG(ERROR) << lua_tostring(L_, -1);
    return false;
  }
  if (0 != lua_pcall(L_, 0, 0, 0)) {
    LOG(ERROR) << "Could not execute Lua file." << std::endl;
    LOG(ERROR) << lua_tostring(L_, -1);
    return false;
  }
  return true;
}

bool LuaState::pushValue(const std::string& name) {
  if (0 == stack_depth_) {
    lua_getglobal(L_, name.c_str());
  } else {
    lua_pushstring(L_, name.c_str());
    lua_gettable(L_, -2);
  }
  if (!lua_isnil(L_, -1)) {
    ++stack_depth_;
    return true;
  }
  lua_pop(L_, 1);
  if (stack_depth_ > 0 &&
      !name.empty() &&
      name.find_first_not_of("0123456789") == std::string::npos) {
    lua_pushinteger(L_, atoi(name.c_str()));
    lua_gettable(L_, -2);
    if (!lua_isnil(L_, -1)) {
      ++stack_depth_;
      return true;
    } else {
      lua_pop(L_, 1);
    }
  }
  return false;
}

bool LuaState::popValue(const std::string& name) {
  lua_pop(L_, 1);
  --stack_depth_;
  return true;
}

bool LuaState::pushTable(const std::string& name) {
  if (!pushValue(name)) {
    return false;
  }
  if (!lua_istable(L_, -1)) {
    popValue(name);
    return false;
  }
  return true;
}

bool LuaState::popTable(const std::string& name) {
  lua_pop(L_, 1);
  --stack_depth_;
  return true;
}

void LuaState::reset() {
  lua_pop(L_, stack_depth_);
  stack_depth_ = 0;
}

bool LuaState::readValue(std::string& value) {
  if (!lua_isstring(L_, -1)) {
    LOG(ERROR) << "value is not a string.";
    return false;
  }
  value = std::string(lua_tostring(L_, -1));
  return true;
}

bool LuaState::readValue(bool& value) {
  if (lua_isnumber(L_, -1)) {
    value = (int)lua_tonumber(L_, -1);
  } else if (lua_isboolean(L_, -1)) {
    value = lua_toboolean(L_, -1);
  } else {
    LOG(ERROR) << "value is neither a number nor a boolean.";
    return false;
  }
  return true;
}

bool LuaState::readValue(config::transform& value) {
  config::transform* t = nullptr;
  if (SWIG_IsOK(SWIG_ConvertPtr(L_, -1, (void**)&t, transform_type_info_, 0))) {
    value = *t;
    return true;
  }
  LOG(ERROR) << "Failed to convert pointer for transform.";
  return false;
}

bool LuaState::readValue(input::Marker*& marker) {
  config::vec* v = nullptr;
  if (SWIG_IsOK(SWIG_ConvertPtr(L_, -1, (void**)&v, vector_type_info_, 0))) {
    marker = new input::StaticMarker(*(v->vector()));
    return true;
  }
  config::sixdof_marker* s = nullptr;
  if (SWIG_IsOK(SWIG_ConvertPtr(L_, 
                                -1, 
                                (void**)&s, 
                                sixdof_marker_type_info_, 
                                0))) {
    const input::SixDOF* sixdof =
      input::getSixDOF.byDeviceNameOrCreate(s->name());
    if (nullptr == sixdof) {
      LOG(ERROR) << "Could not find SixDOF " << s->name() << " for marker.";
      return false;
    }
    marker = new input::SixDOFMarker(*(s->pretransform().matrix()),
                                     *(s->posttransform().matrix()),
                                     sixdof);
    return true;
  }
  return false;
}

bool LuaState::readValue(std::vector<std::string>& value) {
  lua_pushnil(L_);
  while (lua_next(L_, -2)) {
    value.push_back(std::string(lua_tostring(L_, -1)));
    lua_pop(L_, 1);
  }
  return true;
}

bool LuaState::readKeys(std::vector<std::string>& keys) {
  lua_pushnil(L_);
  while (lua_next(L_, -2)) {
    lua_pushvalue(L_, -2);
    keys.push_back(std::string(lua_tostring(L_, -1)));
    lua_pop(L_, 1);
    //keys.push_back(std::string(lua_tostring(L_, -2)));
    lua_pop(L_, 1);
  }
  return true;
}

lua_State* LuaState::getState() {
  return L_;
}

int LuaState::getStackDepth() const {
  return stack_depth_;
}

swig_type_info* LuaState::getTypeInfo(const std::string& type) {
  swig_type_info* type_info = SWIG_TypeQuery(L_, type.c_str());
  if (nullptr == type_info) {
    LOG(ERROR) << "Could not find swig type for " << type;
  }
  return type_info;
}

LuaState::~LuaState() {
  lua_close(L_);
}

} // namespace util

} // namespace cavr
