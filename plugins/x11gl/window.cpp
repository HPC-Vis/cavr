#include "window.h"
#include "x11gl.h"

namespace x11 {

static bool context_error = false;

static int contextErrorHandler(Display* display, XErrorEvent* event) {
  context_error = true;
  return 0;
}

bool isExtensionSupported(const char *ext_list, const char *extension)
{
  const char *start;
  const char *where, *terminator;

  /* Extension names should not have spaces. */
  where = strchr(extension, ' ');
  if (where || *extension == '\0')
    return false;

  /* It takes a bit of care to be fool-proof about parsing the
     OpenGL extensions string. Don't be fooled by sub-strings,
     etc. */
  for (start=ext_list;;) {
    where = strstr(start, extension);

    if (!where)
      break;

    terminator = where + strlen(extension);

    if ( where == start || *(where - 1) == ' ' )
      if ( *terminator == ' ' || *terminator == '\0' )
        return true;

    start = terminator;
  }

  return false;
}

Window::Window()
  : glx_window_(0),
    colormap_(0) {
}

bool Window::open(::Display* display) {
  display_ = display;
  std::vector<int> visual_attribs( {
    GLX_X_RENDERABLE, True,
    GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
    GLX_RENDER_TYPE, GLX_RGBA_BIT,
    GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
    GLX_RED_SIZE, 8,
    GLX_GREEN_SIZE, 8,
    GLX_BLUE_SIZE, 8,
    GLX_ALPHA_SIZE, 8,
    GLX_DEPTH_SIZE, 24,
    GLX_STENCIL_SIZE, 8,
    GLX_DOUBLEBUFFER, True 
  });
  visual_attribs.push_back(None);

  int num_configs = 0;
  GLXFBConfig* framebuffer_configs =
    glXChooseFBConfig(display,
                      DefaultScreen(display),
                      visual_attribs.data(),
                      &num_configs);
  if (!framebuffer_configs) {
    LOG(ERROR) << "Failed to get a matching framebuffer config.";
    return false;
  }

  int best_config_index = -1;
  int best_samples_per_pixel = -1;
  for (int i = 0; i < num_configs; ++i) {
    XVisualInfo* visual_info =
      glXGetVisualFromFBConfig(display, framebuffer_configs[i]);
    if (visual_info) {
      int has_sample_buffers;
      glXGetFBConfigAttrib(display,
                           framebuffer_configs[i],
                           GLX_SAMPLE_BUFFERS,
                           &has_sample_buffers);
      int samples_per_pixel;
      glXGetFBConfigAttrib(display,
                           framebuffer_configs[i],
                           GLX_SAMPLES,
                           &samples_per_pixel);
      if (best_config_index < 0 ||
          has_sample_buffers && best_samples_per_pixel < samples_per_pixel) {
        best_config_index = i;
      }
      XFree(visual_info);
    }
  }

  GLXFBConfig framebuffer_config = framebuffer_configs[best_config_index];
  XFree(framebuffer_configs);
  XVisualInfo* visual_info =
    glXGetVisualFromFBConfig(display, framebuffer_config);
  colormap_ = XCreateColormap(display,
                              RootWindow(display, visual_info->screen),
                              visual_info->visual,
                              AllocNone);
  XSetWindowAttributes window_attributes;
  window_attributes.colormap = colormap_;
  window_attributes.background_pixmap = None;
  window_attributes.border_pixel = 0;
  window_attributes.event_mask =
    StructureNotifyMask | KeyPressMask | KeyReleaseMask;
  glx_window_ = XCreateWindow(display,
                              RootWindow(display, visual_info->screen),
                              x_position_,
                              y_position_,
                              width_,
                              height_,
                              0,
                              visual_info->depth,
                              InputOutput,
                              visual_info->visual,
                              CWColormap | CWEventMask | CWBorderPixel,
                              &window_attributes);
  XFree(visual_info);
  if (!glx_window_) {
    LOG(ERROR) << "Failed to create window.";
    return false;
  }

  std::unique_lock<std::mutex> x_lock(X11GL::mutex());
  XMapWindow(display, glx_window_);
  XMoveWindow(display, glx_window_, x_position_, y_position_);
  XResizeWindow(display, glx_window_, width_, height_);

  const char* extensions = 
    glXQueryExtensionsString(display, DefaultScreen(display));
  typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*,
                                                       GLXFBConfig,
                                                       GLXContext,
                                                       Bool,
                                                       const int*);
  glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
  glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
    glXGetProcAddressARB((const GLubyte*)("glXCreateContextAttribsARB"));
  context_error = false;
  int (*old_handler)(Display*, XErrorEvent*) =
    XSetErrorHandler(&contextErrorHandler);
  if (!isExtensionSupported(extensions, "GLX_ARB_create_context") ||
      !glXCreateContextAttribsARB) {
    context_ = glXCreateNewContext(display,
                                   framebuffer_config,
                                   GLX_RGBA_TYPE,
                                   0,
                                   True);
  } else {
    int context_attribs[] = {
      GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
      GLX_CONTEXT_MINOR_VERSION_ARB, 0,
      None
    };
    context_ = glXCreateContextAttribsARB(display,
                                          framebuffer_config,
                                          0,
                                          True,
                                          context_attribs);
    if (context_error || !context_) {
      int attribs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 1,
        GLX_CONTEXT_MINOR_VERSION_ARB, 0,
        None
      };
      context_ = glXCreateContextAttribsARB(display,
                                            framebuffer_config,
                                            0,
                                            True,
                                            attribs);
    }
  }
  XSync(display, False);
  bool context_error_occurred = context_error;
  XSetErrorHandler(old_handler);
  if (context_error_occurred || !context_) {
    LOG(ERROR) << "Failed to create OpenGL context";
    return false;
  }
  if (!glXIsDirect(display, context_)) {
    LOG(WARNING) << "Indirect GLX rendering used";
  }
  x_lock.unlock();
  if (!glXMakeContextCurrent(display_, glx_window_, glx_window_, context_)) {
    LOG(ERROR) << "Could not make current";
  }
  glDrawBuffer(GL_BACK);
  glViewport(0, 0, width_, height_);
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  glXSwapBuffers(display, glx_window_);
  glXWaitGL();
  glXMakeContextCurrent(display_, None, None, None);
  return true;
}

