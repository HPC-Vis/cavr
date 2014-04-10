#include "x11gl.h"
using namespace cavr;

namespace x11 {

std::mutex X11GL::x_mutex_;

std::mutex& X11GL::mutex() {
  return x_mutex_;
}

X11GL::X11GL()
  : display_(nullptr) {
}

bool X11GL::init(cavr::config::Configuration& config) {
  display_name_ = config.get<std::string>("display");
  auto window_keys = config.get<std::vector<std::string>>("windows.__keys");
  config.pushPrefix("windows.");
  for (auto window_name : window_keys) {
    config.pushPrefix(window_name + ".");
    Window* window = Window::configure(config);
    config.popPrefix(window_name + ".");
    if (window) {
      windows_.push_back(window);
    } else {
      LOG(ERROR) << "Failed to configure window";
    }
  }
  config.popPrefix("windows.");
  display_ = XOpenDisplay(display_name_.empty()? NULL : display_name_.c_str());
  if (!display_) {
    LOG(ERROR) << "Failed to open display " << display_name_;
    return false;
  }

  bool result = true;
  for (auto window : windows_) {
    result &= window->open(display_);
  }

  return result;
}

bool X11GL::step() {
  for (auto window : windows_) {
    window->update();
  }
  return true;
}

X11GL::~X11GL() {
  for (auto window : windows_) {
    delete window;
  }
  windows_.clear();
  XCloseDisplay(display_);
}

} // namespace x11

extern "C" {

PluginGeneratorBase* load() {
  config::ConfigurationSpecification* spec =
    config::ConfigurationSpecification::createFromSchema("plugins/x11.lua",
                                                         "x11");
  if (nullptr == spec) {
    LOG(ERROR) << "Could not load x11 spec";
    return nullptr;
  }
  PluginGeneratorBase* result = new PluginGenerator<x11::X11GL>("x11gl", *spec);
  return result;
}

} // extern "C"
