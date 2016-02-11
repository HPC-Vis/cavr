#include <quat.h>
#include "vrpn.h"
#include <iostream>
using cavr::config::ConfigurationSpecification;

namespace vrpn {

VRPN::VRPN() {
}

bool VRPN::step() {
  for (auto b : buttons_) {
    b->mainloop();
  }
  for (auto a : analogs_) {
    a->mainloop();
  }
  for (auto t : trackers_) {
    t->mainloop();
  }
  return true;
}

bool VRPN::init(cavr::config::Configuration& config) {
  using cavr::config::transform;
  input_name_ = config.get<std::string>("input_name");
  pretransform_ = *config.get<transform>("pretransform").matrix();
  posttransform_ = *config.get<transform>("posttransform").matrix();
  auto button_addresses = config.get<std::vector<std::string>>("buttons");
  auto analog_addresses = config.get<std::vector<std::string>>("analogs");
  auto sixdof_addresses = config.get<std::vector<std::string>>("sixdofs");
  for (auto button_address : button_addresses) {
    vrpn_Button_Remote* b = new vrpn_Button_Remote(button_address.c_str());
    auto data = new CallbackData<cavr::input::Button>();
    std::string server;
    cavr::util::String::split(button_address, "@", data->element_name, server);
    data->vrpn = this;
    b->register_change_handler(data, buttonCallback);
    buttons_.push_back(b);
  }
  for (auto analog_address : analog_addresses) {
    vrpn_Analog_Remote* b = new vrpn_Analog_Remote(analog_address.c_str());
    auto data = new CallbackData<cavr::input::Analog>();
    std::string server;
    cavr::util::String::split(analog_address, "@", data->element_name, server);
    data->vrpn = this;
    b->register_change_handler(data, analogCallback);
    analogs_.push_back(b);
  }
  for (auto sixdof_address : sixdof_addresses) {
    string Bool,sixdof_string;
    cavr::util::String::split(sixdof_address, ":", sixdof_string,Bool);
    vrpn_Tracker_Remote* b = new vrpn_Tracker_Remote(sixdof_string.c_str());
    auto data = new CallbackData<cavr::input::SixDOF>();
    std::string server;
    cavr::util::String::split(sixdof_string, "@", data->element_name, server);
    
    

    data->ReverseMatrix = false;
    if (Bool == "1")
    {
      data->ReverseMatrix = true;
    }
    std::cout << server << std::endl;
    std::cout << data->element_name << std::endl;

    data->vrpn = this;
    b->register_change_handler(data, sixdofCallback);
    trackers_.push_back(b);
  }
  return true;
}

VRPN::~VRPN() {
}

void VRPN::buttonCallback(void* d, const vrpn_BUTTONCB t) {
  using cavr::input::getButton;
  auto data = reinterpret_cast<CallbackData<cavr::input::Button>*>(d);
  auto result = data->input_map.find(t.button);
  if (data->input_map.end() == result) {
    std::string name = 
      data->vrpn->input_name_ + 
      "[" + data->element_name + "[" + std::to_string(t.button) + "]]";
    data->input_map[t.button] = getButton.byDeviceNameOrNull(name);
    result = data->input_map.find(t.button);
  }
  if (result->second) {
    result->second->setState(t.state == 1);
  }
}

void VRPN::analogCallback(void* d, const vrpn_ANALOGCB t) {
  using cavr::input::getAnalog;
  auto data = reinterpret_cast<CallbackData<cavr::input::Analog>*>(d);
  for (int i = 0; i < t.num_channel; ++i) {
    auto result = data->input_map.find(i);
    if (data->input_map.end() == result) {
      std::string name = 
        data->vrpn->input_name_ + 
        "[" + data->element_name + "[" + std::to_string(i) + "]]";
      data->input_map[i] = getAnalog.byDeviceNameOrNull(name);
      result = data->input_map.find(i);
    }
    if (result->second) {
      result->second->setValue(t.channel[i]);
    }
  }
}

void VRPN::sixdofCallback(void* d, const vrpn_TRACKERCB t) {
  using cavr::input::getSixDOF;
  auto data = reinterpret_cast<CallbackData<cavr::input::SixDOF>*>(d);
  auto result = data->input_map.find(t.sensor);
  if (data->input_map.end() == result) {
    std::string name = 
      data->vrpn->input_name_ + 
      "[" + data->element_name + "[" + std::to_string(t.sensor) + "]]";
    data->input_map[t.sensor] = getSixDOF.byDeviceNameOrNull(name);
    result = data->input_map.find(t.sensor);
  }
  if (result->second) {
    q_matrix_type qm;
    if(!data->ReverseMatrix)
    {
      q_to_col_matrix(qm, t.quat);
    }
    else
    {
      q_to_row_matrix(qm, t.quat);
    }
    cavr::math::mat4f m;
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        m[i][j] = qm[i][j];
      }
    }
    m[3].x = t.pos[0];
    m[3].y = t.pos[1];
    m[3].z = t.pos[2];
    result->second->setState(m);
  }
}

} // namespace vrpn

extern "C" {

cavr::PluginGeneratorBase* load() {
  ConfigurationSpecification* spec =
    ConfigurationSpecification::createFromSchema("plugins/vrpn.lua", "vrpn");
  if (nullptr == spec) {
    LOG(ERROR) << "Could not load vrpn spec";
    return nullptr;
  }
  cavr::PluginGeneratorBase* result 
    = new cavr::PluginGenerator<vrpn::VRPN>("vrpn", *spec);
  return result;
}

} // extern "C"
