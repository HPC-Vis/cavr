#pragma once
#include <cavr/plugin.h>
#include <cavr/gfx/renderer.h>
#include <cavr/gl/shader.h>
#include <cavr/gl/vao.h>
#include <cavr/gl/vbo.h>
#include <cavr/math/vector.h>
#include <cavr/input/marker.h>
#include <cavr/input/analog.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/keysym.h>
#include <X11/X.h>
#include <X11/Xlib.h>

namespace x11 {

class Window : public cavr::gfx::Renderer {
public:
  Window();
  bool open(::Display* display,
            GLXFBConfig framebuffer_config,
            GLXContext context);
  void update();
  void setPosition(int x, int y);
  void setResolution(int width, int height);
  bool makeCurrent();
  ::Window getWindow();
  virtual bool isStereo() const;
  virtual void leftClick(bool clicked, bool shifted, int x, int y);
  virtual void middleClick(bool clicked, bool shifted, int x, int y);
  virtual void rightClick(bool clicked, bool shifted, int x, int y);
  virtual void mouseMove(bool shifted, int x, int y);
  virtual void scrollUp();
  virtual void scrollDown();
  virtual void pressKey(KeySym key);
  virtual void setupView(int eye) = 0;
  virtual bool setupRenderData();
  virtual void postRender();
  virtual void setAnalogs(cavr::input::Analog** analogs);
  virtual ~Window();
  virtual void setNear(double d);
  virtual void setFar(double d);
  virtual double getNear() const;
  virtual double getFar() const;
  virtual cavr::math::mat4f getProjection() const;
  virtual cavr::math::mat4f getView() const;
  virtual cavr::math::vec3f getEyePosition() const;
  static Window* configure(cavr::config::Configuration& config);
protected:
  double near_;
  double far_;
  cavr::math::mat4f projection_;
  cavr::math::mat4f view_;
  cavr::math::vec3f eye_position_;
  int eye_index_;
  unsigned int width_;
  unsigned int height_;
  unsigned int x_position_;
  unsigned int y_position_;
  bool fullscreen_;
  std::function<void()> render_callback_;

  ::Colormap colormap_;
  ::GLXContext context_;
  ::Window glx_window_;
  ::Display* display_;
  void* context_data_;
};

class PerspectiveWindow : public Window {
public:
  virtual bool isStereo() const;
  virtual void setupView(int eye);
  static PerspectiveWindow* configure(cavr::config::Configuration& config);
private:
  cavr::input::Marker* lower_left_;
  cavr::input::Marker* lower_right_;
  cavr::input::Marker* upper_left_;
  std::vector<cavr::input::Marker*> eyes_;
};

class SimulatorWindow : public Window {
public:
  SimulatorWindow();
  virtual void setupView(int eye);
  virtual bool setupRenderData();
  virtual void postRender();
  virtual void leftClick(bool clicked, bool shifted, int x, int y);
  virtual void middleClick(bool clicked, bool shifted, int x, int y);
  virtual void rightClick(bool clicked, bool shifted, int x, int y);
  virtual void scrollUp();
  virtual void scrollDown();
  virtual void mouseMove(bool shifted, int x, int y);
  virtual void setAnalogs(cavr::input::Analog** analogs);
  static SimulatorWindow* configure(cavr::config::Configuration& config);
private:
  double pivot_distance_;
  cavr::math::vec3d eye_;
  cavr::gl::Program* simple_program_;
  GLint color_uniform_;
  GLint projection_uniform_;
  GLint view_uniform_;
  GLint model_uniform_;
  cavr::gl::VBO* cube_vbo_;
  cavr::gl::VAO* cube_vao_;
  cavr::gl::VBO* cone_vbo_;
  cavr::gl::VAO* cone_vao_;
  size_t num_cone_vertices_;
  bool rotating_;
  bool panning_;
  bool tilting_;
  int mouse_x_;
  int mouse_y_;
  cavr::input::SixDOF* highlighted_sixdof_;
  cavr::input::SixDOF* selected_sixdof_;
  bool setting_analog_[3];
  cavr::input::Analog* analogs_[6];
};

} // namespace x11
