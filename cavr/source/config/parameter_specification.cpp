#include <cavr/config/parameter_specification.h>

namespace cavr {

namespace config {

ParameterSpecification::ParameterSpecification(ParameterType type,
                                               const std::string& name,
                                               bool is_required)
  : type_(type),
    name_(name),
    required_(is_required) {
}

ParameterSpecification::
ParameterSpecification(const ParameterSpecification& specification) {
  type_ = specification.type();
  name_ = specification.name();
  required_ = specification.required();
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

} // namespace config

} // namespace cavr
