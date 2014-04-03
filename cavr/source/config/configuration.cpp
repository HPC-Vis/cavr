#include <cavr/config/configuration.h>

namespace cavr {

namespace config {

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
