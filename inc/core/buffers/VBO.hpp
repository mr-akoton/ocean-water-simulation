#ifndef VBO_HPP
#define VBO_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
  glm::vec3 position;
  glm::vec3 color;
};

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

#endif
