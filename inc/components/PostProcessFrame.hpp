#ifndef POST_PROCESS_FRAME
#define POST_PROCESS_FRAME

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <core/Engine.hpp>
#include <core/Object.hpp>
#include <core/Texture.hpp>
#include <core/Shader.hpp>

#include <vector>

class PostProcessFrame {
private:
  static const std::vector<glm::vec4> _vertices;

  VAO _vao;
  VBO _vbo;
  FBO _fbo;
  RBO _rbo;

public:
  Shader shader;

public:
  PostProcessFrame(void);
  ~PostProcessFrame();

  void bind(void) const;
  void unbind(void) const;
  void render(void) const;
};

#endif
