#include <core/Object.hpp>

/* ========================================================================== */
/*                          CONTRUCTOR AND DESTRUCTOR                         */
/* ========================================================================== */

VAO::VAO(void) { glGenVertexArrays(1, &_id); }

VAO::~VAO() { glDeleteVertexArrays(1, &_id); }

/* ========================================================================== */
/*                                   BINDER                                   */
/* ========================================================================== */

void VAO::bind(void) const { glBindVertexArray(_id); }

void VAO::unbind(void) const { glBindVertexArray(0); }

/* ========================================================================== */
/*                                   LINKER                                   */
/* ========================================================================== */

void VAO::linkAttribute(const VBO& vbo, GLuint index, GLint size, GLenum type,
                        GLsizei stride, const void* pointer) const {
  vbo.bind();
  glVertexAttribPointer(index, size, type, GL_FALSE, stride, pointer);
  glEnableVertexAttribArray(index);
  vbo.unbind();
}
