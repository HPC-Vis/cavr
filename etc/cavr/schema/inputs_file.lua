import("input/switch.lua");
import("input/analog.lua");
import("input/sixdof.lua");

inputs_file = {
  switches = {
    type = "list";
    required = true;
    description = "all switches that will be used by the program";
    subtype = switch;
  }; -- switches
  analogs = {
    type = "list";
    required = true;
    description = "all analogs that will be used by the program";
    subtype = analog;
  }; -- analogs
  sixdofs = {
    type = "list";
    required = true;
    description = "all sixdofs that will be used by the program";
    subtype = sixdof;
  }; -- sixdofs
};
