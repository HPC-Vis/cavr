#include <cavr/config/lua_reader.h>
#include <cavr/input/input_map.h>

#include <memory>

namespace cavr {

namespace input {

InputMap* InputMap::generateFromLuaFile(const std::string& path) {
  std::unique_ptr<config::LuaReader>
    reader(config::LuaReader::createFromFile(path));
  if (!reader) {
    LOG(ERROR) << "Could not open InputMap file: " << path;
    return nullptr;
  }
  auto readMap = [=](std::map<std::string, std::string>& map,
                     config::LuaReader* reader,
                     const std::string& var_name) {
    bool result = true;
    std::vector<std::string> keys;
    if (!reader->getKeys(var_name, keys)) {
      LOG(ERROR) << var_name << " is not a table in file " << path;
      return false;
    }
    for (const auto& user_name : keys) {
      std::string device_name;
      if (!reader->get(var_name + "." + user_name, device_name)) {
        LOG(ERROR) << var_name + "." + user_name << " is not a string.";
        result = false;
        continue;
      }
      map[user_name] = device_name;
    }
    return result;
  };
  bool result = true;
  InputMap* input_map = new InputMap();
  result &= readMap(input_map->button_map, reader.get(), "button_map");
  result &= readMap(input_map->analog_map, reader.get(), "analog_map");
  result &= readMap(input_map->sixdof_map, reader.get(), "sixdof_map");
  if (!result) {
    LOG(ERROR) << "Failed to parse InputMap file: " << path;
    delete input_map;
    return false;
  }
  return input_map;
}

} // namespace input

} // namespace cavr
