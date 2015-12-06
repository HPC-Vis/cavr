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
      --left_eye = cavr.sixdof("emulated3");
      --right_eye = cavr.sixdof("emulated2");
      --stereo ="mono";
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

vrpn = {
  type = "vrpn";
  input_name = "vrpn";
  buttons = {
    --"Button0@localhost";
  };
  analogs = {
  };
  sixdofs = {
    "tracker0@tracker.rd.unr.edu"
  };
};

self = {
  hostname = "hpcvis5";
  ssh = "hpcvis5";--"chase@" .. HOSTNAME;
  address = "hpcvis5";
  plugins = {
    x11_renderer = x11_renderer;
    vrpn = vrpn;
  };
};


others = {
  hostname = "hpcvis7";
  ssh = "hpcvis7";
  address = "hpcvis7";--"tcp://" .. "hpcvis7" .. ":8888";
  plugins = {
    x11_renderer = x11_renderer;
    vrpn = vrpn;
  };
};

others2 = {
  hostname = "hpcvis2";
  ssh = "hpcvis2";
  address = "hpcvis2";--"tcp://" .. "hpcvis7" .. ":8888";
  plugins = {
    x11_renderer = x11_renderer;
    vrpn = vrpn;
  };
};

machines = {
  self=self;
  self2 = others;
  self3 = others2;
};
