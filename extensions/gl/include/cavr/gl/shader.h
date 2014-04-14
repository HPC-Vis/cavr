#pragma once
#include <cavr/util/file.h>
#include <string>
#include <GL/gl.h>
#include <glog/logging.h>

namespace cavr {

namespace gl {

template<GLenum ShaderType>
class Shader {
public:
  Shader() = delete;
  static Shader* fromFile(const std::string& path);
  static Shader* fromSource(const std::string& source);
private:
  GLuint shader_id_;
};

typedef Shader<GL_VERTEX_SHADER> VertexShader;
typedef Shader<GL_FRAGMENT_SHADER> FragmentShader;

template<GLenum ShaderType>
Shader<ShaderType>* Shader<ShaderType>::fromFile(const std::string& path) {
  std::string source;
  if (!util::File::loadIntoString(path, source)) {
    LOG(ERROR) << "Could not load shader " << path;
    return nullptr;
  }
  return fromSource(source);
}

template<GLenum ShaderType>
Shader<ShaderType>* Shader<ShaderType>::fromSource(const std::string& source) {
  GLuint id = glCreateShader(ShaderType);
  glShaderSource(id, 1, &(source.c_str()), NULL);
  glCompileShader(id);
  GLint compile_status = GL_FALSE;
  int compile_log_length = 0;
  glGetShaderiv(id, GL_COMPILE_STATUS, &compile_status);
  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &compile_log_length);
  std::vector<char> log(compile_log_length);
  glGetShaderInfoLog(id, compile_log_length, NULL, log.data());
  LOG(INFO) << log.data();
  if (GL_FALSE == compile_status) {
    LOG(ERROR) << "Failed to load shader";
    glDeleteShader(id);
    return nullptr;
  }
  Shader<ShaderType>* s = new Shader();
  s->shader_id_ = id;
  return s;
}

} // namespace gl

} // namespace cavr
