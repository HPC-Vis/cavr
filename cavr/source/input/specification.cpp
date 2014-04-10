#include <cavr/config/configuration_specification.h>
#include <cavr/input/specification.h>

#include <memory>

using cavr::config::ConfigurationSpecification;

namespace cavr {

namespace input {

Specification* Specification::createFromLuaFile(const std::string& path) {
  std::unique_ptr<ConfigurationSpecification>
    inputs_file_spec(ConfigurationSpecification::
                      createFromSchema("inputs_file.lua", "inputs_file"));
  if (!inputs_file_spec) {
    LOG(ERROR) << "Could not load inputs file schema.";
    return nullptr;
  }
  std::unique_ptr<config::LuaReader>
    inputs_reader(config::LuaReader::createFromFile(path));
  if (!inputs_reader) {
    LOG(ERROR) << "Failed to open inputs file " << path;
    return nullptr;
  }

  config::Configuration configuration;
  if (!inputs_file_spec->configure(inputs_reader.get(), "", &configuration)) {
    LOG(ERROR) << "Failed to configure inputs file " << path;
    return nullptr;
  }

  Specification* specification = new Specification();
  auto switch_keys =
    configuration.get<std::vector<std::string>>("switches.__keys");
  for (const auto& k : switch_keys) {
    std::string prefix = "switches." + k;
    auto state_keys =
      configuration.get<std::vector<std::string>>(prefix + ".states.__keys");
    std::vector<std::string> state_names;
    std::vector<std::string> state_descriptions;
    for (const auto& s : state_keys) {
      std::string p = prefix + ".states." + s;
      state_names.push_back(configuration.get<std::string>(p + ".name"));
      state_descriptions.
        push_back(configuration.get<std::string>(p + ".description"));
    }
    Switch* input_switch = new Switch(state_names.size());
    input_switch->setName(configuration.get<std::string>(prefix + ".name"));
    input_switch->
      setDescription(configuration.get<std::string>(prefix + ".description"));
    for (size_t i = 0; i < state_names.size(); ++i) {
      input_switch->setStateName(i, state_names[i]);
      input_switch->setStateDescription(i, state_descriptions[i]);
    }
    input_switch->setState(0);
    specification->switches[input_switch->getName()] = input_switch;
  }

  auto analog_keys =
    configuration.get<std::vector<std::string>>("analogs.__keys");
  for (const auto& k : analog_keys) {
    std::string p = "analogs." + k;
    Analog* analog = new Analog();
    analog->setName(configuration.get<std::string>(p + ".name"));
    analog->setDescription(configuration.get<std::string>(p + ".description"));
    specification->analogs[analog->getName()] = analog;
  }

  auto sixdof_keys =
    configuration.get<std::vector<std::string>>("sixdofs.__keys");
  for (const auto& k : sixdof_keys) {
    std::string p = "sixdofs." + k;
    SixDOF* sixdof = new SixDOF();
    sixdof->setName(configuration.get<std::string>(p + ".name"));
    sixdof->setDescription(configuration.get<std::string>(p + ".description"));
    specification->sixdofs[sixdof->getName()] = sixdof;
  }
  return specification;
}

} // namespace input

} // namespace cavr
