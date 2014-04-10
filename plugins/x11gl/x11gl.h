#pragma once
#include <cavr/plugin.h>
#include "window.h"
#include <X11/X.h>
#include <X11/Xlib.h>

namespace x11 {

class X11GL : public cavr::Plugin {
public:
  X11GL();
  virtual bool step();
  virtual bool init(cavr::config::Configuration& config);
private:
  std::vector<Window*> windows_;
  std::string display_name_;
  ::Display* display_;
};

} // namespace x11
