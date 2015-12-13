#include <cavr/cavr.h>
#include <cavr/gfx/renderer.h>
#include <cavr/gfx/shapes.h>
#include <cavr/gl/shader.h>
#include <cavr/gl/vao.h>
#include <cavr/gl/vbo.h>
#include <glog/logging.h>

struct ContextData {
  cavr::gl::Program* simple_program;
  cavr::gl::Program* cube_program;
  GLint color_uniform;
  GLint projection_uniform;
  GLint view_uniform;
  GLint model_uniform;
  GLint mvp_uniform;
  cavr::gl::VAO* sphere_vao;
  cavr::gl::VBO* sphere_vbo;
  size_t num_triangles_in_sphere;
  size_t num_triangles_in_cube;
  cavr::gl::VAO* cube_vao;
  cavr::gl::VBO* cube_vbo;
  float cube_angle;
};

void initContext() {
  ContextData* cd = new ContextData();
  cd->simple_program = cavr::gl::Program::createSimple();
  
  // Create a program
  cd->cube_program = new cavr::gl::Program();

  // Add shaders

  cavr::gl::VertexShader* vs = cavr::gl::VertexShader::fromFile("shader.vert");
  if (!vs) {
    LOG(ERROR) << "Failed to load simple vertex shader";
    cavr::System::shutdown();
    return;
  }

  cavr::gl::FragmentShader* fs = cavr::gl::FragmentShader::fromFile("shader.frag");
  if (!fs) {
    LOG(ERROR) << "Failed to load simple fragment shader";
    cavr::System::shutdown();
    return;
  }

  cd->cube_program->addShader(vs);
  cd->cube_program->addShader(fs);

  cd->cube_program->bindFragDataLocation(0,"color");
  cd->cube_angle = 0;
  if(!cd->cube_program->link())
  {
    LOG(ERROR) << "Failed to link simple shader";
    delete vs;
    delete fs;
    cavr::System::shutdown();
    return;
  }
  delete vs;
  delete fs;

  cd->mvp_uniform = cd->cube_program->getUniform("mvp");

  std::vector<cavr::math::vec4f> cubeTriangles = cavr::gfx::Shapes::wireCube();
  cd->num_triangles_in_cube = cubeTriangles.size();

  cd->cube_vbo = new cavr::gl::VBO(cubeTriangles);
  cd->cube_vao = new cavr::gl::VAO();
  cd->cube_vao->setAttribute(cd->cube_program->getAttribute("pos"),
    cd->cube_vbo,
    4,
    GL_FLOAT,
    0,
    0,
    0);

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
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  ContextData* cd = (ContextData*)cavr::System::getContextData();
  cd->simple_program->begin();
  cd->sphere_vao->bind();
  glUniformMatrix4fv(cd->projection_uniform, 1, GL_FALSE, cavr::gfx::getProjection().v);
  glUniformMatrix4fv(cd->view_uniform, 1, GL_FALSE, cavr::gfx::getView().v);
  auto model = mat4f::translate(0, 1, 0) * mat4f::scale(0.1);
  glUniformMatrix4fv(cd->model_uniform, 1, GL_FALSE, model.v);
  if (cavr::input::getButton("color")->delta() == cavr::input::Button::Held) {
    glUniform3f(cd->color_uniform, 0, 0, 1);
  } else {
    glUniform3f(cd->color_uniform, 1, 0, 0);
  }
  glDrawArrays(GL_TRIANGLES, 0, cd->num_triangles_in_sphere);
  glBindVertexArray(0);
  cd->simple_program->end();  

  
  // Cube Program
  cd->cube_program->begin();
  cd->cube_vao->bind();
  //cout << "HERE YALL" << endl;
  //LOG(ERROR) << cavr::input::InputManager::dt() << endl;
  cd->cube_angle += 3.14/4.0 * cavr::input::InputManager::dt()*1000;
  LOG(INFO) << cd->cube_angle;
  glUniformMatrix4fv(cd->mvp_uniform, 1, GL_FALSE, (cavr::gfx::getProjection() * cavr::gfx::getView() * model * cavr::math::mat4f::rotate(cd->cube_angle,cavr::math::vec3f(0,1,0))).v );
  //LOG(ERROR) << cd->num_triangles_in_cube;
  
  glDrawArrays(GL_TRIANGLES,0,cd->num_triangles_in_cube);
  glBindVertexArray(0);
  cd->cube_program->end();
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
  //input_map.button_map["color"] = "vrpn[Button0[0]]";
  //input_map.sixdof_map["wand"] = "emulated_wand";
  //input_map.sixdof_map["wand"] = "vrpn[Tracker0[0]]";
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
