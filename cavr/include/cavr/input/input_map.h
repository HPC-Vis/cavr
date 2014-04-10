#pragma once
#include <map>
#include <string>

namespace cavr {

namespace input {

struct InputMap {
  std::map<std::string, std::string> button_map;
  std::map<std::string, std::string> analog_map;
  std::map<std::string, std::string> sixdof_map;
  static InputMap* generateFromLuaFile(const std::string& path);
};

} // namespace input

} // namespace cavr
