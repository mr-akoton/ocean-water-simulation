#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Shader {
private:
  GLuint _id;
  std::string _vertexShaderFile;
  std::string _tesControlShaderFile;
  std::string _tesEvalShaderFile;
  std::string _fragmentShaderFile;

public:
  Shader(const std::string vertex, const std::string fragment,
         const std::string tesControl = "", const std::string tesEval = "");
  ~Shader();

  void enable(void) const;

  void setInt(const std::string& uniform, int value) const;
  void setBool(const std::string& uniform, bool value) const;
  void setFloat(const std::string& uniform, float value) const;
  void setVec2(const std::string& uniform, const glm::vec2& value) const;
  void setVec3(const std::string& uniform, const glm::vec3& value) const;
  void setVec4(const std::string& uniform, const glm::vec4& value) const;
  void setMat2(const std::string& uniform, const glm::mat2& value) const;
  void setMat3(const std::string& uniform, const glm::mat3& value) const;
  void setMat4(const std::string& uniform, const glm::mat4& value) const;

private:
  void _debugShaderCompilation(GLuint id, std::string file) const;
  void _debugProgramLink(GLuint id) const;
};

#endif
