#include <cavr/config/configuration_specification.h>
#include <cavr/config/lua_reader.h>
#include <glog/logging.h>

namespace cavr {

namespace config {

bool ConfigurationSpecification::
addParameter(const ParameterSpecification* parameter) {
  const std::string& name = parameter->name();
  if (parameters_.count(name) > 0) {
    LOG(WARNING) << "Parameter " << name << " is already contained in this"
      " ConfigurationSpecification.";
    return false;
  }
  parameters_.insert({name, parameter->copy()});
  return true;
}

const std::map<std::string, ParameterSpecification*>& 
ConfigurationSpecification::getMap() const {
  return parameters_;
}

ConfigurationSpecification::~ConfigurationSpecification() {
  for (auto it : parameters_) {
    delete it.second;
  }
  parameters_.clear();
}

ConfigurationSpecification*
ConfigurationSpecification::createFromLuaFile(const std::string& path,
                                              const std::string& name) {
  LuaReader* reader = LuaReader::createFromFile(path);
  if (nullptr == reader) {
    LOG(ERROR) << "Failed to load Lua file " << path;
    return nullptr;
  }
  ConfigurationSpecification* spec = createFromLuaReader(reader, name);
  delete reader;
  return spec;
}

ConfigurationSpecification*
ConfigurationSpecification::createFromLuaBuffer(const std::string& buffer,
                                                const std::string& name) {
  LuaReader* reader = LuaReader::createFromBuffer(buffer);
  if (nullptr == reader) {
    LOG(ERROR) << "Failed to load Lua config from buffer.";
    return nullptr;
  }
  ConfigurationSpecification* spec = createFromLuaReader(reader, name);
  delete reader;
  return spec;
}

ConfigurationSpecification*
ConfigurationSpecification::createFromLuaReader(LuaReader* reader,
                                                const std::string& name) {
  std::vector<std::string> parameter_names;
  if(!reader->getKeys(name, parameter_names)) {
    LOG(WARNING) << "Empty specification: " << name;
    return new ConfigurationSpecification();
  }
  bool result = true;
  ConfigurationSpecification* specification = new ConfigurationSpecification();
  for (const auto& parameter_name : parameter_names) {
    bool is_required = false;
    if (!reader->get(name + "." + parameter_name + ".required", is_required)) {
      LOG(ERROR) << "required must be specified for " << parameter_name;
      result = false;
      continue;
    }

    std::string type_name;
    if (!reader->get(name + "." + parameter_name + ".type", type_name)) {
      LOG(ERROR) << "type must be specified for " << parameter_name;
      result = false;
      continue;
    }
    
    ParameterSpecification* parameter = nullptr;
    ParameterType parameter_type;
    if ("number" == type_name) {
      parameter = new Parameter<double>(parameter_name, is_required);
    } else if ("string" == type_name) {
      parameter = new Parameter<std::string>(parameter_name, is_required);
    } else if ("transform" == type_name) {
      parameter = new Parameter<transform>(parameter_name, is_required);
    } else {
      LOG(ERROR) << "parameter type unknown for " << parameter_name;
      result = false;
      continue;
    }
    if (!specification->addParameter(parameter)) {
      LOG(ERROR) << "Failed to add parameter " << parameter_name;
      result = false;
      continue;
    }
  }
  if (!result) {
    LOG(ERROR) << "One or more errors encountered in spec for " << name;
    delete specification;
    return nullptr;
  }
  return specification;
}

} // namespace config 

} // namespace cavr
