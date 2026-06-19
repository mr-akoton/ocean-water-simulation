#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>

#include <core/Shader.hpp>

#include <string>

class Texture {
public:
  int width;
  int height;
  int channel;

private:
  GLuint _id;
  GLuint _unit;

public:
  Texture(const char* file, GLuint slot, GLenum format, GLenum inFormat,
          GLenum dataType);
  ~Texture();

  void setFilter(GLuint filter) const;
  void setWrap(GLuint wrap) const;
  void setWrapBorderColor(glm::vec4 color) const;
  void updateShaderTexture(Shader& shader, const std::string uniform,
                           GLuint unit) const;

  void bind(void) const;
  void unbind(void) const;
};

#endif
