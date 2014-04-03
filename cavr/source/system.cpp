#include <cavr/system.h>
#include <cavr/util/file.h>
#include <cavr/util/paths.h>
#include <gflags/gflags.h>
#include <glog/logging.h>

namespace cavr {

System::Data System::data_;

bool System::init(int argc, char** argv) {
  // Initialize various systems and variables
  google::ParseCommandLineFlags(&argc, &argv, true);
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
