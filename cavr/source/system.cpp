#include <cavr/system.h>
#include <cavr/config/configuration_specification.h>
#include <cavr/util/environment.h>
#include <cavr/util/file.h>
#include <cavr/util/paths.h>
#include <gflags/gflags.h>
#include <glog/logging.h>

DEFINE_string(cavr_master,
              "",
              "The master cavr machine's key in machines");

namespace cavr {

System::Data System::data_;

bool System::init(int argc, char** argv) {
  // Initialize various systems and variables
  google::ParseCommandLineFlags(&argc, &argv, false);
  google::InitGoogleLogging(argv[0]);
  data_.terminated = false;

  // Get paths where the config files might exist
  std::vector<std::string> config_paths = util::Paths::getConfigPaths();

  // Find the config file
  std::string config_path;
  if (!util::File::find("cavrconfig.lua", config_paths, config_path)) {
    LOG(ERROR) << "Failed to find config file cavrconfig.lua";
    return false;
  }

  // Open the config file
  data_.config_reader.reset(config::LuaReader::createFromFile(config_path));
  if (!data_.config_reader) {
    LOG(ERROR) << "Failed to read config file.";
    return false;
  }

  // Build the global configuration
  std::unique_ptr<config::ConfigurationSpecification> global_spec(
    config::ConfigurationSpecification::createFromSchema("globals.lua",
                                                         "globals"));
  if (!global_spec) {
    LOG(ERROR) << "Failed to open globals schema.";
    return false;
  }

  config::Configuration global_config;
  if (!global_spec->configure(data_.config_reader.get(),
                              "",
                              &global_config)) {
    LOG(ERROR) << "Failed to get global configuration.";
    return false;
  }

  // Setup other machines if we're the master
  std::vector<std::string> machine_config_names =
    global_config.get<std::vector<std::string>>("machines.__keys");

  data_.master = FLAGS_cavr_master.empty();
  std::string hostname = util::Environment::hostname();
  if (data_.master) {
    std::string cavr_master_key;
    for (auto& n : machine_config_names) {
      std::string config_hostname = 
        global_config.get<std::string>("machines." + n + ".hostname");
      if (hostname == config_hostname) {
        cavr_master_key = n;
        break;
      }
    }
    if (cavr_master_key.empty()) {
      LOG(ERROR) << "The machine this is running on is not configured";
      return false;
    }
    std::string cwd = util::Environment::cwd();
    for (auto& n : machine_config_names) {
      if (n == cavr_master_key) {
        continue;
      }
      std::string ssh_target = 
        global_config.get<std::string>("machines." + n + "ssh");
      std::string remote_command =
        "cd " + cwd + " &&";
      for (int i = 0; i < argc; ++i) {
        remote_command += " ";
        remote_command += argv[i];
      }
      pid_t remote_pid = fork();
      if (0 == remote_pid) {
        int result = execlp("ssh",
                            "ssh", 
                            ssh_target.c_str(),
                            remote_command.c_str(),
                            (char*)NULL);
        if (0 != result) {
          LOG(ERROR) << "Failed to start remote process on " << n;
          return false;
        }
      } else {
        data_.remote_pids.push_back(remote_pid);
      }
    }
  }

  // Load plugin generators
  std::string plugins_file_path;
  if (!util::File::find("cavrplugins.lua", config_paths, plugins_file_path)) {
    LOG(ERROR) << "Failed to find plugins file cavrplugins.lua";
    return false;
  }
  std::unique_ptr<config::LuaReader> 
    plugins_file_reader(config::LuaReader::createFromFile(plugins_file_path));
  if (!plugins_file_reader) {
    LOG(ERROR) << "Failed to read plugins file " << plugins_file_path;
    return false;
  }

  std::unique_ptr<config::ConfigurationSpecification> plugins_file_spec(
    config::ConfigurationSpecification::createFromSchema("plugins_file.lua",
                                                         "plugins_file"));
  if (!plugins_file_spec) {
    LOG(ERROR) << "Failed to open plugins_file schema.";
    return false;
  }

  config::Configuration plugins_file_config;
  if (!plugins_file_spec->configure(plugins_file_reader.get(),
                                    "",
                                    &plugins_file_config)) {
    LOG(ERROR) << "Failed to get plugins file data.";
    return false;
  }
  std::vector<std::string> plugin_paths =
    plugins_file_config.get<std::vector<std::string>>("plugins");
  for (auto& n : plugin_paths) {
    PluginGeneratorBase* plugin_generator = loadPlugin(n);
    if (!plugin_generator) {
      LOG(ERROR) << "Failed to load plugin: " << n;
      continue;
    }
    data_.plugin_generators[plugin_generator->name()] = plugin_generator;
  }

  // Find the config for this machine
  for (auto& n : machine_config_names) {
    std::string config_hostname =
      global_config.get<std::string>("machines." + n + ".hostname");
    if (hostname == config_hostname) {
      data_.machine_name = config_hostname;
      break;
    }
  }
  if (data_.machine_name.empty()) {
    LOG(ERROR) << "Machine with hostname " << hostname << " is not configured.";
    return false;
  }

  return true;
}

void System::setCallback(const std::string& name, std::function<void()> f) {
  data_.callbacks[name] = f;
}

std::function<void()> System::getCallback(const std::string& name) {
  auto result = data_.callbacks.find(name);
  if (data_.callbacks.end() == result) {
    LOG(ERROR) << "No callback named " << name;
    return [](){};
  }
  return result->second;
}

void System::run() {
  auto update_function = getCallback("update");
  auto update_thread = [=]() {
    while (!System::terminated()) {
      update_function();
    }
  };
  data_.threads.push_back(std::thread(update_thread));

  for (auto& thread : data_.threads) {
    thread.join();
  }
}

void System::shutdown() {
  data_.terminated = true;
}

bool System::terminated() {
  return data_.terminated;
}

} // namespace cavr
