#ifndef FBO_HPP
#define FBO_HPP

#include <glad/glad.h>

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

#endif
