#include <algorithm>

#include <cavr/input/analog.h>

namespace cavr {

namespace input {

const std::string Analog::type_name("Analog");

Analog::Analog() {
  value_ = 0;
  live_value_ = 0;
}

double Analog::getValue() const {
  sync_lock_.readLock();
  double result = value_;
  sync_lock_.readUnlock();
  return result;
}

void Analog::setValue(double value) {
  live_lock_.writeLock();
  live_value_ = std::max(-1.0, std::min(value, 1.0));
  live_lock_.writeUnlock();
}

void Analog::sync() {
  sync_lock_.writeLock();
  live_lock_.readLock();
  value_ = live_value_;
  live_lock_.readUnlock();
  sync_lock_.writeUnlock();
}

} // namespace input

} // namespace cavr

