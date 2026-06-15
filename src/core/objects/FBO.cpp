#include <core/Object.hpp>

/* ========================================================================== */
/*                          CONTRUCTOR AND DESTRUCTOR                         */
/* ========================================================================== */

FBO::FBO() { glGenFramebuffers(1, &_id); }

FBO::~FBO() { glDeleteFramebuffers(1, &_id); }

/* ========================================================================== */
/*                                   BINDER                                   */
/* ========================================================================== */

void FBO::bind(void) const { glBindFramebuffer(GL_FRAMEBUFFER, _id); }

void FBO::unbind(void) const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
