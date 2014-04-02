#include <cavr/input/input_manager.h>

namespace cavr {

namespace input {

ManagedInput<Switch> getSwitch;
ManagedInput<Analog> getAnalog;
ManagedInput<Marker> getMarker;
ManagedInput<SixDOF> getSixDOF;

bool reset() {
  bool result = true;
  result &= getSwitch.reset();
  result &= getAnalog.reset();
  result &= getMarker.reset();
  result &= getSixDOF.reset();
  return result;
}

} // namespace input

} // namespace cavr
