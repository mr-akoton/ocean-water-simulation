#include "glad/glad.h"
#include <core/Object.hpp>

/* ========================================================================== */
/*                          CONTRUCTOR AND DESTRUCTOR                         */
/* ========================================================================== */

RBO::RBO(int width, int height) {
  glCreateRenderbuffers(1, &_id);
  glNamedRenderbufferStorage(_id, GL_DEPTH24_STENCIL8, width, height);
}

RBO::~RBO() { glDeleteRenderbuffers(1, &_id); }

/* ========================================================================== */
/*                                   BINDER                                   */
/* ========================================================================== */

void RBO::bind(void) const { glBindRenderbuffer(GL_RENDERBUFFER, _id); }

void RBO::unbind(void) const { glBindRenderbuffer(GL_RENDERBUFFER, 0); }
