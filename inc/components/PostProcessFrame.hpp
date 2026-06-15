#ifndef POST_PROCESS_FRAME
#define POST_PROCESS_FRAME

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <core/Object.hpp>
#include <core/Texture.hpp>
#include <vector>

class PostProcessFrame {
private:
  static const std::vector<glm::vec4> _vertices;
  static const std::vector<GLuint> _indices;

  VAO _vao;
  VBO _vbo;
  FBO _fbo;
  RBO _rbo;
  Texture _texture;

public:
  PostProcessFrame();
  ~PostProcessFrame();
};

#endif
