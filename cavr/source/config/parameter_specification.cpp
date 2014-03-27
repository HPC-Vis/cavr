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

} // namespace config

} // namespace cavr
