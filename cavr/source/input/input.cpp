#include <cavr/input/input.h>

namespace cavr {

namespace input {

const std::string& Input::getName() const {
  return name_;
}

const std::string& Input::getDescription() const {
  return description_;
}

void Input::setName(const std::string& name) {
  name_ = name;
}

void Input::setDescription(const std::string& description) {
  description_ = description;
}

} // namespace input

} // namespace cavr
