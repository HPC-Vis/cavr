#include <GL/glew.h>
#include "x11gl.h"
#include <cavr/system.h>
using namespace cavr;

namespace x11 {

static bool context_error = false;

static int contextErrorHandler(Display* display, XErrorEvent* event) {
  context_error = true;
  return 0;
}

std::mutex X11GL::x_mutex_;

std::mutex& X11GL::mutex() {
  return x_mutex_;
}

bool getFramebufferConfig(::Display* display,
                          bool stereo, 
                          GLXFBConfig& config) {
  std::vector<int> visual_attribs({
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
  if (stereo) {
    visual_attribs.push_back(GLX_STEREO);
    visual_attribs.push_back(True);
  }
  visual_attribs.push_back(None);
  

  int num_configs = 0;
  GLXFBConfig* framebuffer_configs =
    glXChooseFBConfig(display,
                      DefaultScreen(display),
                      visual_attribs.data(),
                      &num_configs);
  if (!framebuffer_configs || num_configs == 0) {
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
  if (-1 == best_config_index) {
    LOG(ERROR) << "None of the configs have visual info";
    XFree(framebuffer_configs);
    return false;
  }
  config = framebuffer_configs[best_config_index];
  XFree(framebuffer_configs);
  return true;
}

GLXContext createContext(::Display* display, GLXFBConfig config) {
  GLXContext context;
  std::vector<std::string> extensions;
  cavr::util::String::split(glXQueryExtensionsString(display,
                                                     DefaultScreen(display)),
                            " ",
                            extensions);
  auto isExtensionSupported = [&extensions](const std::string& extension) {
    auto result = std::find(extensions.begin(), extensions.end(), extension);
    return extensions.end() != result;
  };
  typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*,
                                                       GLXFBConfig,
                                                       GLXContext,
                                                       Bool,
                                                       const int*);
  glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
  glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
    glXGetProcAddressARB((const GLubyte*)("glXCreateContextAttribsARB"));
  std::unique_lock<std::mutex> x_lock(X11GL::mutex());
  context_error = false;
  int (*old_handler)(Display*, XErrorEvent*) =
    XSetErrorHandler(&contextErrorHandler);
  if (!isExtensionSupported("GLX_ARB_create_context") ||
      !glXCreateContextAttribsARB) {
    LOG(INFO) << "glXCreateContextAttribsARB unavailable";
    LOG(INFO) << "Falling back to glXCreateNewContext";
    context = glXCreateNewContext(display, config, GLX_RGBA_TYPE, 0, True);
  } else {
    int attribs30[] = {
      GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
      GLX_CONTEXT_MINOR_VERSION_ARB, 0,
      None
    };
    context = glXCreateContextAttribsARB(display, config, 0, True, attribs30);
    if (context_error || !context) {
      int attribs10[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 1,
        GLX_CONTEXT_MINOR_VERSION_ARB, 0,
        None
      };
      context = glXCreateContextAttribsARB(display, config, 0, True, attribs10);
    }
  }
  XSync(display, False);
  bool context_error_occurred = context_error;
  XSetErrorHandler(old_handler);
  if (context_error_occurred || !context) {
    LOG(ERROR) << "Failed to create OpenGL context";
    return false;
  }
  if (!glXIsDirect(display, context)) {
    LOG(WARNING) << "Indirect GLX rendering used";
  }
  x_lock.unlock();
  return context;
}

X11GL::X11GL()
  : display_(nullptr),
    mono_context_(0),
    stereo_context_(0) {
}

bool X11GL::init(cavr::config::Configuration& config) {
  display_name_ = config.get<std::string>("display");
  input_name_ = config.get<std::string>("input_name");
  update_context_callback_ = 
    cavr::System::getCallback(config.get<std::string>("update_callback"));
  destruct_context_callback_ =
    cavr::System::getCallback(config.get<std::string>("destruct_callback"));
  auto window_keys = config.get<std::vector<std::string>>("windows.__keys");
  config.pushPrefix("windows.");
  for (auto window_name : window_keys) {
    config.pushPrefix(window_name + ".");
    Window* window = Window::configure(config);
    config.popPrefix(window_name + ".");
    if (window) {
      if (window->isStereo()) {
        stereo_windows_.push_back(window);
      } else {
        mono_windows_.push_back(window);
      }
    } else {
      LOG(ERROR) << "Failed to configure window";
    }
  }
  config.popPrefix("windows.");
  display_ = XOpenDisplay(display_name_.empty()? NULL : display_name_.c_str());
  if (!display_) {
    LOG(ERROR) << "Failed to open display " << display_name_;
    return false;
  }

  GLXFBConfig stereo_config;
  if (!stereo_windows_.empty()) {
    if (!getFramebufferConfig(display_, true, stereo_config)) {
      LOG(ERROR) << "Failed to get stereo config";
      return false;
    }
    stereo_context_ = createContext(display_, stereo_config);
    if (!stereo_context_) {
      LOG(ERROR) << "Failed to create stereo context";
      return false;
    }
  }

  GLXFBConfig mono_config;
  if (!mono_windows_.empty()) {
    if (!getFramebufferConfig(display_, false, mono_config)) {
      LOG(ERROR) << "Failed to get mono config";
      return false;
    }
    mono_context_ = createContext(display_, mono_config);
    if (!mono_context_) {
      LOG(ERROR) << "Failed to create mono context";
      return false;
    }
  }

  bool result = true;
  for (auto window : mono_windows_) {
    result &= window->open(display_, mono_config, mono_context_);
    window_map_[window->getWindow()] = window;
  }
  for (auto window : stereo_windows_) {
    result &= window->open(display_, stereo_config, stereo_context_);
    window_map_[window->getWindow()] = window;
  }

  if (!result) {
    LOG(ERROR) << "Failed to open all requested windows";
    return false;
  }

  if (!mono_windows_.empty()) {
    mono_windows_[0]->makeCurrent();
    glewInit();
    cavr::System::setContextData(nullptr);
    cavr::System::getCallback(config.get<std::string>("init_callback"))();
    mono_context_data_ = cavr::System::getContextData();
    glXMakeContextCurrent(display_, None, None, None);
  }
  if (!stereo_windows_.empty()) {
    stereo_windows_[0]->makeCurrent();
    cavr::System::setContextData(nullptr);
    cavr::System::getCallback(config.get<std::string>("init_callback"))();
    stereo_context_data_ = cavr::System::getContextData();
    glXMakeContextCurrent(display_, None, None, None);
  }

  for (auto it : window_map_) {
    it.second->makeCurrent();
    if (!it.second->setupRenderData()) {
      LOG(ERROR) << "Failed to setup render data";
      result = false;
    }
  }
  glXMakeContextCurrent(display_, None, None, None);

  std::vector<cavr::input::Analog*> analogs(6);
  using cavr::input::getAnalog;
  analogs[0] = getAnalog.byDeviceNameOrNull(input_name_ + "[analog[x0]]");
  analogs[1] = getAnalog.byDeviceNameOrNull(input_name_ + "[analog[y0]]");
  analogs[2] = getAnalog.byDeviceNameOrNull(input_name_ + "[analog[x1]]");
  analogs[3] = getAnalog.byDeviceNameOrNull(input_name_ + "[analog[y1]]");
  analogs[4] = getAnalog.byDeviceNameOrNull(input_name_ + "[analog[x2]]");
  analogs[5] = getAnalog.byDeviceNameOrNull(input_name_ + "[analog[y2]]");
  for (auto it : window_map_) {
    it.second->setAnalogs(analogs.data());
  }

  return result;
}

bool X11GL::step() {
  if (!mono_windows_.empty()) {
    mono_windows_[0]->makeCurrent();
    cavr::System::setContextData(mono_context_data_);
    update_context_callback_();
    mono_context_data_ = cavr::System::getContextData();
    glXMakeContextCurrent(display_, None, None, None);
  }
  if (!stereo_windows_.empty()) {
    stereo_windows_[0]->makeCurrent();
    cavr::System::setContextData(nullptr);
    update_context_callback_();
    stereo_context_data_ = cavr::System::getContextData();
    glXMakeContextCurrent(display_, None, None, None);
  }
  for (auto it : window_map_) {
    it.second->update();
  }
  processEvents();
  return true;
}

void X11GL::processEvents() {
  while (!window_map_.empty() && XPending(display_)) {
    XEvent event;
    XNextEvent(display_, &event);
    switch (event.type) {
      case ConfigureNotify:
      {
        auto configure_event = reinterpret_cast<XConfigureEvent*>(&event);
        Window* window = window_map_[configure_event->window];
        if (!window) {
          break;
        }
        window->setPosition(configure_event->x, configure_event->y);
        window->setResolution(configure_event->width, configure_event->height);
      }
      break;
      case KeyPress:
      case KeyRelease: 
      {
        auto key_event = reinterpret_cast<XKeyEvent*>(&event);
        KeySym key_sym = XLookupKeysym(key_event, 0);
        KeySym uppercase_key_sym;
        KeySym lowercase_key_sym;
        XConvertCase(key_sym, &lowercase_key_sym, &uppercase_key_sym);
        auto result = button_by_key_.find(lowercase_key_sym);
        if (button_by_key_.end() == result) {
          std::string button_name =
            input_name_ + "[" + XKeysymToString(lowercase_key_sym) + "]";
          auto button = cavr::input::getButton.byDeviceNameOrNull(button_name);
          button_by_key_[lowercase_key_sym] = button;
        }
        auto button = button_by_key_[lowercase_key_sym];
        if (button) {
          button->setState(event.type == KeyPress);
        }
        ::Window x_window = key_event->window;
        window_map_[x_window]->pressKey(lowercase_key_sym);
      }
      break;
      case MotionNotify:
      {
        auto motion_event = reinterpret_cast<XMotionEvent*>(&event);
        Window* window = window_map_[motion_event->window];
        if (!window) {
          break;
        }
        int x = motion_event->x;
        int y = motion_event->y;
        window->mouseMove(motion_event->state & ShiftMask, x, y);
      }
      case ButtonPress:
      {
        auto press_event = reinterpret_cast<XButtonPressedEvent*>(&event);
        Window* window = window_map_[press_event->window];
        if (!window) {
          break;
        }
        int x = press_event->x;
        int y = press_event->y;
        switch (press_event->button) {
          case 1:
            window->leftClick(true, press_event->state & ShiftMask, x, y);
            break;
          case 2:
            window->middleClick(true, press_event->state & ShiftMask, x, y);
            break;
          case 3:
            window->rightClick(true, press_event->state & ShiftMask, x, y);
            break;
          case 4:
            window->scrollUp();
            break;
          case 5:
            window->scrollDown();
            break;
          default:
            break;
        }
      }
      break;
      case ButtonRelease:
      {
        auto release_event = reinterpret_cast<XButtonReleasedEvent*>(&event);
        Window* window = window_map_[release_event->window];
        if (!window) {
          break;
        }
        int x = release_event->x;
        int y = release_event->y;
        switch (release_event->button) {
          case 1:
            window->leftClick(false, release_event->state & ShiftMask, x, y);
            break;
          case 2:
            window->middleClick(false, release_event->state & ShiftMask, x, y);
            break;
          case 3:
            window->rightClick(false, release_event->state & ShiftMask, x, y);
            break;
          default:
            break;
        }
      }
      break;
      default:
      break;
    }
  }
}

X11GL::~X11GL() {
  if (!mono_windows_.empty()) {
    mono_windows_[0]->makeCurrent();
    cavr::System::setContextData(mono_context_data_);
    destruct_context_callback_();
    for (auto window : mono_windows_) {
      delete window;
    }
    glXMakeContextCurrent(display_, None, None, None);
    glXDestroyContext(display_, mono_context_);
  }
  if (!stereo_windows_.empty()) {
    stereo_windows_[0]->makeCurrent();
    cavr::System::setContextData(stereo_context_data_);
    destruct_context_callback_();
    for (auto window : stereo_windows_) {
      delete window;
    }
    glXMakeContextCurrent(display_, None, None, None);
    glXDestroyContext(display_, stereo_context_);
  }
  XCloseDisplay(display_);
}

} // namespace x11

extern "C" {

PluginGeneratorBase* load() {
  config::ConfigurationSpecification* spec =
    config::ConfigurationSpecification::createFromSchema("plugins/x11.lua",
                                                         "x11");
  if (nullptr == spec) {
    LOG(ERROR) << "Could not load x11 spec";
    return nullptr;
  }
  PluginGeneratorBase* result = new PluginGenerator<x11::X11GL>("x11gl", *spec);
  return result;
}

} // extern "C"
