#pragma once
#include <cavr/lock.h>
#include <cavr/input/input.h>

namespace cavr {

namespace input {

class Button : public Input {
public:
  static const std::string type_name;
  enum Delta {
    Open = 0,
    Pressed = 1,
    Held = 2,
    Released = 3
  };
  Button();
  bool pressed() const;
  Delta delta() const;
  void setState(bool is_pressed);
  void syncState(bool is_pressed);
  void sync();
private:
  mutable cavr::Lock sync_lock_;
  mutable cavr::Lock live_lock_;
  bool live_value_;
  bool value_;
  Delta delta_;
};

} // namespace input

} // namespace cavr
