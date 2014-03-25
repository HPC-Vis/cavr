#include <cavr/input/switch.h>
#include <glog/logging.h>

namespace cavr {

namespace input {

const std::string Switch::out_of_range_ = "OUT OF RANGE";

Switch::Switch(unsigned int num_states)
  : num_states_(num_states) {
  state_names_.resize(num_states_);
  state_descriptions_.resize(num_states_);
  state_ = 0;
}

void Switch::setStateName(unsigned int i, const std::string& name) {
  if (i >= num_states_) {
    LOG(ERROR) << "Invalid state index " << i << " out of " << num_states_;
    return;
  }
  state_names_[i] = name;
}

void Switch::setStateDescription(unsigned int i, 
                                 const std::string& description) {
  if (i >= num_states_) {
    LOG(ERROR) << "Invalid state index " << i << " out of " << num_states_;
    return;
  }
  state_descriptions_[i] = description;
}

unsigned int Switch::getNumberOfStates() const {
  return num_states_;
}

const std::string& Switch::getStateName(unsigned int i) const {
  if (i >= num_states_) {
    LOG(ERROR) << "Invalid state index " << i << " out of " << num_states_;
    return out_of_range_; 
  }
  return state_names_[i];
}

const std::string& Switch::getStateDescription(unsigned int i) const {
  if (i >= num_states_) {
    LOG(ERROR) << "Invalid state index " << i << " out of " << num_states_;
    return out_of_range_;
  }
  return state_descriptions_[i];
}

unsigned int Switch::getState() const {
  return state_;
}

void Switch::setState(unsigned int state) {
  state_ = state;
}

} // namespace input 

} // namespace cavr
