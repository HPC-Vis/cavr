root_dir = os.getenv("CAVR_PLUGIN_PATH");
if (root_dir == nil) then root_dir = "." end;
plugins = {
  root_dir .. "/libx11gl.so";
  root_dir .. "/libcavr_vrpn.so";
};
