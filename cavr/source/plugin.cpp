#include <cavr/plugin.h>
#include <dlfcn.h>

namespace cavr {

PluginGeneratorBase::PluginGeneratorBase()
  : handle_(nullptr) {
}

void PluginGeneratorBase::setHandle(void* handle) {
  handle_ = handle;
}

PluginGeneratorBase::~PluginGeneratorBase() {
  if (handle_) {
    dlclose(handle_);
  }
}

PluginGeneratorBase* loadPlugin(const std::string& path) {
  void* handle = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
  if (nullptr == handle) {
    LOG(ERROR) << "Failed to open plugin " << path;
    LOG(ERROR) << "Reason: " << dlerror();
    return nullptr;
  }
  PluginGeneratorBase* (*function)();
  *(void**)(&function) = dlsym(handle, "load");
  if (nullptr == function) {
    dlclose(handle);
    LOG(ERROR) << "Plugin does not contain load function: " << path;
    return nullptr;
  }
  PluginGeneratorBase* result = (*function)();
  if (nullptr == result) {
    LOG(ERROR) << "Load did not produce a PluginGeneratorBase: " << path;
    return nullptr;
  }
  LOG(INFO) << "Loaded plugin: " << path;
  result->setHandle(handle);
  return result;
}

} // namespace cavr
