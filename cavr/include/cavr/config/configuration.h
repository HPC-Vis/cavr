#pragma once
#include <glog/logging.h>
#include <map>
#include <string>

namespace cavr {

namespace config {

class Configuration {
public:
  template<typename T>
  bool add(const std::string& name, const T& value) {
    if (!name.empty() && name[0] == '.') {
      values_[name.c_str() + 1] = new Value<T>(value);
    } else {
      values_[name] = new Value<T>(value);
    }
    return true;
  }
  template<typename T>
  const T& get(const std::string& name) const {
    auto result = values_.find(name);
    if (values_.end() == result) {
      LOG(ERROR) << "Configuration does not contain " << name;
    }
    const ValueBase* v = result->second;
    return ((const Value<T>*)v)->value;
  }
  bool absorb(Configuration* configuration);
  ~Configuration();
private:
  struct ValueBase {
  };

  template<typename T>
  struct Value : public ValueBase {
    Value(const T& v)
      : value(v) {
    }
    T value;
  };
  std::map<std::string, ValueBase*> values_;
};

} // namespace config

} // namespace cavr
