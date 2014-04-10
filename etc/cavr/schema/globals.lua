import("machine.lua")

globals = {
  machines = {
    type = "list";
    required = true;
    subtype = machine;
  }; -- machines
  sync_port = {
    type = "number";
    required = false;
    description = "port used to connect to the master";
    default = 8952;
  };
  data_port = {
    type = "number";
    required = false;
    description = "port used to receive data from the master";
    default = 8953;
  };
};
