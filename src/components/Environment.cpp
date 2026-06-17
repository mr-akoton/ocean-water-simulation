#include <components/Environment.hpp>
#include <components/CubeMap.hpp>
#include <settings.hpp>
#include <iostream>

const std::vector<glm::vec4> Environment::_vertices{
    {-1.0f, -1.0f, 0.0f, 0.0f}, // 1
    {+1.0f, -1.0f, 1.0f, 0.0f}, // 2
    {+1.0f, +1.0f, 1.0f, 1.0f}, // 3
    {-1.0f, -1.0f, 0.0f, 0.0f}, // 4
    {+1.0f, +1.0f, 1.0f, 1.0f}, // 5
    {-1.0f, +1.0f, 0.0f, 1.0f}, // 6
};

/* ========================================================================== */
/*                         CONSTRUCTOR AND DESTRUCTOR                         */
/* ========================================================================== */

Environment::Environment(void)
    : shader("shader/postpro-vertex.glsl", "shader/postpro-fragment.glsl"),
      skybox((const char**)SKYBOX_FACES), skyColor(0.0f),
      lightDirection(-0.2f, -0.2f, -1.0f), lightColor(1.0f) {
  shader.enable();
  shader.setInt("u_colorTexture", 0);
  shader.setInt("u_depthTexture", 1);

  _vao.bind();
  _vbo.bindData(_vertices);
  _vao.linkAttribute(_vbo, 0, 2, GL_FLOAT, sizeof(glm::vec4), (void*)0);
  _vao.linkAttribute(_vbo, 1, 2, GL_FLOAT, sizeof(glm::vec4),
                     (void*)(2 * sizeof(float)));
  _vao.unbind();
  _vbo.unbind();

  _fbo.bind();
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &_colorTexture);
  glBindTexture(GL_TEXTURE_2D, _colorTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB,
               GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         _colorTexture, 0);

  glActiveTexture(GL_TEXTURE1);
  glGenTextures(1, &_depthTexture);
  glBindTexture(GL_TEXTURE_2D, _depthTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, WINDOW_WIDTH,
               WINDOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         _depthTexture, 0);

  auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
    std::cerr << "Framebuffer error: " << fboStatus << std::endl;
  }
  _fbo.unbind();
}

Environment::~Environment() {}

/* ========================================================================== */
/*                              ATTACH AND DETACH                             */
/* ========================================================================== */

void Environment::attach(void) const { _fbo.bind(); }

void Environment::detach(void) const { _fbo.unbind(); }

/* ========================================================================== */
/*                                   RENDER                                   */
/* ========================================================================== */

void Environment::render(Camera& camera) {
  glDisable(GL_DEPTH_TEST);

  _vao.bind();
  shader.enable();

  shader.setMat4("u_invProjection", glm::inverse(camera.getProjection(
                                        CAMERA_FOV, CAMERA_NEAR, CAMERA_FAR)));
  shader.setMat4("u_invView", glm::inverse(camera.getView()));
  shader.setVec3("u_viewPosition", camera.position);

  shader.setBool("u_enableFog", fog.enabled);
  shader.setFloat("u_fogNear", fog.near);
  shader.setFloat("u_fogFar", fog.far);
  shader.setFloat("u_fogSteepness", fog.steepness);
  shader.setFloat("u_fogOffset", fog.offset);
  shader.setVec3("u_fogColor", fog.color);

  glBindTexture(GL_TEXTURE_2D, _colorTexture);
  glBindTexture(GL_TEXTURE_2D, _depthTexture);

  glDrawArrays(GL_TRIANGLES, 0, 6);
  glEnable(GL_DEPTH_TEST);
}
