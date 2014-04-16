sim_window = {
  view = {
    simulator_view = true;
  };
};

sim_window = {
  view = {
    simulator_view = true;
  };
};

perspective_window = {
  view = {
    eyes = {
      eye = cavr.sixdof("emulated");
    };
    lower_left = cavr.sixdof("emulated") * cavr.translate(-1, -1, -1);
    lower_right = cavr.sixdof("emulated") * cavr.translate(1, -1, -1);
    upper_left = cavr.sixdof("emulated") * cavr.translate(-1, 1, -1);
  };
};

x11_renderer = {
  type = "x11gl";
  display = ":0.0";
  windows = {
    sim_window = sim_window;
    sim_window2 = sim_window;
    perspective_window = perspective_window;
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
