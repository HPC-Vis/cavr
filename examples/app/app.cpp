#include <cavr/cavr.h>
#include <glog/logging.h>

struct ContextData {
  int foo;
};

void initContext() {
  LOG(INFO) << "initContext";
  ContextData* cd = new ContextData();
  cavr::System::setContextData(cd);
  cd->foo = 101;
}

void frame() {
  ContextData* cd = (ContextData*)cavr::System::getContextData();
  if (cd->foo != 101) {
    LOG(INFO) << "FML";
  }
}

void render() {
  LOG(INFO) << "render";
}

void destructContext() {
  LOG(INFO) << "destructContext";
  ContextData* cd = (ContextData*)cavr::System::getContextData();
  delete cd;
}

void update() {
  if (cavr::input::getButton("exit")->delta() == cavr::input::Button::Pressed) {
    cavr::System::shutdown();
    return;
  }
  static int i = 0;
  LOG(INFO) << i++;
  sleep(1);
  if (i >= 10) {
    cavr::System::shutdown();
    return;
  }
}

int main(int argc, char** argv) {
  LOG(INFO) << "Setting callbacks.";
  cavr::System::setCallback("update", update);
  cavr::System::setCallback("init_gl_context", initContext);
  cavr::System::setCallback("update_gl_context", frame);
  cavr::System::setCallback("gl_render", render);
  cavr::System::setCallback("destruct_gl_context", destructContext);
  cavr::input::InputMap input_map;
  input_map.button_map["exit"] = "keyboard[Escape]";
  if (!cavr::System::init(argc, argv, &input_map)) {
    LOG(ERROR) << "Failed to initialize cavr.";
    return -1;
  }
  LOG(INFO) << "Successfully initialized cavr.";
  LOG(INFO) << "Callbacks set.";
  LOG(INFO) << "Starting simulation.";
  cavr::System::run();
  LOG(INFO) << "Shutting down.";
  return 0;
}
