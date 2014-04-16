#pragma once
#include <cavr/plugin.h>
#include <cavr/input/input_manager.h>

#include <vrpn_Button.h>
#include <vrpn_Analog.h>
#include <vrpn_Tracker.h>

namespace vrpn {

class VRPN : public cavr::Plugin {
public:
  VRPN();
  virtual bool step();
  virtual bool init(cavr::config::Configuration& config);
  ~VRPN();
  static void buttonCallback(void*, const vrpn_BUTTONCB t);
  static void analogCallback(void*, const vrpn_ANALOGCB t);
  static void sixdofCallback(void*, const vrpn_TRACKERCB t);
private:
  std::string input_name_;
  std::vector<vrpn_Button_Remote*> buttons_;
  std::vector<vrpn_Analog_Remote*> analogs_;
  std::vector<vrpn_Tracker_Remote*> trackers_;
  cavr::math::mat4f pretransform_;
  cavr::math::mat4f posttransform_;

  template<typename T>
  struct CallbackData {
    std::map<int, T*> input_map;
    std::string element_name;
    VRPN* vrpn;
  };
};

} // namespace vrpn
