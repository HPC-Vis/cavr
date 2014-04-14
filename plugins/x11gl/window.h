#pragma once
#include <cavr/plugin.h>
#include <cavr/math/vector.h>
#include <cavr/input/marker.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <X11/keysym.h>
#include <X11/X.h>
#include <X11/Xlib.h>

namespace x11 {

class Window {
public:
  Window();
  bool open(::Display* display,
            GLXFBConfig framebuffer_config,
            GLXContext context);
  void update();
  bool makeCurrent();
  ::Window getWindow();
  virtual bool isStereo() const;
  virtual void leftClick(bool clicked, bool shifted, int x, int y);
  virtual void middleClick(bool clicked, bool shifted, int x, int y);
  virtual void rightClick(bool clicked, bool shifted, int x, int y);
  virtual void scrollUp();
  virtual void scrollDown();
  virtual void pressKey(KeySym key);
  virtual ~Window();
  static Window* configure(cavr::config::Configuration& config);
protected:
  double near_;
  double far_;
  unsigned int width_;
  unsigned int height_;
  unsigned int x_position_;
  unsigned int y_position_;
  bool fullscreen_;

  ::Colormap colormap_;
  ::GLXContext context_;
  ::Window glx_window_;
  ::Display* display_;
  void* context_data_;
};

class PerspectiveWindow : public Window {
public:
  virtual bool isStereo() const;
  static PerspectiveWindow* configure(cavr::config::Configuration& config);
private:
  cavr::input::Marker* lower_left_;
  cavr::input::Marker* lower_right_;
  cavr::input::Marker* upper_left_;
  std::vector<cavr::input::Marker*> eyes_;
};

class SimulatorWindow : public Window {
public:
  static SimulatorWindow* configure(cavr::config::Configuration& config);
private:
  cavr::math::vec3d eye_;
};

} // namespace x11
