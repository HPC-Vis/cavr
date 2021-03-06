#include <cavr/input/input_manager.h>
#include "device_inputs.pb.h"

namespace cavr {

namespace input {

InputManager::Data InputManager::data_;

ManagedInput<Button> getButton;
ManagedInput<Analog> getAnalog;
ManagedInput<SixDOF> getSixDOF;

bool InputManager::reset() {
  bool result = true;
  result &= getButton.reset();
  result &= getAnalog.reset();
  result &= getSixDOF.reset();
  return result;
}

bool InputManager::mapInputs(const InputMap* input_map) {
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
  return result;
}

bool InputManager::initialize(com::Socket* sync_socket,
                              com::Socket* pub_socket,
                              bool master,
                              int num_machines) {
  data_.sync_socket = sync_socket;
  data_.pub_socket = pub_socket;
  data_.master = master;
  data_.num_machines = num_machines;
  bool result = true;
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
      //exit(0);
      std::string packet;
      if (!sync_socket->recv(packet)) {
        LOG(ERROR) << "Failed to receive device inputs";
        return false;
      }
      com::DeviceInputs di;
      di.ParseFromString(packet);
      add_device_inputs(di);
      if (!sync_socket->send(" ")) {
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
  std::vector<std::string> button_names = getButton.deviceNames();
  for (auto n : button_names) {
    data_.buttons.push_back(getButton.byDeviceName(n));
  }
  std::vector<std::string> analog_names = getAnalog.deviceNames();
  for (auto n : analog_names) {
    data_.analogs.push_back(getAnalog.byDeviceName(n));
  }
  std::vector<std::string> sixdof_names = getSixDOF.deviceNames();
  for (auto n : sixdof_names) {
    data_.sixdofs.push_back(getSixDOF.byDeviceName(n));
  }
  data_.last_time = std::chrono::high_resolution_clock::now();
  return true;
}

bool InputManager::sync() {
  if (data_.master) {
    for (int i = 0; i < data_.num_machines - 1; ++i) {
      std::string sync_packet = " ";
      if (!data_.sync_socket->recv(sync_packet)) {
        LOG(ERROR) << "Failed to recv sync";
        return false;
      }
      else
      {
        //LOG(ERROR) << "HERE NOW";
        if(!data_.sync_socket->send(sync_packet))
        {
          LOG(ERROR) << "INSERT WIN MESSAGE NOT";
          return false;
        } 
      }
   
    }
  } else {
    std::string sync_packet=" ";
    if (!data_.sync_socket->send(sync_packet)) {
      LOG(ERROR) << "Failed to send sync";
      return false;
    }
    if(!data_.sync_socket->recv(sync_packet))
    {
         LOG(ERROR) << "INSERT FAIL MESSAGE HERE";
	 return false;
    }
  }

  com::DeviceSync device_sync;
  if (data_.master) {
    auto current_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> dt = current_time - data_.last_time;
    data_.last_time = current_time;
    data_.dt = dt.count();
    device_sync.set_dt(data_.dt);
    if (data_.master) {
      device_sync.set_user_data(data_.sync_data);
    }
    for (auto button : data_.buttons) {
      button->sync();
      device_sync.add_buttons(button->pressed());
    }
    for (auto analog : data_.analogs) {
      analog->sync();
      device_sync.add_analogs(analog->getValue());
    }
    for (auto sixdof : data_.sixdofs) {
      sixdof->sync();
      const auto& m = sixdof->getMatrix();
      for (int i = 0; i < 16; ++i) {
        device_sync.add_sixdofs(m.v[i]);
      }
    }
    std::string packet;
    device_sync.SerializeToString(&packet);
    if (!data_.pub_socket->send(packet)) {
      LOG(ERROR) << "Failed to publish device sync";
      return false;
    }
  } else {
    std::string packet;
    if (!data_.pub_socket->recv(packet)) {
      LOG(ERROR) << "Failed to receive device sync";
      return false;
    }
    else
    {
    //LOG(ERROR) << "RECIEVED";
    //LOG(ERROR) << packet;
    }
    device_sync.ParseFromString(packet);
    data_.dt = device_sync.dt();
    data_.sync_data = device_sync.user_data();
    for (int i = 0; i < device_sync.buttons_size(); ++i) {
      data_.buttons[i]->syncState(device_sync.buttons(i));
    }
    for (int i = 0; i < device_sync.analogs_size(); ++i) {
      data_.analogs[i]->syncState(device_sync.analogs(i));
    }
    //LOG(ERROR) << device_sync.sixdofs_size();
    for (int i = 0; i < device_sync.sixdofs_size() / 16; ++i) {
      math::mat4d m;
      int k = i * 16;
      for (int j = 0; j < 16; ++j) {
        m.v[j] = device_sync.sixdofs(k + j);
      }
      data_.sixdofs[i]->syncState(m);
    }
  }
  return true;
}

double InputManager::dt() {
  return data_.dt;
}


void InputManager::setSyncData(const std::string& data) {
  data_.sync_data = data;
}

const std::string& InputManager::getSyncData() {
  return data_.sync_data;
}


} // namespace input

} // namespace cavr
