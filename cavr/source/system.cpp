#include <cavr/system.h>
#include <cavr/config/configuration_specification.h>
#include <cavr/input/input_manager.h>
#include <cavr/util/environment.h>
#include <cavr/util/file.h>
#include <cavr/util/paths.h>
#include <gflags/gflags.h>
#include <glog/logging.h>

DEFINE_string(cavr_master,
              "",
              "The master cavr machine's key in machines");

namespace cavr {

typedef std::vector<std::string> strvec;

System::Data System::data_;

#if __GNUC__
__thread void* System::context_data_;
#else
void* System::context_data_;
#endif

bool System::init(int argc,
                  char** argv,
                  input::InputMap* input_map) {
  // Initialize various systems and variables
  google::ParseCommandLineFlags(&argc, &argv, false);
  google::InitGoogleLogging(argv[0]);
  com::Communications::initialize();
  data_.terminated = false;

  // Get paths where the config files might exist
  strvec config_paths = util::Paths::getConfigPaths();

  // Find the config file
  std::string config_path;
  if (!util::File::find("cavrconfig.lua", config_paths, config_path)) {
    LOG(ERROR) << "Failed to find config file cavrconfig.lua";
    return false;
  }

  // Open the config file
  std::unique_ptr<config::LuaReader> config_reader;
  config_reader.reset(config::LuaReader::createFromFile(config_path));
  if (!config_reader) {
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
  if (!global_spec->configure(config_reader.get(),
                              "",
                              &global_config)) {
    LOG(ERROR) << "Failed to get global configuration.";
    return false;
  }

  // Setup other machines if we're the master
  strvec machine_config_names =
    global_config.get<strvec>("machines.__keys");
  data_.num_machines = machine_config_names.size();

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
      remote_command += " --cavr_master=";
      remote_command += cavr_master_key;
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
  strvec plugin_paths =
    plugins_file_config.get<strvec>("plugins");
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
      data_.machine_name = n;
      break;
    }
  }
  if (data_.machine_name.empty()) {
    LOG(ERROR) << "Machine with hostname " << hostname << " is not configured.";
    return false;
  }
  std::string machine_prefix = "machines." + data_.machine_name;

  // Load and initialize plugins
  strvec plugin_keys =
    global_config.get<strvec>(machine_prefix + ".plugins.__keys");
  for (auto& n : plugin_keys) {
    std::string plugin_prefix = machine_prefix + ".plugins." + n;
    std::string plugin_type =
      global_config.get<std::string>(plugin_prefix + ".type");
    auto search_result = data_.plugin_generators.find(plugin_type);
    if (data_.plugin_generators.end() == search_result) {
      LOG(ERROR) << "Could not find plugin of type " << plugin_type <<
        " for " << plugin_prefix;
      LOG(ERROR) << "Choices are: ";
      for (auto it : data_.plugin_generators) {
        LOG(ERROR) << "  " << it.first;
      }
      LOG(ERROR) << "Skipping load of " << plugin_prefix;
      continue;
    }
    PluginGeneratorBase* generator = search_result->second;
    const config::ConfigurationSpecification& plugin_spec =
      generator->configuration();
    config::Configuration plugin_config;
    if (!plugin_spec.configure(config_reader.get(),
                               plugin_prefix,
                               &plugin_config)) {
      LOG(ERROR) << "Failed to configure " << plugin_prefix;
      LOG(ERROR) << "Skipping load of " << plugin_prefix;
      continue;
    }
    // Create the plugin
    Plugin* plugin = generator->generate();
    plugin_config.pushPrefix(plugin_prefix + ".");
    if (!plugin->init(plugin_config)) {
      LOG(ERROR) << "Failed to initialize plugin " << plugin_prefix;
      LOG(ERROR) << "Skipping load of " << plugin_prefix;
      delete plugin;
      continue;
    }
    data_.plugins[plugin_prefix] = plugin;
  }

  // Every machine should be running at this point with all potential
  // device inputs registered
  int sync_port = global_config.get<double>("sync_port");
  int data_port = global_config.get<double>("data_port");
  if (data_.master) {
    data_.sync_socket = com::Communications::createReplier(sync_port);
    data_.pubsub_socket = com::Communications::createPublisher(data_port);
  } else {
    std::string master_address = 
      global_config.get<std::string>("machines." + FLAGS_cavr_master + 
                                     ".address");
    data_.sync_socket =
      com::Communications::createRequester(master_address, sync_port);
    data_.pubsub_socket =
      com::Communications::createSubscriber(master_address, data_port);
  }
  if (!data_.sync_socket || !data_.pubsub_socket) {
    LOG(ERROR) << "Failed to setup network.";
    return false;
  }
  if (!input::InputManager::initialize(input_map,
                                       data_.sync_socket,
                                       data_.pubsub_socket,
                                       data_.master,
                                       data_.num_machines)) {
    LOG(ERROR) << "Failed to initialize inputs.";
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
      if (!input::InputManager::sync()) {
        LOG(ERROR) << "Failed to sync InputManager";
        return;
      }
      update_function();
    }
  };
  data_.threads.push_back(std::thread(update_thread));

  for (auto it : data_.plugins) {
    Plugin* plugin = it.second;
    auto thread_func = [plugin]() {
      while (!System::terminated()) {
        plugin->step();
      }
    };
    data_.threads.push_back(std::thread(thread_func));
  }

  for (auto& thread : data_.threads) {
    thread.join();
  }
  
  cleanup();
}

bool System::cleanup() {
  for (auto it : data_.plugins) {
    delete it.second;
  }
  data_.plugins.clear();
}

void System::shutdown() {
  data_.terminated = true;
}

bool System::terminated() {
  return data_.terminated;
}

void* System::getContextData() {
  return context_data_;
}

void System::setContextData(void* data) {
  context_data_ = data;
}

} // namespace cavr
