#include "components/Camera.hpp"
#include "core/Shader.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include <core/Object.hpp>
#include <vector>

class CubeMap {
private:
  static const std::vector<glm::vec3> _vertices;
  static const std::vector<GLuint> _indices;

  VAO _vao;
  VBO _vbo;
  EBO _ebo;

public:
  GLuint texture;
  Shader shader;

public:
  CubeMap(const char* textures[6]);
  ~CubeMap();

  void render(Camera& camera);
};
