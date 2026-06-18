#include <core/Object.hpp>

/* ========================================================================== */
/*                          CONTRUCTOR AND DESTRUCTOR                         */
/* ========================================================================== */

EBO::EBO(void) { glGenBuffers(1, &_id); }

EBO::EBO(const std::vector<GLuint>& indices) {
  glGenBuffers(1, &_id);
  this->bindData(indices);
}

EBO::~EBO() { glDeleteBuffers(1, &_id); }

/* ========================================================================== */
/*                                   BINDER                                   */
/* ========================================================================== */

void EBO::bindData(const std::vector<GLuint>& indices) {
  this->bind();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
               indices.data(), GL_STATIC_DRAW);
  this->unbind();
}

void EBO::bind(void) const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id); }

void EBO::unbind(void) const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
