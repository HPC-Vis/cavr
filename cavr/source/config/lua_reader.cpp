#include <cavr/config/lua_reader.h>
#include <cavr/util/file.h>

namespace cavr {

namespace config {

LuaReader::LuaReader() {
  L_ = lua_open();
  luaL_openlibs(L_);
}

LuaReader::~LuaReader() {
  lua_close(L_);
}

bool LuaReader::loadBuffer_(const std::string& buffer) {
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

bool LuaReader::getValue_(std::string& value) {
  if (!lua_isstring(L_, -1)) {
    LOG(ERROR) << "var is not a string.";
    return false;
  }
  value = std::string(lua_tostring(L_, -1));
  return true;
}

LuaReader* LuaReader::createFromBuffer(const std::string& buffer) {
  LuaReader* reader = new LuaReader();
  if (!reader->loadBuffer_(buffer)) {
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