#include <core/Texture.hpp>

using namespace glm;

/* ========================================================================== */
/*                          CONTRUCTOR AND DESTRUCTOR                         */
/* ========================================================================== */

Texture::Texture(const char* file, GLuint slot, GLenum format, GLenum pixType)
    : _unit(slot) {
  unsigned char* bytes;

  stbi_set_flip_vertically_on_load(true);
  bytes = stbi_load(file, &width, &height, &channel, 0);

  glGenTextures(1, &_id);
  glActiveTexture(GL_TEXTURE0 + _unit);
  this->bind();

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, pixType,
               bytes);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(bytes);
  this->unbind();
}

Texture::~Texture() { glDeleteTextures(1, &_id); }

/* ========================================================================== */
/*                                   BINDER                                   */
/* ========================================================================== */

void Texture::bind(void) const {
  glActiveTexture(GL_TEXTURE0 + _unit);
  glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture::unbind(void) const { glBindTexture(GL_TEXTURE_2D, 0); }

/* ========================================================================== */
/*                                   SETTER                                   */
/* ========================================================================== */

void Texture::setFilter(GLuint filter) const {
  this->bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
  this->unbind();
}

void Texture::setWrap(GLuint wrap) const {
  this->bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
  this->unbind();
}

void Texture::setWrapBorderColor(vec4 color) const {
  this->bind();
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR,
                   glm::value_ptr(color));
  this->unbind();
}

/* ========================================================================== */
/*                                   UPDATE                                   */
/* ========================================================================== */

void Texture::updateShaderTexture(Shader& shader, const std::string uniform,
                                  GLuint unit) const {
  shader.enable();
  shader.setInt(uniform, unit);
}
