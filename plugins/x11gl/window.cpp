#include <cavr/system.h>
#include <cavr/gfx/shapes.h>
#include <cavr/gfx/sphere.h>
#include "window.h"
#include "x11gl.h"

namespace x11 {

const float sixdof_radius = 0.25;

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
  window_attributes.override_redirect = True;
  window_attributes.event_mask =
    StructureNotifyMask | 
    KeyPressMask | 
    KeyReleaseMask | 
    ButtonPressMask |
    ButtonReleaseMask |
    PointerMotionMask;
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
  if (fullscreen_) {
    XEvent fullscreen_event;
    Atom wm_state = XInternAtom(display_, "_NET_WM_STATE", False);
    Atom fullscreen_state = 
      XInternAtom(display_, "_NET_WM_STATE_FULLSCREEN", False);
    memset(&fullscreen_event, 0, sizeof(fullscreen_event));
    fullscreen_event.type = ClientMessage;
    fullscreen_event.xclient.window = glx_window_;
    fullscreen_event.xclient.message_type = wm_state;
    fullscreen_event.xclient.format = 32;
    fullscreen_event.xclient.data.l[0] = 1;
    fullscreen_event.xclient.data.l[1] = fullscreen_state;
    fullscreen_event.xclient.data.l[2] = 0;
    XSendEvent(display_,
               DefaultRootWindow(display_), 
               False, 
               SubstructureNotifyMask, 
               &fullscreen_event);
    XFlush(display_);
  }
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
  glEnable(GL_DEPTH_TEST);
  if (isStereo()) {
    glDrawBuffer(GL_BACK_LEFT);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    eye_index_ = 0;
    setupView(0);
    cavr::gfx::Renderer::renderer = this;
    render_callback_();
    postRender();

    glDrawBuffer(GL_BACK_RIGHT);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    eye_index_ = 1;
    setupView(1);
    cavr::gfx::Renderer::renderer = this;
    render_callback_();
    postRender();
  } else {
    glDrawBuffer(GL_BACK);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    eye_index_ = 0;
    setupView(0);
    cavr::gfx::Renderer::renderer = this;
    render_callback_();
    postRender();
  }
  glXSwapBuffers(display_, glx_window_);
  glXWaitGL();
}

void Window::setPosition(int x, int y) {
  x_position_ = x;
  y_position_ = y;
}

void Window::setResolution(int width, int height) {
  width_ = width;
  height_ = height;
}

void Window::postRender() {
}

void Window::setAnalogs(cavr::input::Analog** analogs) {
}

Window::~Window() {
  glXMakeCurrent(display_, 0, 0);
  XDestroyWindow(display_, glx_window_);
  XFreeColormap(display_, colormap_);
}

void Window::setNear(double d) {
  near_ = d;
  setupView(eye_index_);
}

void Window::setFar(double d) {
  far_ = d;
  setupView(eye_index_);
}

double Window::getNear() const {
  return near_;
}

double Window::getFar() const {
  return far_;
}

cavr::math::mat4f Window::getProjection() const {
  return projection_;
}

cavr::math::mat4f Window::getView() const {
  return view_;
}

cavr::math::vec3f Window::getEyePosition() const {
  return eye_position_;
}

