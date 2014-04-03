x11_renderer = {
  type = "x11";
  display = ":0.0";
  windows = {
  };
};

machines = {
  self = {
    hostname = HOSTNAME;
    ssh = HOSTNAME;
    address = "tcp://" .. HOSTNAME .. ":8888";
  };
};
