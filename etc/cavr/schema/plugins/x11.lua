stereo_eyes = {
  left_eye = {
    required = true;
    type = "marker";
    description = "marker for the left eye";
  }; -- left_eye
  right_eye = {
    required = true;
    type = "marker";
    description = "marker for the right eye";
  }; -- right eye
  stereo = {
    required = true;
    type = "string";
    description = "type of stereoscopy to use";
  }; -- stereo
}; --stereo_eyes

mono_eyes = {
  eye = {
    required = true;
    type = "marker";
    description = "marker to render from";
  }; -- eye
}; -- mono_eyes

perspective_render = {
  lower_left = { 
    required = true; 
    type = "marker"; 
    description = "marker of the lower left corner of the window";
  }; -- lower_left
  lower_right = { 
    required = true; 
    type = "marker"; 
    description = "marker of the lower right corner of the window";
  }; -- lower_right
  upper_left = { 
    required = true; 
    type = "marker"; 
    description = "marker for the upper left corner of the window";
  }; -- upper_left
  eyes = {
    required = true;
    type = "one_of";
    possibilities = {
      stereo = stereo_eyes;
      mono = mono_eyes;
    }; -- possibilities
  }; -- eyes
}; -- perspective_render

simulator_view = {
  simulator_view = {
    required = true;
    type = "boolean";
    description = "existence forces rendering in simulation view";
  };
}; -- simulator_view

x11_window = {
  near = {
    required = false;
    type = "number";
    default = 0.01;
    description = "near clipping distance";
  }; -- near
  far = {
    required = false;
    type = "number";
    default = 10000.0;
    description = "far clipping distance";
  }; -- far
  width = { 
    required = false;
    type = "number";
    default = 600;
    description = "initial width of the window";
  }; -- width
  height = {
    required = false; 
    type = "number";
    default = 600;
    description = "initial height of the window";
  }; -- height 
  x = {
    required = false; 
    type = "number"; 
    default = 200; 
    description = "initial horizontal position of the window";
  }; -- x
  y = { 
    required = false; 
    type = "number"; 
    default = 200; 
    description = "initial vertical position of the window";
  }; -- y
  fullscreen = {
    required = false;
    type = "boolean";
    default = false;
    description = "true if fullscreen; false otherwise";
  }; -- fullscreen
  view = {
    required = true;
    type = "one_of";
    possibilities = {
      perspective_render = perspective_render;
      simulator_view = simulator_view;
    }; -- possibilities
  }; -- view
  render_callback = {
    required = false;
    type = "string";
    default = "gl_render";
    description = "callback used to render GL graphics";
  }; -- render_callback
}; -- x11_window

x11 = {
  display = {
    required = false;
    type = "string";
    default = ":0.0";
    description = "X11 display to create windows on";
  }; -- display
  windows = {
    required = true; 
    type = "list"; 
    subtype = x11_window; 
  }; -- windows
  init_callback = {
    required = false;
    type = "string";
    default = "init_gl_context";
    description = "callback used to initialize GL data";
  }; -- init_callback
  update_callback = {
    required = false;
    type = "string";
    default = "update_gl_context";
    description = "callback used to update GL data (not render)";
  }; -- update_callback
  destruct_callback = {
    required = false;
    type = "string";
    default = "destruct_gl_context";
    description = "callback used to destroy GL data";
  }; -- destruct_callback
  input_name = {
    required = false;
    type = "string";
    description = "base name for all inputs collected from these windows";
    default = "keyboard";
  }; -- input_name
}; -- x11
