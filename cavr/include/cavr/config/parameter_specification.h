#pragma once
#include <cavr/config/config.h>
#include <cavr/config/parameter_types.h>

#include <string>

namespace cavr {

namespace config {

class ParameterSpecification {
public:
  ParameterSpecification(ParameterType type,
                         const std::string& name,
                         bool is_required);
  ParameterType type() const;
  const std::string& name() const;
  bool required() const;
  bool setDefaultNumber(double number);
  bool setDefaultString(const std::string& string);
  bool setDefaultTransform(const transform& t);
  virtual bool setDefault(const double& value);
  virtual bool setDefault(const std::string& value);
  virtual bool setDefault(const transform& value);
  virtual bool getDefault(double& value) const;
  virtual bool getDefault(std::string& value) const;
  virtual bool getDefault(transform& value) const;
  virtual ParameterSpecification* copy() const = 0;
private:
  ParameterType type_;
  std::string name_;
  bool required_;
};

template<typename T>
class Parameter : public ParameterSpecification {
public:
  Parameter(const std::string& name, bool is_required)
    : ParameterSpecification(parameter_traits<T>::type, name, is_required) {
  }

  virtual bool setDefault(const T& value) {
    default_value_ = value;
    return true;
  }

  virtual ParameterSpecification* copy() const {
    Parameter<T>* result = new Parameter<T>(name(), required());
    result->setDefault(default_value_);
    return result;
  }
private:
  T default_value_;
};

} // namespace config

} // namespace cavr
