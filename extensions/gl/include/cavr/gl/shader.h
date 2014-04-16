#pragma once
#include <cavr/util/file.h>
#include <string>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glog/logging.h>

namespace cavr {

namespace gl {

template<GLenum ShaderType>
class Shader {
public:
  GLuint id() const;
  ~Shader();
  static Shader* fromFile(const std::string& path);
  static Shader* fromSource(const std::string& source);
private:
  Shader(GLuint id);
  GLuint shader_id_;
};

typedef Shader<GL_VERTEX_SHADER> VertexShader;
typedef Shader<GL_FRAGMENT_SHADER> FragmentShader;

class Program {
public:
  Program();
  template<GLenum ShaderType>
  void addShader(Shader<ShaderType>* shader) {
    glAttachShader(program_id_, shader->id());
  }
  void bindFragDataLocation(GLuint location, const std::string& name);
  bool link();
  void begin();
  GLint getUniform(const std::string& name);
  GLint getAttribute(const std::string& name);
  GLuint id() const;
  void end();
  ~Program();
  static Program* createSimple();
private:
  GLuint program_id_;
};

template<GLenum ShaderType>
Shader<ShaderType>::Shader(GLuint id)
  : shader_id_(id) {
}

template<GLenum ShaderType>
GLuint Shader<ShaderType>::id() const {
  return shader_id_;
}

template<GLenum ShaderType>
Shader<ShaderType>::~Shader() {
  glDeleteShader(shader_id_);
}

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
  const char* source_ptr = source.c_str();
  glShaderSource(id, 1, &source_ptr, NULL);
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
  return new Shader<ShaderType>(id);
}

} // namespace gl

} // namespace cavr
