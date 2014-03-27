analog = {
  name = { required = true; type = "string"; };
  description = { required = false; type = "string"; default = ""; };
  min = { required = false; type = "number"; default = -1.0; };
  max = { required = false; type = "number"; default = 1.0; };
  default = { required = false; type = "number"; default = 0.0; };
}
