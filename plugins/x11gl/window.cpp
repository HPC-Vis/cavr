#include "window.h"
#include "x11gl.h"

namespace x11 {

Window::Window()
  : glx_window_(0),
    colormap_(0) {
}

bool Window::isStereo() const {
  return false;
}

bool Window::open(::Display* display,
                  GLXFBConfig framebuffer_config,
                  GLXContext context) {
  display_ = display;
  context_ = context;
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
  x_lock.unlock();

  if (!glXMakeContextCurrent(display_, glx_window_, glx_window_, context)) {
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

bool Window::makeCurrent() {
  return glXMakeContextCurrent(display_, glx_window_, glx_window_, context_);
}

void Window::update() {
  if (!glXMakeContextCurrent(display_, glx_window_, glx_window_, context_)) {
    LOG(ERROR) << "Could not make current";
  }
  glViewport(0, 0, width_, height_);
  glDrawBuffer(GL_BACK);
  glXSwapBuffers(display_, glx_window_);
  glXWaitGL();
}

Window::~Window() {
  glXMakeCurrent(display_, 0, 0);
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
  return window;
}

::Window Window::getWindow() {
  return glx_window_;
}

bool PerspectiveWindow::isStereo() const {
  return eyes_.size() == 2;
}

void Window::leftClick(bool clicked, bool shifted, int x, int y) {
}

void Window::middleClick(bool clicked, bool shifted, int x, int y) {
}

void Window::rightClick(bool clicked, bool shifted, int x, int y) {
}

void Window::scrollUp() {
}

void Window::scrollDown() {
}

void Window::pressKey(KeySym key) {
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
