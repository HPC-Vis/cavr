#pragma once
#include <cavr/lock.h>
#include <cavr/input/input.h>

namespace cavr {

namespace input {

class Analog : public Input {
public:
  static const std::string type_name;
  Analog();
  double getValue() const;
  void setValue(double value);
  void sync();
private:
  double value_;
  double live_value_;
  mutable Lock live_lock_;
  mutable Lock sync_lock_;
};

} // namespace input

} // namespace cavr
