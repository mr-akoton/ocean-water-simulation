#include <glad/glad.h>
#include <core/buffers/EBO.hpp>

/* ========================================================================== */
/*                          CONTRUCTOR AND DESTRUCTOR                         */
/* ========================================================================== */

EBO::EBO(void) { glCreateBuffers(1, &_id); }

EBO::EBO(const std::vector<GLuint>& indices) {
  glCreateBuffers(1, &_id);
  this->bindData(indices);
}

EBO::~EBO() { glDeleteBuffers(1, &_id); }

/* ========================================================================== */
/*                                   BINDER                                   */
/* ========================================================================== */

void EBO::bindData(const std::vector<GLuint>& indices) {
  glNamedBufferData(_id, indices.size() * sizeof(GLuint), indices.data(),
                    GL_STATIC_DRAW);
}

void EBO::bind(void) const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id); }

void EBO::unbind(void) const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

GLuint EBO::getID(void) const { return _id; }
