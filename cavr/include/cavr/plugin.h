#pragma once
#include <cavr/config/configuration.h>
#include <cavr/config/configuration_specification.h>

namespace cavr {

class Plugin {
public:
  virtual bool step() = 0;
  virtual bool init(const config::Configuration& config) = 0;
private:
  config::ConfigurationSpecification* config_spec_;
};

class PluginGeneratorBase {
public:
  PluginGeneratorBase();
  virtual const std::string& name() const = 0;
  virtual const config::ConfigurationSpecification& configuration() const = 0;
  virtual Plugin* generate() const = 0;
  void setHandle(void* handle);
  virtual ~PluginGeneratorBase();
private:
  void* handle_;
};

template<typename T>
class PluginGenerator : public PluginGeneratorBase {
public:
  PluginGenerator(const std::string& name,
                  const config::ConfigurationSpecification& spec)
    : name_(name),
      spec_(spec) {
  }

  const std::string& name() const {
    return name_;
  }
  
  const config::ConfigurationSpecification& configuration() const {
    return spec_;
  }

  T* generate() const {
    return new T;
  }
private:
  std::string name_;
  config::ConfigurationSpecification spec_;
};

PluginGeneratorBase* loadPlugin(const std::string& path);

} // namespace cavr
