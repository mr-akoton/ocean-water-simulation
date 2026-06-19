#include "glad/glad.h"
#include "glm/ext/vector_float4.hpp"
#include <components/Environment.hpp>
#include <components/CubeMap.hpp>
#include <settings/SettingsData.hpp>
#include <iostream>

static const char* VERTEX_SHADER = "shader/postpro-vertex.glsl";
static const char* FRAGMENT_SHADER = "shader/postpro-fragment.glsl";

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
    : shader(VERTEX_SHADER, FRAGMENT_SHADER),
      skybox((const char**)SKYBOX_FACES), skyColor(0.0f),
      lightDirection(-0.2f, -0.2f, -1.0f), lightColor(1.0f) {
  _vbo.bindData(_vertices);
  _vao.linkVBO(_vbo, 0, 0, sizeof(glm::vec4));
  _vao.linkAttribute(0, 2, GL_FLOAT, 0);
  _vao.linkAttribute(1, 2, GL_FLOAT, 2 * sizeof(float));

  // Init Color texture
  glCreateTextures(GL_TEXTURE_2D, 1, &_colorTexture);
  glTextureStorage2D(_colorTexture, 1, GL_RGB8, WINDOW_WIDTH, WINDOW_HEIGHT);
  glTextureParameteri(_colorTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTextureParameteri(_colorTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTextureParameteri(_colorTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTextureParameteri(_colorTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  _fbo.attachTexture(_colorTexture, GL_COLOR_ATTACHMENT0, 0);

  // Init Depth texture
  glCreateTextures(GL_TEXTURE_2D, 1, &_depthTexture);
  glTextureStorage2D(_depthTexture, 1, GL_DEPTH_COMPONENT24, WINDOW_WIDTH,
                     WINDOW_HEIGHT);
  glTextureParameteri(_depthTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTextureParameteri(_depthTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTextureParameteri(_depthTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTextureParameteri(_depthTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
  glTextureParameterfv(_depthTexture, GL_TEXTURE_BORDER_COLOR, borderColor);
  _fbo.attachTexture(_depthTexture, GL_DEPTH_ATTACHMENT, 0);

  auto fboStatus = glCheckNamedFramebufferStatus(_fbo.getID(), GL_FRAMEBUFFER);
  if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
    std::cerr << "Framebuffer error: " << fboStatus << std::endl;
  }

  shader.enable();
  shader.setInt("u_colorTexture", 0);
  shader.setInt("u_depthTexture", 1);
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

  glBindTextureUnit(0, _colorTexture);
  glBindTextureUnit(1, _depthTexture);

  glDrawArrays(GL_TRIANGLES, 0, 6);
  glEnable(GL_DEPTH_TEST);
}
