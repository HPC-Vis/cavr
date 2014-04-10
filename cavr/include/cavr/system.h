#pragma once
#include <cavr/plugin.h>
#include <cavr/com/communications.h>
#include <cavr/config/lua_reader.h>
#include <cavr/input/input_map.h>
#include <cavr/input/specification.h>
#include <functional>
#include <map>
#include <string>
#include <thread>
#include <vector>
#include <unistd.h>

namespace cavr {

class System {
public:
  enum class SyncMode {
    Replicate,
    MasterSlave
  };
  static bool init(int argc,
                   char** argv, 
                   input::InputMap* input_map);
  static void setCallback(const std::string& name, std::function<void()> f);
  static std::function<void()> getCallback(const std::string& name);
  static void run();
  static void shutdown();
  static bool terminated();
private:
  struct Data {
    std::map<std::string, std::function<void()>> callbacks;
    bool terminated;
    bool master;
    std::vector<std::thread> threads;
    std::vector<pid_t> remote_pids;
    std::string machine_name;
    std::map<std::string, PluginGeneratorBase*> plugin_generators;
    std::map<std::string, Plugin*> plugins;
    int num_machines;
    com::Socket* sync_socket;
    com::Socket* pubsub_socket;
  };
  static Data data_;
};

} // namespace cavr