void Window::update() {
  if (!glXMakeContextCurrent(display_, glx_window_, glx_window_, context_)) {
    LOG(ERROR) << "Could not make current";
  }
  glViewport(0, 0, width_, height_);
  glDrawBuffer(GL_BACK);
  glClearColor(1, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  glXSwapBuffers(display_, glx_window_);
  glXWaitGL();
}

Window::~Window() {
  glXMakeCurrent(display_, 0, 0);
  glXDestroyContext(display_, context_);
  XDestroyWindow(display_, glx_window_);
  XFreeColormap(display_, colormap_);
}

Window* Window::configure(cavr::config::Configuration& config) {
  std::string window_type = config.get<std::string>("view");
  Window* window = nullptr;
  config.pushPrefix("view.");
  if ("perspective_render" == window_type) {
    PerspectiveWindow* p = PerspectiveWindow::configure(config);
    window = p;
  } else if ("simulator_view" == window_type) {
    SimulatorWindow* s = SimulatorWindow::configure(config);
    window = s;
  }
  config.popPrefix("view.");
  window->near_ = config.get<double>("near");
  window->far_ = config.get<double>("far");
  window->width_ = config.get<double>("width");
  window->height_ = config.get<double>("height");
  window->x_position_ = config.get<double>("x");
  window->y_position_ = config.get<double>("y");
  window->fullscreen_ = config.get<bool>("fullscreen");
  window->input_name_ = config.get<std::string>("input_name");
  return window;
}

PerspectiveWindow*
PerspectiveWindow::configure(cavr::config::Configuration& config) {
  PerspectiveWindow* p = new PerspectiveWindow();
  p->lower_left_ = config.get<cavr::input::Marker*>("lower_left");
  p->lower_right_ = config.get<cavr::input::Marker*>("lower_right");
  p->upper_left_ = config.get<cavr::input::Marker*>("upper_left");
  std::string eye_type = config.get<std::string>("eyes");
  if ("stereo" == eye_type) {
    p->eyes_.push_back(config.get<cavr::input::Marker*>("left_eye"));
    p->eyes_.push_back(config.get<cavr::input::Marker*>("right_eye"));
  } else if ("mono" == eye_type) {
    p->eyes_.push_back(config.get<cavr::input::Marker*>("eye"));
  }
  return p;
}

SimulatorWindow*
SimulatorWindow::configure(cavr::config::Configuration& config) {
  SimulatorWindow* s = new SimulatorWindow();
  return s;
}

} // namespace x11
