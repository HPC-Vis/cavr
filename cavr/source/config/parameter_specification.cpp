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

bool ConfigurationListParameter::configure(Configuration* configuration,
                                           LuaReader* reader,
                                           const std::string& base) {
  std::string path = base + "." + name();
  std::vector<std::string> keys;
  if (!reader->getKeys(path, keys)) {
    LOG(ERROR) << path << " must be a table/list";
    return false;
  }
  for (const auto& key : keys) {
    if (!spec_->configure(reader, path + "." + key, configuration)) {
      LOG(ERROR) << path << "." << key << " is not valid.";
      return false;
    }
  }
  configuration->add(path + ".__keys", keys);
  return true;
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

bool OneOfParameter::configure(Configuration* configuration,
                               LuaReader* reader,
                               const std::string& base) {
  std::string path = base + "." + name();
  bool configured = false;
  std::vector<std::string> matched_choices;
  for (auto it : choices_) {
    Configuration choice_config;
    if (it.second->configure(reader, path, &choice_config)) {
      matched_choices.push_back(it.first);
      if (!configured) {
        configuration->absorb(&choice_config);
        configured = true;
      }
    }
  }
  if (matched_choices.size() > 1) {
    LOG(ERROR) << "Configuration of " << path << " matched multiple choices:";
    for (auto choice : matched_choices) {
      LOG(ERROR) << "  " << choice;
    }
    return false;
  }
  if (!configured) {
    LOG(ERROR) << "Configuration of " << path << " matches no possibilities.";
    return false;
  }
  return true;
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

bool MarkerParameter::configure(Configuration* configuration,
                                LuaReader* reader,
                                const std::string& base) {
  std::string path = base + "." + name();
  input::Marker* marker;
  bool specified = reader->get(path, marker);
  if (!specified) {
    LOG(ERROR) << path << " must be specified.";
    return false;
  }
  configuration->add(path, marker);
  return true;
}

MarkerParameter::~MarkerParameter() {
}

} // namespace config

} // namespace cavr
