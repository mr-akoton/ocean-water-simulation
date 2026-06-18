#include <core/Object.hpp>

/* ========================================================================== */
/*                          CONTRUCTOR AND DESTRUCTOR                         */
/* ========================================================================== */

VBO::VBO(void) { glGenBuffers(1, &_id); }

VBO::VBO(const std::vector<Vertex>& vertices) {
  glGenBuffers(1, &_id);
  bindData(vertices);
}

VBO::VBO(const std::vector<glm::vec2>& vertices) {
  glGenBuffers(1, &_id);
  bindData(vertices);
}

VBO::VBO(const std::vector<glm::vec3>& vertices) {
  glGenBuffers(1, &_id);
  bindData(vertices);
}

VBO::VBO(const std::vector<glm::vec4>& vertices) {
  glGenBuffers(1, &_id);
  bindData(vertices);
}

VBO::~VBO() { glDeleteBuffers(1, &_id); }

/* ========================================================================== */
/*                                   BINDER                                   */
/* ========================================================================== */

void VBO::bindData(const std::vector<Vertex>& vertices) {
  this->bind();
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);
  this->unbind();
}

void VBO::bindData(const std::vector<glm::vec2>& vertices) {
  this->bind();
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2),
               vertices.data(), GL_STATIC_DRAW);
  this->unbind();
}

void VBO::bindData(const std::vector<glm::vec3>& vertices) {
  this->bind();
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
               vertices.data(), GL_STATIC_DRAW);
  this->unbind();
}

void VBO::bindData(const std::vector<glm::vec4>& vertices) {
  this->bind();
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec4),
               vertices.data(), GL_STATIC_DRAW);
  this->unbind();
}

void VBO::bind(void) const { glBindBuffer(GL_ARRAY_BUFFER, _id); }

void VBO::unbind(void) const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
