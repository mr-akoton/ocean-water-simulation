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

/* ========================================================================== */
/*                            VERTEXT BUFFER OBJECT                           */
/* ========================================================================== */

class VBO {
private:
  GLuint _id;

public:
  VBO(void);
  VBO(const std::vector<Vertex>& vertices);
  VBO(const std::vector<glm::vec2>& vertices);
  VBO(const std::vector<glm::vec3>& vertices);
  VBO(const std::vector<glm::vec4>& vertices);
  ~VBO();

  void bind(void) const;
  void unbind(void) const;
  void bindData(const std::vector<Vertex>& vertices);
  void bindData(const std::vector<glm::vec2>& vertices);
  void bindData(const std::vector<glm::vec3>& vertices);
  void bindData(const std::vector<glm::vec4>& vertices);
  GLuint getID(void) const;
};

/* ========================================================================== */
/*                            ELEMENT BUFFER OBJECT                           */
/* ========================================================================== */

class EBO {
private:
  GLuint _id;

public:
  EBO(void);
  EBO(const std::vector<GLuint>& indices);
  ~EBO();

  void bindData(const std::vector<GLuint>& indices);
  void bind(void) const;
  void unbind(void) const;
  GLuint getID(void) const;
};

/* ========================================================================== */
/*                             FRAME BUFFER OBJECT                            */
/* ========================================================================== */

class FBO {
private:
  GLuint _id;

public:
  FBO();
  ~FBO();

  void bind(void) const;
  void unbind(void) const;
  void attachTexture(GLuint texture, GLenum attachPoint, GLint mipmapLevel);
  GLuint getID(void) const;
};

/* ========================================================================== */
/*                             RENDER BUFFER OBJECT                           */
/* ========================================================================== */

class RBO {
private:
  GLuint _id;

public:
  RBO(int width, int height);
  ~RBO();

  void bind(void) const;
  void unbind(void) const;
};

#endif
