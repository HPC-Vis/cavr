#include <cavr/input/button.h>
#include <glog/logging.h>

namespace cavr {

namespace input {

const std::string Button::type_name("Button");

Button::Button()
  : value_(false),
    live_value_(false),
    delta_(Open) {
}

bool Button::pressed() const {
  sync_lock_.readLock();
  bool result = value_;
  sync_lock_.readUnlock();
  return result;
}

Button::Delta Button::delta() const {
  sync_lock_.readLock();
  Delta result = delta_;
  sync_lock_.readUnlock();
  return result;
}

void Button::setState(bool is_pressed) {
  sync_lock_.writeLock();
  live_value_ = is_pressed;
  sync_lock_.writeUnlock();
}

void Button::syncState(bool is_pressed) {
  sync_lock_.writeLock();
  if (is_pressed && value_) {
    delta_ = Held;
  } else if (is_pressed && !value_) {
    delta_ = Pressed;
  } else if (!is_pressed && value_) {
    delta_ = Released;
  } else {
    delta_ = Open;
  }
  value_ = is_pressed;
  sync_lock_.writeUnlock();
}

void Button::sync() {
  live_lock_.readLock();
  bool is_pressed = live_value_;
  live_lock_.readUnlock();
  syncState(is_pressed);
}

} // namespace input

} // namespace cavr
