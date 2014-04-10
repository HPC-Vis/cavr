sim_window = {
  view = {
    simulator_view = true;
  };
};

x11_renderer = {
  type = "x11gl";
  display = ":0.0";
  windows = {
    sim_window = sim_window;
  };
};

self = {
  hostname = HOSTNAME;
  ssh = HOSTNAME;
  address = "tcp://" .. HOSTNAME .. ":8888";
  plugins = {
    x11_renderer = x11_renderer;
  };
};

machines = {
  self = self;
};
