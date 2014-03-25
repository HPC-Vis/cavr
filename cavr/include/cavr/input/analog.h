#pragma once
#include <cavr/input/input.h>

namespace cavr {

namespace input {

class Analog : public Input {
public:
  Analog(double min = -1.0, double max = 1.0);
  double getMin() const;
  double getMax() const;
  void setMin(double min);
  void setMax(double max);
  double getValue() const;
  void setValue(double value);
private:
  double value_;
  double min_;
  double max_;
};

} // namespace input

} // namespace cavr
