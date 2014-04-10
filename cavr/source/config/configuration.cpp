#include <cavr/config/configuration.h>

namespace cavr {

namespace config {

void Configuration::pushPrefix(const std::string& prefix) {
  if (getPrefix().empty()) {
    access_prefixes_.push(prefix);
  } else {
    access_prefixes_.push(getPrefix() + prefix);
  }
}

const std::string& Configuration::getPrefix() const {
  if (!access_prefixes_.empty()) {
    return access_prefixes_.top();
  }
  static std::string empty("");
  return empty;
}

void Configuration::popPrefix(const std::string& prefix) {
  access_prefixes_.pop();
}

bool Configuration::absorb(Configuration* configuration) {
  for (auto it : configuration->values_) {
    auto result = values_.find(it.first);
    if (result != values_.end()) {
      LOG(ERROR) << it.first << " already exists.";
      return false;
    }
  }
  for (auto it : configuration->values_) {
    values_[it.first] = it.second;
  }
  configuration->values_.clear();
  return true;
}

Configuration::~Configuration() {
  for (auto it : values_) {
    delete it.second;
  }
  values_.clear();
}

} // namespace config

} // namespace cavr
