vrpn = {
  input_name = {
    type = "string";
    description = "name of inputs generated";
    required = true;
  }; -- input_name
  buttons = {
    type = "string_list";
    description = "list of button addresses to get of form <name>@<address>";
    required = true;
  }; -- buttons
  analogs = {
    type = "string_list";
    description = "list of analog addresses to get of form <name>@<address>";
    required = true;
  }; -- analogs
  sixdofs = {
    type = "string_list";
    description = "list of sixdof addresses to get of form <name>@<address>";
    required = true;
  }; -- sixdofs
  pretransform = {
    type = "transform";
    default = cavr.identity();
    description = "world-space transformation";
    required = false;
  };
  posttransform = {
    type = "transform";
    default = cavr.identity();
    description = "local-space transformation";
    required = false;
  };
}; -- vrpn
