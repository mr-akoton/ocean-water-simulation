#include "glad/glad.h"
#include <core/Object.hpp>

/* ========================================================================== */
/*                          CONTRUCTOR AND DESTRUCTOR                         */
/* ========================================================================== */

FBO::FBO() { glCreateFramebuffers(1, &_id); }

FBO::~FBO() { glDeleteFramebuffers(1, &_id); }

/* ========================================================================== */
/*                                   BINDER                                   */
/* ========================================================================== */

void FBO::bind(void) const { glBindFramebuffer(GL_FRAMEBUFFER, _id); }

void FBO::unbind(void) const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void FBO::attachTexture(GLuint texture, GLenum attachPoint, GLint mipmapLevel) {
  glNamedFramebufferTexture(_id, attachPoint, texture, mipmapLevel);
};

GLuint FBO::getID(void) const { return _id; }
