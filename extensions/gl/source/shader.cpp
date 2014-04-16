#include <cavr/gl/shader.h>

namespace cavr {

namespace gl {

Program::Program() {
  program_id_ = glCreateProgram();
}

void Program::begin() {
  glUseProgram(program_id_);
}

void Program::end() {
  glUseProgram(0);
}

GLint Program::getUniform(const std::string& name) {
  GLint location = glGetUniformLocation(program_id_, name.c_str());
  if (-1 == location) {
    LOG(WARNING) << "Uniform " << name << " does not exist";
  }
  return location;
}

GLint Program::getAttribute(const std::string& name) {
  GLint location = glGetAttribLocation(program_id_, name.c_str());
  if (-1 == location) {
    LOG(WARNING) << "Attribute " << name << " does not exist";
  }
  return location;
}

void Program::bindFragDataLocation(GLuint location, const std::string& name) {
  glBindFragDataLocation(program_id_, location, name.c_str());
}

bool Program::link() {
  glLinkProgram(program_id_);
  GLint link_status = GL_FALSE;
  int link_log_length = 0;
  glGetProgramiv(program_id_, GL_LINK_STATUS, &link_status);
  glGetProgramiv(program_id_, GL_INFO_LOG_LENGTH, &link_log_length);
  std::vector<char> log(link_log_length);
  glGetProgramInfoLog(program_id_, link_log_length, NULL, log.data());
  LOG(INFO) << log.data();
  if (GL_FALSE == link_status) {
    LOG(ERROR) << "Failed to link program";
  }
  return GL_FALSE != link_status;
}

GLuint Program::id() const {
  return program_id_;
}

Program::~Program() {
  glDeleteProgram(program_id_);
}

const char* simple_vs =
"#version 130\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"in vec4 in_position;\n"
"\n"
"void main() {\n"
"  gl_Position = projection * view * model * in_position;\n"
"}\n"
;

const char* simple_fs =
"#version 130\n"
"out vec4 frag_color;\n"
"uniform vec3 color;\n"
"\n"
"void main() {\n"
"  frag_color.xyz = color;\n"
"  frag_color.w = 1.0;\n"
"}\n"
;

Program* Program::createSimple() {
  VertexShader* vs = VertexShader::fromSource(simple_vs);
  if (!vs) {
    LOG(ERROR) << "Failed to load simple vertex shader";
    return nullptr;
  }
  FragmentShader* fs = FragmentShader::fromSource(simple_fs);
  if (!fs) {
    LOG(ERROR) << "Failed to load simple fragment shader";
    return nullptr;
  }
  Program* p = new Program();
  p->addShader(vs);
  p->addShader(fs);
  p->bindFragDataLocation(0, "frag_color");
  if (!p->link()) {
    LOG(ERROR) << "Failed to link simple shader";
    delete p;
    delete vs;
    delete fs;
    return nullptr;
  }
  delete vs;
  delete fs;
  return p;
}

} // namespace gl

} // namespace cavr

