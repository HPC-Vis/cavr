#include <cavr/input/input_manager.h>
#include "device_inputs.pb.h"

namespace cavr {

namespace input {

ManagedInput<Button> getButton;
ManagedInput<Analog> getAnalog;
ManagedInput<SixDOF> getSixDOF;

bool reset() {
  bool result = true;
  result &= getButton.reset();
  result &= getAnalog.reset();
  result &= getSixDOF.reset();
  return result;
}

bool initialize(const InputMap* input_map,
                com::Socket* sync_socket,
                com::Socket* pub_socket,
                bool master,
                int num_machines) {
  bool result = true;
  if (input_map) {
    for (const auto it : input_map->button_map) {
      result &= getButton.map(it.first, it.second);
    }
    for (const auto it : input_map->analog_map) {
      result &= getAnalog.map(it.first, it.second);
    }
    for (const auto it : input_map->sixdof_map) {
      result &= getSixDOF.map(it.first, it.second);
    }
  }
  if (!result) {
    LOG(ERROR) << "An error occurred mapping inputs";
    return false;
  }
  auto add_device_inputs = [=](com::DeviceInputs& di) {
    for (int j = 0; j < di.buttons_size(); ++j) {
      input::getButton.byDeviceNameOrCreate(di.buttons(j));
    }
    for (int j = 0; j < di.analogs_size(); ++j) {
      input::getAnalog.byDeviceNameOrCreate(di.analogs(j));
    }
    for (int j = 0; j < di.sixdofs_size(); ++j) {
      input::getSixDOF.byDeviceNameOrCreate(di.sixdofs(j));
    }
  };
  auto build_device_inputs = [=](com::DeviceInputs& di) {
    std::vector<std::string> buttons = getButton.deviceNames();
    for (auto& n : buttons) {
      di.add_buttons(n);
    }
    std::vector<std::string> analogs = getAnalog.deviceNames();
    for (auto& n : analogs) {
      di.add_analogs(n);
    }
    std::vector<std::string> sixdofs = getSixDOF.deviceNames();
    for (auto& n : sixdofs) {
      di.add_sixdofs(n);
    }
  };
  if (master) {
    for (int i = 0; i < num_machines - 1; ++i) {
      std::string packet;
      if (!sync_socket->recv(packet)) {
        LOG(ERROR) << "Failed to receive device inputs";
        return false;
      }
      com::DeviceInputs di;
      di.ParseFromString(packet);
      add_device_inputs(di);
      if (!sync_socket->send("")) {
        LOG(ERROR) << "Failed to send sync signal";
        return false;
      }
    }
    com::DeviceInputs di;
    build_device_inputs(di);
    std::string packet;
    di.SerializeToString(&packet);
    if (!pub_socket->send(packet)) {
      LOG(ERROR) << "Failed to publish synchronized device inputs";
      return false;
    }
  } else { // slave
    com::DeviceInputs di;
    build_device_inputs(di);
    std::string packet;
    di.SerializeToString(&packet);
    if (!sync_socket->send(packet)) {
      LOG(ERROR) << "Worker failed to send its device inputs";
      return false;
    }
    if (!sync_socket->recv(packet)) {
      LOG(ERROR) << "Worker failed to recv ack for device inputs";
      return false;
    }
    di.Clear();
    if (!pub_socket->recv(packet)) {
      LOG(ERROR) << "Failed to recv complete device inputs";
      return false;
    }
    di.ParseFromString(packet);
    add_device_inputs(di);
  }
  // at this point, all maps should be exactly the same
  return true;
}

} // namespace input

} // namespace cavr
