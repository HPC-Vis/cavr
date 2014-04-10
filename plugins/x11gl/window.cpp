#include "window.h"

namespace x11 {

Window* Window::configure(cavr::config::Configuration& config) {
  std::string window_type = config.get<std::string>("view");
  Window* window = nullptr;
  config.pushPrefix("view.");
  if ("perspective_render" == window_type) {
    PerspectiveWindow* p = PerspectiveWindow::configure(config);
    window = p;
  } else if ("simulator_view" == window_type) {
    SimulatorWindow* s = SimulatorWindow::configure(config);
    window = s;
  }
  config.popPrefix("view.");
  window->near_ = config.get<double>("near");
  window->far_ = config.get<double>("far");
  window->width_ = config.get<double>("width");
  window->height_ = config.get<double>("height");
  window->x_position_ = config.get<double>("x");
  window->y_position_ = config.get<double>("y");
  window->fullscreen_ = config.get<bool>("fullscreen");
  window->input_name_ = config.get<std::string>("input_name");
  return window;
}

PerspectiveWindow*
PerspectiveWindow::configure(cavr::config::Configuration& config) {
  PerspectiveWindow* p = new PerspectiveWindow();
  p->lower_left_ = config.get<cavr::input::Marker*>("lower_left");
  p->lower_right_ = config.get<cavr::input::Marker*>("lower_right");
  p->upper_left_ = config.get<cavr::input::Marker*>("upper_left");
  std::string eye_type = config.get<std::string>("eyes");
  if ("stereo" == eye_type) {
    p->eyes_.push_back(config.get<cavr::input::Marker*>("left_eye"));
    p->eyes_.push_back(config.get<cavr::input::Marker*>("right_eye"));
  } else if ("mono" == eye_type) {
    p->eyes_.push_back(config.get<cavr::input::Marker*>("eye"));
  }
  return p;
}

SimulatorWindow*
SimulatorWindow::configure(cavr::config::Configuration& config) {
  SimulatorWindow* s = new SimulatorWindow();
  return s;
}

} // namespace x11
