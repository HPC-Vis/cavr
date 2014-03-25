#include <algorithm>

#include <cavr/input/analog.h>

namespace cavr {

namespace input {

Analog::Analog(double min, double max) 
  : min_(min),
    max_(max) {
  setValue(0);
}

double Analog::getMin() const {
  return min_;
}

double Analog::getMax() const {
  return max_;
}

void Analog::setMin(double min) {
  min_ = min;
  setValue(value_);
}

void Analog::setMax(double max) {
  max_ = max;
  setValue(value_);
}

double Analog::getValue() const {
  return value_;
}

void Analog::setValue(double value) {
  value_ = std::max(min_, std::min(value, max_));
}

} // namespace input

} // namespace cavr

