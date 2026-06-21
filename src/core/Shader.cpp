#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <core/Shader.hpp>
#include <fstream>
#include <iostream>

using namespace glm;

/* ========================================================================== */
/*                         CONSTRUCTOR AND DESTRUCTOR                         */
/* ========================================================================== */

static std::string getFileContent(const std::string path) {
  std::string content;
  std::ifstream file(path);

  if (file.is_open()) {
    file.seekg(0, std::ios::end);
    content.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&content[0], content.size());
    file.close();
    return content;
  }

  throw std::runtime_error("Error: failed to open " + path);
}

Shader::Shader(const std::string vertex, const std::string fragment,
               const std::string tesControl, const std::string tesEval)
    : _vertexShaderFile(vertex), _tesControlShaderFile(tesControl),
      _tesEvalShaderFile(tesEval), _fragmentShaderFile(fragment) {
  bool hasTesselation = (not tesControl.empty() and not tesEval.empty());

  const std::string vertexContent = getFileContent(vertex);
  const std::string fragmentContent = getFileContent(fragment);
  const std::string tesControlContent =
      hasTesselation ? getFileContent(tesControl) : "";
  const std::string tesEvalContent =
      hasTesselation ? getFileContent(tesEval) : "";

  const char* vertexSource = vertexContent.c_str();
  const char* tesControlSource = tesControlContent.c_str();
  const char* tesEvalSource = tesEvalContent.c_str();
  const char* fragmentSource = fragmentContent.c_str();

  GLuint vertexShader, fragmentShader, tesControlShader, tesEvalShader;

  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, nullptr);
  glCompileShader(vertexShader);
  _debugShaderCompilation(vertexShader, vertex);

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
  glCompileShader(fragmentShader);
  _debugShaderCompilation(fragmentShader, fragment);

  if (hasTesselation) {
    tesControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
    glShaderSource(tesControlShader, 1, &tesControlSource, nullptr);
    glCompileShader(tesControlShader);
    _debugShaderCompilation(tesControlShader, tesControl);

    tesEvalShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
    glShaderSource(tesEvalShader, 1, &tesEvalSource, nullptr);
    glCompileShader(tesEvalShader);
    _debugShaderCompilation(tesEvalShader, tesEval);
  }

  _id = glCreateProgram();
  glAttachShader(_id, vertexShader);
  glAttachShader(_id, fragmentShader);
  if (hasTesselation) {
    glAttachShader(_id, tesControlShader);
    glAttachShader(_id, tesEvalShader);
  }
  glLinkProgram(_id);
  _debugProgramLink(_id);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  if (hasTesselation) {
    glDeleteShader(tesControlShader);
    glDeleteShader(tesEvalShader);
  }
}

Shader::~Shader() { glDeleteProgram(_id); }

/* ========================================================================== */
/*                                    STATE                                   */
/* ========================================================================== */

void Shader::enable(void) const { glUseProgram(_id); }

/* ========================================================================== */
/*                                   SETTER                                   */
/* ========================================================================== */

void Shader::setInt(const std::string& uniform, int value) const {
  glUniform1i(glGetUniformLocation(_id, uniform.c_str()), value);
}

void Shader::setBool(const std::string& uniform, bool value) const {
  glUniform1i(glGetUniformLocation(_id, uniform.c_str()),
              static_cast<int>(value));
}

void Shader::setFloat(const std::string& uniform, float value) const {
  glUniform1f(glGetUniformLocation(_id, uniform.c_str()), value);
}

void Shader::setVec2(const std::string& uniform, const vec2& value) const {
  glUniform2f(glGetUniformLocation(_id, uniform.c_str()), value.x, value.y);
}

void Shader::setVec3(const std::string& uniform, const vec3& value) const {
  glUniform3f(glGetUniformLocation(_id, uniform.c_str()), value.x, value.y,
              value.z);
}

void Shader::setVec4(const std::string& uniform, const vec4& value) const {
  glUniform4f(glGetUniformLocation(_id, uniform.c_str()), value.x, value.y,
              value.z, value.w);
}

void Shader::setMat2(const std::string& uniform, const mat2& value) const {
  glUniformMatrix2fv(glGetUniformLocation(_id, uniform.c_str()), 1, GL_FALSE,
                     value_ptr(value));
}

void Shader::setMat3(const std::string& uniform, const mat3& value) const {
  glUniformMatrix3fv(glGetUniformLocation(_id, uniform.c_str()), 1, GL_FALSE,
                     value_ptr(value));
}

void Shader::setMat4(const std::string& uniform, const mat4& value) const {
  glUniformMatrix4fv(glGetUniformLocation(_id, uniform.c_str()), 1, GL_FALSE,
                     value_ptr(value));
}

/* ========================================================================== */
/*                                    DEBUG                                   */
/* ========================================================================== */

constexpr int INFO_LOG_BUFFER_SIZE = 1024;

void Shader::_debugShaderCompilation(GLuint id, std::string file) const {
  int success;
  char infoLog[INFO_LOG_BUFFER_SIZE];

  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (not success) {
    glGetShaderInfoLog(id, INFO_LOG_BUFFER_SIZE, nullptr, infoLog);
    std::cerr << "Error: " << file << ": ShaderCompilationFailed\n"
              << infoLog << std::endl;
  }
}

void Shader::_debugProgramLink(GLuint id) const {
  int success;
  char infoLog[INFO_LOG_BUFFER_SIZE];

  glGetProgramiv(id, GL_LINK_STATUS, &success);
  if (not success) {
    glGetProgramInfoLog(id, INFO_LOG_BUFFER_SIZE, nullptr, infoLog);
    std::cerr << "Error: ProgramLinkFailed\n" << infoLog << std::endl;
  }
}