Window* Window::configure(cavr::config::Configuration& config) {
  std::string window_type = config.get<std::string>("view");
  Window* window = nullptr;
  config.pushPrefix("view.");
  if ("perspective_render" == window_type) {
    PerspectiveWindow* p = PerspectiveWindow::configure(config);
    window = p;
  } else if ("simulator_view" == window_type) { SimulatorWindow* s = SimulatorWindow::configure(config);
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
  window->render_callback_ =
    cavr::System::getCallback(config.get<std::string>("render_callback"));
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

void Window::mouseMove(bool shifted, int x, int y) {
}

void Window::scrollUp() {
}

void Window::scrollDown() {
}

void Window::pressKey(KeySym key) {
}

bool Window::setupRenderData() {
  return true;
}

PerspectiveWindow*
PerspectiveWindow::configure(cavr::config::Configuration& config) {
  PerspectiveWindow* p = new PerspectiveWindow();
  p->lower_left_ = config.get<cavr::input::Marker*>("lower_left");
  p->lower_right_ = config.get<cavr::input::Marker*>("lower_right");
  p->upper_left_ = config.get<cavr::input::Marker*>("upper_left");
  std::string eye_type = config.get<std::string>("eyes");
  if ("stereo" == eye_type) {
    p->eyes_.push_back(config.get<cavr::input::Marker*>("eyes.left_eye"));
    p->eyes_.push_back(config.get<cavr::input::Marker*>("eyes.right_eye"));
  } else if ("mono" == eye_type) {
    p->eyes_.push_back(config.get<cavr::input::Marker*>("eyes.eye"));
  }
  return p;
}

void PerspectiveWindow::setupView(int eye) {
  using cavr::math::vec3f;
  using cavr::math::vec4f;
  using cavr::math::mat4f;
  vec3f ll = lower_left_->getPosition();
  vec3f lr = lower_right_->getPosition();
  vec3f ul = upper_left_->getPosition();
  vec3f right = (lr - ll).normalized();
  vec3f up = (ul - ll).normalized();
  vec3f back = right.cross(up).normalized();
  right = up.cross(back).normalized();
  up = back.cross(right).normalized();
  mat4f screen_reference(1.0);
  screen_reference[0].xyz = right;
  screen_reference[1].xyz = up;
  screen_reference[2].xyz = back;
  screen_reference[3].xyz = ll;
  mat4f inverse_screen_reference = screen_reference.inverted();
  eye_position_ = eyes_[eye]->getPosition();
  vec4f screen_eye = inverse_screen_reference * vec4f(eye_position_, 1.0);
  float l = -screen_eye.x / screen_eye.z * near_;
  float r = ((lr - ll).length() - screen_eye.x) / screen_eye.z * near_;
  float b = -screen_eye.y / screen_eye.z * near_;
  float t = ((ul - ll).length() - screen_eye.y) / screen_eye.z * near_;
  projection_ = mat4f::frustum(l, r, b, t, near_, far_);
  view_ = mat4f::translate(-screen_eye.xyz) * inverse_screen_reference;
}

SimulatorWindow::SimulatorWindow()
  : cube_vao_(nullptr),
    cube_vbo_(nullptr),
    cone_vao_(nullptr),
    cone_vbo_(nullptr),
    rotating_(false),
    panning_(false),
    tilting_(false),
    highlighted_sixdof_(nullptr),
    selected_sixdof_(nullptr) {
  setting_analog_[0] = false;
  setting_analog_[1] = false;
  setting_analog_[2] = false;
}

SimulatorWindow*
SimulatorWindow::configure(cavr::config::Configuration& config) {
  SimulatorWindow* s = new SimulatorWindow();
  s->pivot_distance_ = 5.0;
  s->eye_position_ = cavr::math::vec3d(1.0).normalized() * s->pivot_distance_;
  s->view_ = cavr::math::mat4d::look_at(s->eye_position_,
                                        cavr::math::vec3d(0.0),
                                        cavr::math::vec3d(0.0, 1.0, 0.0));
  return s;
}

void SimulatorWindow::setupView(int eye) {
  projection_ = cavr::math::mat4d::perspective(M_PI * 0.5,
                                               double(width_) / double(height_),
                                               near_,
                                               far_);
  eye_position_ = eye_;
}

bool SimulatorWindow::setupRenderData() {
  simple_program_ = cavr::gl::Program::createSimple();
  if (!simple_program_) {
    LOG(ERROR) << "Failed to load rendering shader";
    return false;
  }
  GLint position_location = simple_program_->getAttribute("in_position");
  if (-1 == position_location) {
    LOG(ERROR) << "in_position does not exist";
    return false;
  }
  color_uniform_ = simple_program_->getUniform("color");
  projection_uniform_ = simple_program_->getUniform("projection");
  view_uniform_ = simple_program_->getUniform("view");
  model_uniform_ = simple_program_->getUniform("model");
  if (-1 == color_uniform_ ||
      -1 == projection_uniform_ ||
      -1 == view_uniform_) {
    LOG(ERROR) << "Failed to get all uniforms";
    return false;
  }
  auto cube_lines = cavr::gfx::Shapes::wireCube();
  cube_vbo_ = new cavr::gl::VBO(cube_lines, GL_STATIC_DRAW);
  cube_vao_ = new cavr::gl::VAO();
  cube_vao_->setAttribute(position_location, cube_vbo_, 4, GL_FLOAT, 0, 0, 0);
  auto cone_verts =
    cavr::gfx::Shapes::solidCone(30, -sixdof_radius * 1.5, sixdof_radius);
  num_cone_vertices_ = cone_verts.size();
  cone_vbo_ = new cavr::gl::VBO(cone_verts, GL_STATIC_DRAW);
  cone_vao_ = new cavr::gl::VAO();
  cone_vao_->setAttribute(position_location, cone_vbo_, 4, GL_FLOAT, 0, 0, 0);
  return true;
}

void SimulatorWindow::postRender() {
  using cavr::math::mat4f;
  simple_program_->begin();
  cube_vao_->bind();
  glUniformMatrix4fv(projection_uniform_, 1, GL_FALSE, projection_.v);
  glUniformMatrix4fv(view_uniform_, 1, GL_FALSE, view_.v);
  glUniformMatrix4fv(model_uniform_, 1, GL_FALSE, mat4f::translate(0, 1, 0).v);
  glUniform3f(color_uniform_, 1, 1, 1);
  glDrawArrays(GL_LINES, 0, 24);

  cone_vao_->bind();
  std::vector<std::string> device_sixdof_names = 
    cavr::input::getSixDOF.deviceNames();
  for (auto n : device_sixdof_names) {
    cavr::input::SixDOF* sixdof = cavr::input::getSixDOF.byDeviceName(n);
    mat4f m = sixdof->getMatrix();
    if (highlighted_sixdof_ == sixdof) {
      glUniform3f(color_uniform_, 0.0, 1.0, 0.0);
    } else {
      glUniform3f(color_uniform_, 0.4, 0.4, 0.4);
    }
    glUniformMatrix4fv(model_uniform_, 1, GL_FALSE, m.v);
    glDrawArrays(GL_TRIANGLES, 0, num_cone_vertices_);
  }
  glBindVertexArray(0);
  simple_program_->end();
}

void SimulatorWindow::scrollUp() {
  using cavr::math::mat4f;
  float camera_distance = view_[3].xyz.length();
  float new_camera_distance = std::max(camera_distance / 1.1, 0.5);
  float delta = new_camera_distance - camera_distance;
  mat4f translation = mat4f::translate(0, 0, -delta);
  view_ = translation * view_;
}

void SimulatorWindow::scrollDown() {
  using cavr::math::mat4f;
  float camera_distance = view_[3].xyz.length();
  float new_camera_distance = std::min(camera_distance * 1.1, 10.0);
  float delta = new_camera_distance - camera_distance;
  mat4f translation = mat4f::translate(0, 0, -delta);
  view_ = translation * view_;
}

void SimulatorWindow::leftClick(bool clicked, bool shifted, int x, int y) {
  if (!clicked) {
    selected_sixdof_ = nullptr;
  } else {
    selected_sixdof_ = highlighted_sixdof_;
  }
  tilting_ = clicked && !shifted;
  rotating_ &= !clicked;
  panning_ &= !clicked;
  setting_analog_[0] = clicked && shifted;
}

void SimulatorWindow::middleClick(bool clicked, bool shifted, int x, int y) {
  if (!clicked) {
    selected_sixdof_ = nullptr;
  } else {
    selected_sixdof_ = highlighted_sixdof_;
  }
  panning_ = clicked && !shifted;
  rotating_ &= !clicked;
  tilting_ &= !clicked;
  setting_analog_[1] = clicked && shifted;
}

void SimulatorWindow::rightClick(bool clicked, bool shifted, int x, int y) {
  if (!clicked) {
    selected_sixdof_ = nullptr;
  } else {
    selected_sixdof_ = highlighted_sixdof_;
  }
  rotating_ = clicked && !shifted;
  panning_ &= !clicked;
  tilting_ &= !clicked;
  setting_analog_[2] = clicked && shifted;
}

void SimulatorWindow::mouseMove(bool shifted, int x, int y) {
  using cavr::math::mat4f;
  using cavr::math::vec4f;
  using cavr::math::vec3f;
  using cavr::math::vec2f;

  vec2f old_window_coords(2.0 * mouse_x_ / width_ - 1.0,
                          2.0 * (height_ - mouse_y_) / height_ - 1.0);
  vec2f window_coords(2.0 * x / width_ - 1.0,
                      2.0 * (height_ - y) / height_ - 1.0);
  mat4f projection_view = projection_ * view_;
  mat4f projection_view_inverse = projection_view.inverted();
  vec4f projected_point(window_coords, -1, 1);
  vec4f unprojected_point = projection_view_inverse * projected_point;
  unprojected_point /= unprojected_point.w;
  mat4f view_inverse = view_.inverted();
  vec3f eye_position = view_inverse[3].xyz;
  vec3f pick_direction = unprojected_point.xyz - eye_position;
  pick_direction = pick_direction.normalized();

  int dx = x - mouse_x_;
  int dy = y - mouse_y_;
  if (nullptr == selected_sixdof_) {
    if (rotating_) {
      mat4f forward_translation = mat4f::translate(-view_[3].xyz);
      mat4f backwards_translation = mat4f::translate(view_[3].xyz);
      mat4f x_rotation = mat4f::rotate((float)dx / 180.0f, 0, 1, 0);
      mat4f y_rotation = mat4f::rotate((float)dy / 180.0f, 1, 0, 0);
      view_ = backwards_translation * 
        y_rotation * x_rotation * forward_translation * view_;
    } else if (tilting_) {
      float old_angle = atan2(old_window_coords.y, old_window_coords.x);
      float new_angle = atan2(window_coords.y, window_coords.x);
      float delta = new_angle - old_angle;
      mat4f rotation = mat4f::rotate(delta, 0, 0, 1);
      view_ = rotation * view_;
    }
  } else { // selected_sixdof_
    if (rotating_) {
      mat4f view_reference = view_;
      view_reference[3].xyz = vec3f(0.0);
      mat4f inverse_view_reference = view_reference.inverted();
      mat4f x_rotation = mat4f::rotate((float)dx / 180.0f, 0, 1, 0);
      mat4f y_rotation = mat4f::rotate((float)dy / 180.0f, 1, 0, 0);
      mat4f m = selected_sixdof_->getMatrix();
      vec3f p = m[3].xyz;
      m[3].xyz = vec3f(0);
      m = inverse_view_reference * y_rotation * x_rotation * view_reference * m;
      m[3].xyz = p;
      selected_sixdof_->setState(m);
    } else if (panning_) {
      vec3f p = selected_sixdof_->getPosition();
      vec4f projected_position = projection_ * view_ * vec4f(p, 1.0);
      projected_position /= projected_position.w;
      vec4f new_position =
        projection_view_inverse * 
        vec4f(window_coords, projected_position.z, 1);
      new_position /= new_position.w;
      mat4f m = selected_sixdof_->getMatrix();
      m[3] = new_position;
      selected_sixdof_->setState(m);
    } else if (tilting_) {
      mat4f view_reference = view_;
      view_reference[3].xyz = vec3f(0.0);
      mat4f inverse_view_reference = view_reference.inverted();
      vec3f p = selected_sixdof_->getPosition();
      vec4f projected_position = projection_ * view_ * vec4f(p, 1.0);
      projected_position /= projected_position.w;
      vec2f old_v = old_window_coords - projected_position.xy;
      vec2f new_v = window_coords - projected_position.xy;
      float old_angle = atan2(old_v.y, old_v.x);
      float new_angle = atan2(new_v.y, new_v.x);
      float delta = new_angle - old_angle;
      mat4f rotation = mat4f::rotate(delta, 0, 0, 1);
      mat4f m = selected_sixdof_->getMatrix();
      m[3].xyz = vec3f(0);
      m = inverse_view_reference * rotation * view_reference * m;
      m[3].xyz = p;
      selected_sixdof_->setState(m);
    }
  }
  mouse_x_ = x;
  mouse_y_ = y;
  cavr::gfx::Ray pick_ray(eye_position, pick_direction);
  std::vector<std::string> device_sixdof_names = 
    cavr::input::getSixDOF.deviceNames();
  cavr::input::SixDOF* closest_sixdof = nullptr;
  float closest_distance = -1.0f;
  for (auto n : device_sixdof_names) {
    cavr::input::SixDOF* sixdof = cavr::input::getSixDOF.byDeviceName(n);
    vec3f collision_center =
      sixdof->getPosition() + sixdof_radius * sixdof->getForward();
    cavr::gfx::Sphere collision_sphere(collision_center, sixdof_radius);
    float collision_distance = 0.0f;
    if (collision_sphere.intersect(pick_ray, collision_distance)) {
      if (nullptr == closest_sixdof || collision_distance < closest_distance) {
        closest_distance = collision_distance;
        closest_sixdof = sixdof;
      }
    }
  }
  highlighted_sixdof_ = closest_sixdof;
  if (highlighted_sixdof_) {
    XStoreName(display_, glx_window_, highlighted_sixdof_->getName().c_str());
  } else {
    XStoreName(display_, glx_window_, "");
  }

  for (int i = 0; i < 3; ++i) {
    if (setting_analog_[i]) {
      if (analogs_[2 * i]) {
        analogs_[2 * i]->setValue(window_coords.x);
      }
      if (analogs_[2 * i + 1]) {
        analogs_[2 * i + 1]->setValue(window_coords.y);
      }
    }
  }
}

void SimulatorWindow::setAnalogs(cavr::input::Analog** analogs) {
  for (int i = 0; i < 6; ++i) {
    analogs_[i] = analogs[i];
  }
}

} // namespace x11
