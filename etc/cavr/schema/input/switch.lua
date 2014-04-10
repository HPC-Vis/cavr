switch = {
  name = { 
    required = true;
    type = "string";
    description = "user- and developer-visible name";
  }; -- name
  description = {
    required = false;
    type = "string";
    description = "user-visible description of switch function";
    default = "";
  };  -- description
  states = {
    required = true;
    type = "list";
    subtype = {
      name = {
        required = false;
        type = "string";
        description = "state name";
        default = "";
      }; -- name
      description = {
        required = false;
        type = "string";
        description = "state description";
        default = "";
      }; -- description
    }; -- subtype
  }; -- states
}; -- switch
