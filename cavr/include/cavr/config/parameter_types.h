#pragma once

namespace cavr {

namespace config {

enum ParameterType {
  kUnknown = 0,
  kNumber = 1,
  kString = 2,
  kTransform = 3
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

} // namespace config

} // namespace cavr
