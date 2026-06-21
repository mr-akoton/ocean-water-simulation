#include <glad/glad.h>
#include <core/buffers/VBO.hpp>

/* ========================================================================== */
/*                          CONTRUCTOR AND DESTRUCTOR                         */
/* ========================================================================== */

VBO::VBO(void) { glCreateBuffers(1, &_id); }

VBO::VBO(const std::vector<Vertex>& vertices) {
  glCreateBuffers(1, &_id);
  bindData(vertices);
}

VBO::VBO(const std::vector<glm::vec2>& vertices) {
  glCreateBuffers(1, &_id);
  bindData(vertices);
}

VBO::VBO(const std::vector<glm::vec3>& vertices) {
  glCreateBuffers(1, &_id);
  bindData(vertices);
}

VBO::VBO(const std::vector<glm::vec4>& vertices) {
  glCreateBuffers(1, &_id);
  bindData(vertices);
}

VBO::~VBO() { glDeleteBuffers(1, &_id); }

/* ========================================================================== */
/*                                   BINDER                                   */
/* ========================================================================== */

void VBO::bindData(const std::vector<Vertex>& vertices) {
  glNamedBufferData(_id, vertices.size() * sizeof(Vertex), vertices.data(),
                    GL_STATIC_DRAW);
}

void VBO::bindData(const std::vector<glm::vec2>& vertices) {
  glNamedBufferData(_id, vertices.size() * sizeof(glm::vec2), vertices.data(),
                    GL_STATIC_DRAW);
}

void VBO::bindData(const std::vector<glm::vec3>& vertices) {
  glNamedBufferData(_id, vertices.size() * sizeof(glm::vec3), vertices.data(),
                    GL_STATIC_DRAW);
}

void VBO::bindData(const std::vector<glm::vec4>& vertices) {
  glNamedBufferData(_id, vertices.size() * sizeof(glm::vec4), vertices.data(),
                    GL_STATIC_DRAW);
}

void VBO::bind(void) const { glBindBuffer(GL_ARRAY_BUFFER, _id); }

void VBO::unbind(void) const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

GLuint VBO::getID(void) const { return _id; }
