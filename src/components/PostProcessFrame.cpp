#include <components/PostProcessFrame.hpp>
#include <settings.hpp>
#include <iostream>

const std::vector<glm::vec4> PostProcessFrame::_vertices{
    {1.0f, -1.0f, 1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f, 0.0f},
    {-1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f},
    {1.0f, -1.0f, 1.0f, 0.0f}, {-1.0f, 1.0f, 0.0f, 0.0f},
};

/* ========================================================================== */
/*                         CONSTRUCTOR AND DESTRUCTOR                         */
/* ========================================================================== */

PostProcessFrame::PostProcessFrame(void)
    : _vao(), _vbo(_vertices), _fbo(), _rbo(WINDOW_WIDTH, WINDOW_HEIGHT),
      shader("shader/postpro-vertex.glsl", "shader/postpro-fragment.glsl") {
  _vao.bind();
  _vao.linkAttribute(_vbo, 0, 2, GL_FLOAT, sizeof(glm::vec4), (void*)0);
  _vao.linkAttribute(_vbo, 1, 2, GL_FLOAT, sizeof(glm::vec4),
                     (void*)(2 * sizeof(float)));
  _vao.unbind();

  _fbo.bind();
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cerr << "Framebuffer is not complete!" << std::endl;
  }
  _fbo.unbind();

  glActiveTexture(GL_TEXTURE0);
}

PostProcessFrame::~PostProcessFrame() {}

/* ========================================================================== */
/*                                   BINDER                                   */
/* ========================================================================== */

void PostProcessFrame::bind(void) const { _fbo.bind(); }

void PostProcessFrame::unbind(void) const { _fbo.unbind(); }

/* ========================================================================== */
/*                                  RENDERER                                  */
/* ========================================================================== */

void PostProcessFrame::render(void) const {
  _vao.bind();

  shader.enable();
  glDisable(GL_DEPTH_TEST);

  glDrawArrays(GL_TRIANGLES, 0, 6);
}
