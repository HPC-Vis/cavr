#pragma once
#include <mutex>
#include <string>
#include <vector>

namespace cavr {

namespace input {

class Input {
public:
  const std::string& getName() const;
  const std::string& getDescription() const;
  void setName(const std::string& name);
  void setDescription(const std::string& description);
protected:
  Input() = default;
private:
  std::string name_;
  std::string description_;
};

} // namespace input

} // namespace cavr
