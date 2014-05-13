#pragma once
#include <cavr/com/communications.h>
#include <cavr/input/analog.h>
#include <cavr/input/button.h>
#include <cavr/input/input_map.h>
#include <cavr/input/sixdof.h>
#include <cavr/input/switch.h>
#include <glog/logging.h>

#include <chrono>
#include <map>
#include <string>

namespace cavr {

namespace input {

template<typename T>
class ManagedInput {
public:
  T* operator()(const std::string& name) {
    return byAlias(name);
  }

  ManagedInput& operator()() {
    return *this;
  }

  T* byAlias(const std::string& name) {
    auto result = by_alias_.find(name);
    if (by_alias_.end() == result) {
      LOG(WARNING) << "Could not find " << T::type_name << " aliased as " <<
        name;
      LOG(WARNING) << "Using dummy " << T::type_name;
      return &dummy_;
    }
    return result->second;
  }

  T* byDeviceNameOrNull(const std::string& name) {
    auto result = by_device_name_.find(name);
    if (by_device_name_.end() == result) {
      return nullptr;
    }
    return result->second;
  }

  T* byDeviceName(const std::string& name) {
    T* result = byDeviceNameOrNull(name);
    if (!result) {
      LOG(WARNING) << "Could not find " << T::type_name <<
        " with device name " << name;
      LOG(WARNING) << "Using dummy " << T::type_name;
      return &dummy_;
    }
    return result;
  }

  T* byDeviceNameOrCreate(const std::string& name) {
    auto result = by_device_name_.find(name);
    if (by_device_name_.end() != result) {
      return result->second;
    }
    T* t = new T();
    t->setName(name);
    by_device_name_[name] = t;
    return t;
  }

  bool reset() {
    for (auto it : by_device_name_) {
      delete it.second;
    }
    by_alias_.clear();
    by_device_name_.clear();
  }

  bool map(const std::string& alias, const std::string& device_name) {
    T* device_t = byDeviceNameOrCreate(device_name);
    if (by_alias_.count(alias) != 0) {
      LOG(ERROR) << T::type_name << " alias " << alias << " is already mapped.";
      return false;
    }
    by_alias_[alias] = device_t;
    return true;
  }

  std::vector<std::string> deviceNames() const {
    std::vector<std::string> results;
    for (const auto it : by_device_name_) {
      results.push_back(it.first);
    }
    return results;
  }

  ~ManagedInput() {
    reset();
  }
private:
  std::map<std::string, T*> by_alias_;
  std::map<std::string, T*> by_device_name_;
  T dummy_;
};

extern ManagedInput<Button> getButton;
extern ManagedInput<Analog> getAnalog;
extern ManagedInput<SixDOF> getSixDOF;

class InputManager {
public:
  static bool reset();
  static bool mapInputs(const InputMap* input_map);
  static bool initialize(com::Socket* sync_socket,
                         com::Socket* pub_socket,
                         bool master,
                         int num_machines);
  static bool sync();
  static double dt();
  static void setSyncData(const std::string& data);
  static const std::string& getSyncData();
private:
  struct Data {
    com::Socket* sync_socket;
    com::Socket* pub_socket;
    bool master;
    int num_machines;
    std::vector<Button*> buttons;
    std::vector<Analog*> analogs;
    std::vector<SixDOF*> sixdofs;
    double dt;
    std::chrono::time_point<std::chrono::high_resolution_clock> last_time;
    std::string sync_data;
  };
  static Data data_;
};

} // namespace input

} // namespace cavr
