#pragma once
#include <cavr/plugin.h>
#include <cavr/math/vector.h>
#include <cavr/input/marker.h>
#include <X11/X.h>
#include <X11/Xlib.h>

namespace x11 {

class Window {
public:
  bool open(::Display* display);
  static Window* configure(cavr::config::Configuration& config);
protected:
  double near_;
  double far_;
  unsigned int width_;
  unsigned int height_;
  unsigned int x_position_;
  unsigned int y_position_;
  bool fullscreen_;
  std::string input_name_;
};

class PerspectiveWindow : public Window {
public:
  static PerspectiveWindow* configure(cavr::config::Configuration& config);
private:
  cavr::input::Marker* lower_left_;
  cavr::input::Marker* lower_right_;
  cavr::input::Marker* upper_left_;
  std::vector<cavr::input::Marker*> eyes_;
};

class SimulatorWindow : public Window {
public:
  static SimulatorWindow* configure(cavr::config::Configuration& config);
private:
};

} // namespace x11
