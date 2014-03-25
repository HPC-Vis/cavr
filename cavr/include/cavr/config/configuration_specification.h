#pragma once
#include <map>

#include <cavr/config/parameter_specification.h>

namespace cavr {

namespace config {

class ConfigurationSpecification {
public:
  ConfigurationSpecification() = default;
  bool addParameter(const ParameterSpecification& parameter);
  const std::map<std::string, ParameterSpecification>& getMap() const;
  static ConfigurationSpecification* createFromLuaFile(const std::string& path,
                                                       const std::string& name);
  static ConfigurationSpecification*
    createFromLuaBuffer(const std::string& buffer, const std::string& name);
  static ConfigurationSpecification*
    createFromLuaReader(class LuaReader* reader, const std::string& name); 
private:
  std::map<std::string, ParameterSpecification> parameters_;
};

} // namespace config

} // namespace cavr
