#include <cavr/cavr.h>
#include <cavr/gfx/renderer.h>
#include <cavr/gfx/shapes.h>
#include <cavr/gl/shader.h>
#include <cavr/gl/vao.h>
#include <cavr/gl/vbo.h>
#include <glog/logging.h>

struct ContextData {
  cavr::gl::Program* simple_program;
  GLint color_uniform;
  GLint projection_uniform;
  GLint view_uniform;
  GLint model_uniform;
  cavr::gl::VAO* sphere_vao;
  cavr::gl::VBO* sphere_vbo;
  size_t num_triangles_in_sphere;
};

void initContext() {
  ContextData* cd = new ContextData();
  cd->simple_program = cavr::gl::Program::createSimple();
  cd->color_uniform = cd->simple_program->getUniform("color");
  cd->projection_uniform = cd->simple_program->getUniform("projection");
  cd->view_uniform = cd->simple_program->getUniform("view");
  cd->model_uniform = cd->simple_program->getUniform("model");
  std::vector<cavr::math::vec4f> sphere_vertices = 
    cavr::gfx::Shapes::solidSphere(30, 30);
  cd->num_triangles_in_sphere = sphere_vertices.size();
  cd->sphere_vbo = new cavr::gl::VBO(sphere_vertices);
  cd->sphere_vao = new cavr::gl::VAO();
  cd->sphere_vao->setAttribute(cd->simple_program->getAttribute("in_position"),
                               cd->sphere_vbo,
                               4,
                               GL_FLOAT,
                               0,
                               0,
                               0);
  cavr::System::setContextData(cd);
}

void frame() {
  ContextData* cd = (ContextData*)cavr::System::getContextData();
}

void render() {
  using cavr::math::mat4f;
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  ContextData* cd = (ContextData*)cavr::System::getContextData();
  cd->simple_program->begin();
  cd->sphere_vao->bind();
  glUniformMatrix4fv(cd->projection_uniform, 1, GL_FALSE, cavr::gfx::getProjection().v);
  glUniformMatrix4fv(cd->view_uniform, 1, GL_FALSE, cavr::gfx::getView().v);
  auto model = mat4f::translate(0, 1, 0) * mat4f::scale(0.1);
  glUniformMatrix4fv(cd->model_uniform, 1, GL_FALSE, model.v);
  glUniform3f(cd->color_uniform, 1, 0, 0);
  glDrawArrays(GL_TRIANGLES, 0, cd->num_triangles_in_sphere);
  glBindVertexArray(0);
  cd->simple_program->end();
}

void destructContext() {
  ContextData* cd = (ContextData*)cavr::System::getContextData();
  delete cd;
}

void update() {
  if (cavr::input::getButton("exit")->delta() == cavr::input::Button::Pressed) {
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
  input_map.sixdof_map["wand"] = "emulated_wand";
  input_map.analog_map["rotation"] = "keyboard[analog[y0]]";
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
