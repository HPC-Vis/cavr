#pragma once

namespace cavr {

namespace config {

enum ParameterType {
  kUnknown = 0,
  kNumber = 1,
  kString = 2,
  kTransform = 3,
  kStringList = 4,
  kConfigurationList = 5,
  kBoolean = 6,
  kOneOf = 7,
  kMarker = 8
};

template<typename T>
struct parameter_traits {
};

template<>
struct parameter_traits<double> {
  static const ParameterType type = kNumber;
};

template<>
struct parameter_traits<std::string> {
  static const ParameterType type = kString;
};

template<>
struct parameter_traits<transform> {
  static const ParameterType type = kTransform;
};

template<>
struct parameter_traits<std::vector<std::string>> {
  static const ParameterType type = kStringList;
};

template<>
struct parameter_traits<bool> {
  static const ParameterType type = kBoolean;
};

} // namespace config

} // namespace cavr
