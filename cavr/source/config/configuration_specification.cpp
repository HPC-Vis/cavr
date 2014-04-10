#include <cavr/config/configuration_specification.h>
#include <cavr/config/lua_reader.h>
#include <cavr/util/file.h>
#include <cavr/util/paths.h>
#include <glog/logging.h>
#include <memory>

namespace cavr {

namespace config {

ConfigurationSpecification::
ConfigurationSpecification(const ConfigurationSpecification& cs) {
  for (const auto it : cs.parameters_) {
    parameters_[it.first] = it.second->copy();
  }
}

ConfigurationSpecification&
ConfigurationSpecification::operator=(const ConfigurationSpecification& cs) {
  for (auto it : parameters_) {
    delete it.second;
  }
  parameters_.clear();
  for (const auto it : cs.parameters_) {
    parameters_[it.first] = it.second->copy();
  }
  return *this;
}

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

bool ConfigurationSpecification::configure(LuaReader* reader,
                                           const std::string& name,
                                           Configuration* configuration) const {
  bool result = true;
  for (auto it : parameters_) {
    if (!it.second->configure(configuration, reader, name)) {
      LOG(INFO) << "Failed to configure " << name << "." << it.first;
      result = false;
    }
  }
  return result;
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
ConfigurationSpecification::createFromSchema(const std::string& path,
                                             const std::string& name) {
  std::vector<std::string> schema_paths = util::Paths::getSchemaPaths();
  std::string schema_path;
  if (!util::File::find(path, schema_paths, schema_path)) {
    LOG(ERROR) << "Could not find schema file " << path;
    return nullptr;
  }
  return createFromLuaFile(schema_path, name);
}

template<typename T>
bool setDefault(LuaReader* reader,
                const std::string& prefix,
                ParameterSpecification* parameter) {
  T default_value;
  if (!reader->get(prefix + ".default", default_value)) {
    LOG(ERROR) << "default must be specified for " << prefix;
    return false;
  }
  parameter->setDefault(default_value);
  return true;
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
    std::string prefix = name + "." + parameter_name;
    bool is_required = false;
    if (!reader->get(prefix + ".required", is_required)) {
      LOG(ERROR) << "required must be specified for " << parameter_name;
      result = false;
      continue;
    }
    std::string type_name;
    if (!reader->get(prefix + ".type", type_name)) {
      LOG(ERROR) << "type must be specified for " << parameter_name;
      result = false;
      continue;
    }
    std::unique_ptr<ParameterSpecification> parameter = nullptr;
    if ("boolean" == type_name) {
      parameter.reset(new Parameter<bool>(parameter_name, is_required));
      result &= 
        is_required || setDefault<bool>(reader, prefix, parameter.get());
    } else if ("number" == type_name) {
      parameter.reset(new Parameter<double>(parameter_name, is_required));
      result &= 
        is_required || setDefault<double>(reader, prefix, parameter.get());
    } else if ("string" == type_name) {
      parameter.reset(new Parameter<std::string>(parameter_name, is_required));
      result &= 
        is_required || setDefault<std::string>(reader, prefix, parameter.get());
    } else if ("transform" == type_name) {
      parameter.reset(new Parameter<transform>(parameter_name, is_required));
      result &= 
        is_required || setDefault<transform>(reader, prefix, parameter.get());
    } else if ("string_list" == type_name) {
      parameter.reset(new Parameter<std::vector<std::string>>(parameter_name,
                                                              is_required));
      result &=
        is_required || setDefault<std::vector<std::string>>(reader,
                                                            prefix,
                                                            parameter.get());
    } else if ("marker" == type_name) {
      parameter.reset(new MarkerParameter(parameter_name, is_required));
    } else if ("list" == type_name) {
      ConfigurationSpecification* subspec =
        createFromLuaReader(reader, prefix + ".subtype");
      if (subspec) {
        parameter.reset(new ConfigurationListParameter(parameter_name,
                                                       is_required,
                                                       subspec));
        delete subspec;
      } else {
        LOG(ERROR) << "Failed to read subtype for list of " << parameter_name;
        result = false;
      }
    } else if ("one_of" == type_name) {
      std::vector<std::string> type_keys;
      if (reader->getKeys(prefix + ".possibilities", type_keys)) {
        std::map<std::string, ConfigurationSpecification*> choices;
        for (const auto& type : type_keys) {
          ConfigurationSpecification* subspec =
            createFromLuaReader(reader, prefix + ".possibilities." + type);
          if (subspec) {
            choices[type] = subspec;
          } else {
            LOG(ERROR) << "Failed to read subconfiguration " << type;
            result = false;
          }
        }
        parameter.reset(new OneOfParameter(parameter_name,
                                           is_required,
                                           choices));
      }
    } else {
      LOG(ERROR) << "parameter type unknown for " << parameter_name;
      result = false;
    }
    if (parameter && !specification->addParameter(parameter.get())) {
      LOG(ERROR) << "Failed to add parameter " << parameter_name;
      result = false;
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
