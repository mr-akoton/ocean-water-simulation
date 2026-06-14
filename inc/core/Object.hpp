#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>

struct Vertex {
  glm::vec3 position;
  glm::vec3 color;
};

/* ========================================================================== */
/*                             VERTEX ARRAY OBJECT                            */
/* ========================================================================== */

class VBO;

class VAO {
private:
  GLuint _id;

public:
  VAO(void);
  ~VAO();

  void bind(void) const;
  void unbind(void) const;
  void linkAttribute(const VBO& vbo, GLuint index, GLint size, GLenum type,
                     GLsizei stride, const void* pointer) const;
};

/* ========================================================================== */
/*                            VERTEXT BUFFER OBJECT                           */
/* ========================================================================== */

class VBO {
private:
  GLuint _id;

public:
  VBO(const std::vector<Vertex>& vertices);
  VBO(const std::vector<glm::vec3>& vertices);
  ~VBO();

  void bind(void) const;
  void unbind(void) const;
};

/* ========================================================================== */
/*                            ELEMENT BUFFER OBJECT                           */
/* ========================================================================== */

class EBO {
private:
  GLuint _id;

public:
  EBO(const std::vector<GLuint>& indices);
  ~EBO();

  void bind(void) const;
  void unbind(void) const;
};

#endif
