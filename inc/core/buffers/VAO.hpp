#ifndef VAO_HPP
#define VAO_HPP

#include <glad/glad.h>

class VBO;
class EBO;

class VAO {
private:
  GLuint _id;

public:
  VAO(void);
  ~VAO();

  void bind(void) const;
  void unbind(void) const;
  void linkAttribute(GLuint index, GLint size, GLenum type,
                     GLuint offest) const;
  void linkVBO(VBO& vbo, GLuint index, GLuint offset, GLsizei stride);
  void linkEBO(EBO& ebo);
};

#endif
