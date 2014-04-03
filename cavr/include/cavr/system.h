#pragma once
#include <cavr/config/lua_reader.h>
#include <functional>
#include <map>
#include <string>
#include <thread>
#include <vector>

namespace cavr {

class System {
public:
  enum class SyncMode {
    Replicate,
    MasterSlave
  };
  static bool init(int argc, char** argv);
  static void setCallback(const std::string& name, std::function<void()> f);
  static std::function<void()> getCallback(const std::string& name);
  static void run();
  static void shutdown();
  static bool terminated();
private:
  struct Data {
    std::map<std::string, std::function<void()>> callbacks;
    bool terminated;
    std::vector<std::thread> threads;
    std::unique_ptr<config::LuaReader> config_reader;
  };
  static Data data_;
};

} // namespace cavr
