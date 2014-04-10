#pragma once
#include <cavr/plugin.h>
#include "window.h"
#include <X11/X.h>
#include <X11/Xlib.h>

#include <mutex>

namespace x11 {

class X11GL : public cavr::Plugin {
public:
  X11GL();
  virtual bool step();
  virtual bool init(cavr::config::Configuration& config);
  static std::mutex& mutex();
  ~X11GL();
private:
  std::vector<Window*> windows_;
  std::string display_name_;
  ::Display* display_;
  static std::mutex x_mutex_;
};

} // namespace x11
