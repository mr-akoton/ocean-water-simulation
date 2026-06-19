#include "glad/glad.h"
#include <core/Texture.hpp>
#include <core/Engine.hpp>

using namespace glm;

/* ========================================================================== */
/*                          CONTRUCTOR AND DESTRUCTOR                         */
/* ========================================================================== */

Texture::Texture(const char* file, GLuint slot, GLenum format, GLenum inFormat,
                 GLenum dataType)
    : _unit(slot) {
  unsigned char* bytes;

  stbi_set_flip_vertically_on_load(true);
  bytes = stbi_load(file, &width, &height, &channel, 0);

  glCreateTextures(GL_TEXTURE_2D, 1, &_id);

  glTextureStorage2D(_id, 1, inFormat, width, height);
  glTextureSubImage2D(_id, 0, 0, 0, width, height, format, dataType, bytes);
  glGenerateTextureMipmap(_id);

  glTextureParameteri(_id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTextureParameteri(_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTextureParameteri(_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(_id, GL_TEXTURE_WRAP_T, GL_REPEAT);

  stbi_image_free(bytes);
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
  glTextureParameteri(_id, GL_TEXTURE_MIN_FILTER, filter);
  glTextureParameteri(_id, GL_TEXTURE_MAG_FILTER, filter);
}

void Texture::setWrap(GLuint wrap) const {
  glTextureParameteri(_id, GL_TEXTURE_WRAP_S, wrap);
  glTextureParameteri(_id, GL_TEXTURE_WRAP_T, wrap);
}

void Texture::setWrapBorderColor(vec4 color) const {
  glTextureParameterfv(_id, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(color));
}

/* ========================================================================== */
/*                                   UPDATE                                   */
/* ========================================================================== */

void Texture::updateShaderTexture(Shader& shader, const std::string uniform,
                                  GLuint unit) const {
  shader.enable();
  glBindTextureUnit(unit, _id);
  shader.setInt(uniform, unit);
}
