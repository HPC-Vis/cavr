#pragma once
#include <cavr/config/parameter_types.h>

#include <string>

namespace cavr {

namespace config {

class ParameterSpecification {
public:
  ParameterSpecification(ParameterType type,
                         const std::string& name,
                         bool is_required);
  ParameterSpecification(const ParameterSpecification& specification);
  ParameterType type() const;
  const std::string& name() const;
  bool required() const;
private:
  ParameterType type_;
  std::string name_;
  bool required_;
};

} // namespace config

} // namespace cavr
