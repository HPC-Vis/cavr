#include <cavr/config/configuration_specification.h>
#include <glog/logging.h>

namespace cavr {

namespace config {

bool ConfigurationSpecification::
addParameter(const ParameterSpecification& parameter) {
  const std::string& name = parameter.name();
  if (parameters_.count(name) > 0) {
    LOG(WARNING) << "Parameter " << name << " is already contained in this"
      " ConfigurationSpecification.";
    return false;
  }
  parameters_.insert({name, parameter});
  return true;
}

const std::map<std::string, ParameterSpecification>& 
ConfigurationSpecification::getMap() const {
  return parameters_;
}

} // namespace config 

} // namespace cavr
