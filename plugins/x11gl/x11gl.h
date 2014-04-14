#pragma once
#include <cavr/plugin.h>
#include <cavr/input/input_manager.h>
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
  void processEvents();
  static std::mutex& mutex();
  ~X11GL();
private:
  std::vector<Window*> stereo_windows_;
  ::GLXContext stereo_context_;
  void* stereo_context_data_;

  std::vector<Window*> mono_windows_;
  ::GLXContext mono_context_;
  void* mono_context_data_;

  std::string display_name_;
  ::Display* display_;
  static std::mutex x_mutex_;

  std::string input_name_;
  std::map< ::Window, Window*> window_map_;
  std::map<KeySym, cavr::input::Button*> button_by_key_;

  std::function<void()> update_context_callback_;
  std::function<void()> destruct_context_callback_;
};

} // namespace x11
