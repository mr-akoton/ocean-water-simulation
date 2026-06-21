#include <glad/glad.h>
#include <core/buffers/VAO.hpp>
#include <core/buffers/VBO.hpp>
#include <core/buffers/EBO.hpp>

/* ========================================================================== */
/*                          CONTRUCTOR AND DESTRUCTOR                         */
/* ========================================================================== */

VAO::VAO(void) { glCreateVertexArrays(1, &_id); }

VAO::~VAO() { glDeleteVertexArrays(1, &_id); }

/* ========================================================================== */
/*                                   BINDER                                   */
/* ========================================================================== */

void VAO::bind(void) const { glBindVertexArray(_id); }

void VAO::unbind(void) const { glBindVertexArray(0); }

/* ========================================================================== */
/*                                   LINKER                                   */
/* ========================================================================== */

void VAO::linkAttribute(GLuint index, GLint size, GLenum type,
                        GLuint offset) const {
  glEnableVertexArrayAttrib(_id, index);
  glVertexArrayAttribBinding(_id, index, 0);
  glVertexArrayAttribFormat(_id, index, size, type, GL_FALSE, offset);
}

void VAO::linkVBO(VBO& vbo, GLuint index, GLuint offset, GLsizei stride) {
  glVertexArrayVertexBuffer(_id, index, vbo.getID(), offset, stride);
}

void VAO::linkEBO(EBO& ebo) { glVertexArrayElementBuffer(_id, ebo.getID()); }
