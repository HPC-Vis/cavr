#include <GL/gl.h>
#include <GL/glx.h>
#include "x11gl.h"
using namespace cavr;

namespace x11 {

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
  }
  config.popPrefix("windows.");
  display_ = XOpenDisplay(display_name_.empty()? NULL : display_name_.c_str());
  if (!display_) {
    LOG(ERROR) << "Failed to open display " << display_name_;
    return false;
  }

  std::vector<int> visual_attribs( {
    GLX_X_RENDERABLE, True,
    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
    GLX_RENDER_TYPE, GLX_RGBA_BIT,
    GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
    GLX_RED_SIZE, 8,
    GLX_GREEN_SIZE, 8,
    GLX_BLUE_SIZE, 8,
    GLX_ALPHA_SIZE, 8,
    GLX_DEPTH_SIZE, 24,
    GLX_STENCIL_SIZE, 8,
    GLX_DOUBLEBUFFER, True 
  });

  visual_attribs.push_back(None);
  return true;
}

bool X11GL::step() {
  return true;
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
