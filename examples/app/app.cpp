#include <cavr/cavr.h>
#include <glog/logging.h>

void update() {
  static int i = 0;
  LOG(INFO) << i++;
  if (i >= 10) {
    cavr::System::shutdown();
    return;
  }
}

int main(int argc, char** argv) {
  if (!cavr::System::init(argc, argv)) {
    LOG(ERROR) << "Failed to initialize cavr.";
    return -1;
  }
  LOG(INFO) << "Successfully initialized cavr.";
  LOG(INFO) << "Setting callbacks.";
  cavr::System::setCallback("update", update);
  LOG(INFO) << "Callbacks set.";
  LOG(INFO) << "Starting simulation.";
  cavr::System::run();
  LOG(INFO) << "Shutting down.";
  return 0;
}
