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
private:
  std::map<std::string, ParameterSpecification> parameters_;
};

} // namespace config

} // namespace cavr
