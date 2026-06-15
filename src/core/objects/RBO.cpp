#include <core/Object.hpp>

/* ========================================================================== */
/*                          CONTRUCTOR AND DESTRUCTOR                         */
/* ========================================================================== */

RBO::RBO(int width, int height) {
  glGenRenderbuffers(1, &_id);
  this->bind();
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, _id);
  this->unbind();
}

RBO::~RBO() { glDeleteRenderbuffers(1, &_id); }

/* ========================================================================== */
/*                                   BINDER                                   */
/* ========================================================================== */

void RBO::bind(void) const { glBindRenderbuffer(GL_RENDERBUFFER, _id); }

void RBO::unbind(void) const { glBindRenderbuffer(GL_RENDERBUFFER, 0); }
