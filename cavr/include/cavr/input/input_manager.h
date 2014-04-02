#pragma once
#include <cavr/input/analog.h>
#include <cavr/input/marker.h>
#include <cavr/input/sixdof.h>
#include <cavr/input/switch.h>
#include <glog/logging.h>

#include <map>
#include <string>

namespace cavr {

namespace input {

template<typename T>
class ManagedInput {
public:
  const T* operator()(const std::string& name) const {
    return byAlias(name);
  }
  ManagedInput& operator()() {
    return *this;
  }
  const T* byAlias(const std::string& name) const {
    auto result = by_alias_.find(name);
    if (by_alias_.end() == result) {
      LOG(WARNING) << "Could not find " << T::type_name << " aliased as " <<
        name;
      LOG(WARNING) << "Using dummy " << T::type_name;
    }
    return result->second;
  }
  T* byDeviceName(const std::string& name) {
    auto result = by_device_name_.find(name);
    if (by_device_name_.end() == result) {
      LOG(WARNING) << "Could not find " << T::type_name <<
        " with device name " << name;
      LOG(WARNING) << "Using dummy " << T::type_name;
    }
    return result->second;
  }
  bool reset() {
    for (auto it : by_device_name_) {
      delete it.second;
    }
    by_alias_.clear();
    by_device_name_.clear();
  }
  ~ManagedInput() {
    reset();
  }
private:
  std::map<std::string, const T*> by_alias_;
  std::map<std::string, T*> by_device_name_;
};

extern ManagedInput<Switch> getSwitch;
extern ManagedInput<Analog> getAnalog;
extern ManagedInput<Marker> getMarker;
extern ManagedInput<SixDOF> getSixDOF;
bool reset();

} // namespace input

} // namespace cavr
