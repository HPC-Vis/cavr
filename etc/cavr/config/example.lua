sim_window = {
  view = {
    simulator_view = true;
  };
--fullscreen = true;
};

sim_window = {
  view = {
    simulator_view = true;
  };
--fullscreen = true;
};

perspective_window = {
  view = {
    eyes = {
      eye = cavr.sixdof("vrpn[Tracker0[0]]");
      --left_eye = cavr.sixdof("emulated3");
      --right_eye = cavr.sixdof("emulated2");
      --stereo ="mono";
    };
    lower_left = cavr.sixdof("vrpn[Tracker0[0]]") * cavr.translate(-1, -1, -1);
    lower_right = cavr.sixdof("vrpn[Tracker0[0]]") * cavr.translate(1, -1, -1);
    upper_left = cavr.sixdof("vrpn[Tracker0[0]]") * cavr.translate(-1, 1, -1);
  };
  fullscreen = true;
};

x11_renderer = {
  type = "x11gl";
  display = ":0.0";
  windows = {
    sim_window = sim_window;
    --sim_window2 = sim_window;
    --perspective_window = perspective_window;
  };
};

x11_renderer2 = {
  type = "x11gl";
  display = ":0.2";
  windows = {
    --sim_window = sim_window;
    --sim_window2 = sim_window;
    perspective_window = perspective_window;

  };
};

x11_renderer3 = {
  type = "x11gl";
  display = ":0.1";
  windows = {
    --sim_window = sim_window;
    --sim_window2 = sim_window;
    perspective_window = perspective_window;
  };
};


vrpn = {
  type = "vrpn";
  input_name = "vrpn";
  buttons = {
    "WiiMote0@localhost";
  };
  analogs = {
  };
  sixdofs = {
    "Tracker0@tracker.rd.unr.edu";
  };
};

self = {
  hostname = HOSTNAME;
  ssh = HOSTNAME;--"chase@" .. HOSTNAME;
  address = HOSTNAME;
  plugins = {
  x11_renderer = x11_renderer;
    x11_renderer2 = x11_renderer2;
    x11_renderer3 = x11_renderer3;
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
  --self2 = others;
  --self3 = others2;
};
