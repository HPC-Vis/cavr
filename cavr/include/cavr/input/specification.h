#pragma once
#include <cavr/input/analog.h>
#include <cavr/input/sixdof.h>
#include <cavr/input/switch.h>

namespace cavr {

namespace input {

class Specification {
public:
  std::map<std::string, Switch*> switches;
  std::map<std::string, Analog*> analogs;
  std::map<std::string, SixDOF*> sixdofs;
  static Specification* createFromLuaFile(const std::string& path);
};

} // namespace input

} // namespace cavr
