#pragma once
#include <cavr/config/config.h>
#include <cavr/config/configuration.h>
#include <cavr/config/lua_reader.h>
#include <cavr/config/parameter_types.h>

#include <map>
#include <string>
#include <vector>

namespace cavr {

namespace config {

class ConfigurationSpecification;

class ParameterSpecification {
public:
  ParameterSpecification(ParameterType type,
                         const std::string& name,
                         bool is_required);
  ParameterType type() const;
  const std::string& name() const;
  bool required() const;

  virtual bool setDefault(const bool& value);
  virtual bool setDefault(const double& value);
  virtual bool setDefault(const std::string& value);
  virtual bool setDefault(const transform& value);
  virtual bool setDefault(const std::vector<std::string>& value);

  virtual bool getDefault(bool& value) const;
  virtual bool getDefault(double& value) const;
  virtual bool getDefault(std::string& value) const;
  virtual bool getDefault(transform& value) const;
  virtual bool getDefault(std::vector<std::string>& value) const;

  virtual ParameterSpecification* copy() const = 0;
  virtual bool configure(Configuration* configuration,
                         LuaReader* reader,
                         const std::string& base) = 0;
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

  virtual bool getDefault(T& value) const {
    value = default_value_;
    return true;
  }

  virtual ParameterSpecification* copy() const {
    Parameter<T>* result = new Parameter<T>(name(), required());
    result->setDefault(default_value_);
    return result;
  }

  virtual bool configure(Configuration* configuration,
                         LuaReader* reader,
                         const std::string& base) {
    std::string path = base + "." + name();
    T value;
    bool specified = reader->get(path, value);
    if (!specified && required()) {
      LOG(ERROR) << path << " must be specified.";
      return false;
    }
    if (!specified) {
      value = default_value_;
    }
    configuration->add(path, value);
    return true;
  }
private:
  T default_value_;
};

class ConfigurationListParameter : public ParameterSpecification {
public:
  ConfigurationListParameter(const std::string& name,
                             bool is_required,
                             const class ConfigurationSpecification* cs);
  virtual ParameterSpecification* copy() const;
  virtual bool configure(Configuration* configuration,
                         LuaReader* reader,
                         const std::string& base);
  virtual ~ConfigurationListParameter();
private:
  class ConfigurationSpecification* spec_;
};

class OneOfParameter : public ParameterSpecification {
public:
  OneOfParameter(const std::string& name,
                 bool is_required,
                 const std::map<std::string,
                                class ConfigurationSpecification*> choices);
  virtual ParameterSpecification* copy() const;
  virtual bool configure(Configuration* configuration,
                         LuaReader* reader,
                         const std::string& base);
  virtual ~OneOfParameter();
private:
  std::map<std::string, class ConfigurationSpecification*> choices_;
};

class MarkerParameter : public ParameterSpecification {
public:
  MarkerParameter(const std::string& name, bool is_required);
  virtual ParameterSpecification* copy() const;
  virtual bool configure(Configuration* configuration,
                         LuaReader* reader,
                         const std::string& base);
  virtual ~MarkerParameter();
};

} // namespace config

} // namespace cavr
