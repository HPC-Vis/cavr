machine = {
  hostname = {
    type = "string";
    required = true;
    description = "hostname as specified by the environment variable HOSTNAME";
  }; -- hostname
  ssh = {
    type = "string";
    required = true;
    description = "ssh address to machine; assumes shared keys";
  }; -- ssh
  address = {
    type = "string";
    required = true;
    description = "zeromq-style address of the machine for communications";
  }; -- address
}; -- machine
