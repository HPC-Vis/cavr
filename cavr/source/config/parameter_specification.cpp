#include <cavr/config/configuration_specification.h>
#include <cavr/config/parameter_specification.h>
#include <glog/logging.h>

namespace cavr {

namespace config {

ParameterSpecification::ParameterSpecification(ParameterType type,
                                               const std::string& name,
                                               bool is_required)
  : type_(type),
    name_(name),
    required_(is_required) {
}

ParameterType ParameterSpecification::type() const {
  return type_;
}

const std::string& ParameterSpecification::name() const {
  return name_;
}

bool ParameterSpecification::required() const {
  return required_;
}

bool ParameterSpecification::setDefault(const bool& value) {
  LOG(WARNING) << name_ << " is not a boolean.";
  return false;
}

bool ParameterSpecification::setDefault(const double& number) {
  LOG(WARNING) << name_ << " is not a number.";
  return false;
}

bool ParameterSpecification::setDefault(const std::string& string) {
  LOG(WARNING) << name_ << " is not a string.";
  return false;
}

bool ParameterSpecification::setDefault(const transform& t) {
  LOG(WARNING) << name_ << " is not a tranform.";
  return false;
}

bool ParameterSpecification::setDefault(const std::vector<std::string>& value) {
  LOG(WARNING) << name_ << " is not a string list.";
  return false;
}

bool ParameterSpecification::getDefault(double& value) const {
  LOG(WARNING) << name_ << " is not a number.";
  return false;
}

bool ParameterSpecification::getDefault(std::string& value) const {
  LOG(WARNING) << name_ << " is not a string.";
  return false;
}

bool ParameterSpecification::getDefault(transform& value) const {
  LOG(WARNING) << name_ << " is not a number.";
  return false;
}

bool ParameterSpecification::getDefault(std::vector<std::string>& value) const {
  LOG(WARNING) << name_ << " is not a string list.";
  return false;
}

bool ParameterSpecification::getDefault(bool& value) const {
  LOG(WARNING) << name_ << " is not a boolean.";
  return false;
}

ConfigurationListParameter::
ConfigurationListParameter(const std::string& name,
                           bool is_required,
                           const class ConfigurationSpecification* cs)
  : ParameterSpecification(kConfigurationList, name, is_required) {
  spec_ = new ConfigurationSpecification(*cs);
}

ParameterSpecification* ConfigurationListParameter::copy() const {
  ConfigurationListParameter* result =
    new ConfigurationListParameter(name(), required(), spec_);
  return result;
}

ConfigurationListParameter::~ConfigurationListParameter() {
  if (spec_) {
    delete spec_;
  }
}

OneOfParameter::
OneOfParameter(const std::string& name,
               bool is_required,
               const std::map<std::string,
                              ConfigurationSpecification*> choices) 
  : ParameterSpecification(kOneOf, name, is_required) {
  for (auto it : choices) {
    choices_[it.first] = it.second;
  }
}

ParameterSpecification* OneOfParameter::copy() const {
  OneOfParameter* result = new OneOfParameter(name(), required(), choices_);
  return result;
}

OneOfParameter::~OneOfParameter() {
  for (auto it : choices_) {
    delete it.second;
  }
  choices_.clear();
}

MarkerParameter::MarkerParameter(const std::string& name, bool is_required) 
  : ParameterSpecification(kMarker, name, is_required) {
}

ParameterSpecification* MarkerParameter::copy() const {
  return new MarkerParameter(name(), required());
}

MarkerParameter::~MarkerParameter() {
}

} // namespace config

} // namespace cavr
