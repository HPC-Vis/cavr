#pragma once
#include <string>
#include <vector>
#include <cavr/input/input.h>

namespace cavr {

namespace input {

class Switch : public Input {
public:
  static constexpr char type_name[] = "Switch";
  Switch(unsigned int num_states = 1);
  void setStateName(unsigned int i, const std::string& name);
  void setStateDescription(unsigned int i, const std::string& description);
  unsigned int getNumberOfStates() const;
  const std::string& getStateName(unsigned int i) const;
  const std::string& getStateDescription(unsigned int i) const;
  unsigned int getState() const;
  void setState(unsigned int state);
private:
  std::vector<std::string> state_names_;
  std::vector<std::string> state_descriptions_;
  unsigned int num_states_;
  unsigned int state_;
  static const std::string out_of_range_;
};

} // namespace input

} // namespace cavr
