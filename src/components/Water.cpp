#include "core/Object.hpp"
#include "glad/glad.h"
#include "glm/ext/vector_float2.hpp"
#include <components/Water.hpp>
#include <settings/SettingsData.hpp>

static const char* VERTEX_SHADER = "shader/water-vertex.glsl";
static const char* FRAGMENT_SHADER = "shader/water-fragment.glsl";

using namespace glm;

/* ========================================================================== */
/*                         CONSTRUCTOR AND DESTRUCTOR                         */
/* ========================================================================== */

Water::Water(unsigned int width, unsigned int height, float gridSize)
    : width(width), height(height), gridSize(gridSize), iteration(32),
      amplitude(8.0), frequency(0.02), speed(1.4), drag(1.4), peakMax(1.0),
      peakOffset(1.0), amplitudeMult(0.82), frequencyMult(1.18),
      speedMult(1.07), iterationMult(1.18), ambientColor(1.0f),
      ambientStrength(0.4), specularStrength(1.0), shininess(256),
      position(0.0f), color(0.629f, 0.883f, 0.917f), model(1.0f), imodel(1.0f),
      shader(VERTEX_SHADER, FRAGMENT_SHADER), _indicesCount(0) {
  model = translate(model, position);
  imodel = transpose(inverse(mat3(model)));

  float iter = 0.0f;
  for (int i = 0; i < MAX_WAVE_ITERATION; i++) {
    waveDirections.push_back(normalize(vec2(std::cos(iter), std::sin(iter))));
    iter += iterationMult;
  }

  glCreateTextures(GL_TEXTURE_1D, 1, &_directionTexture);
  glTextureStorage1D(_directionTexture, 1, GL_RG32F, MAX_WAVE_ITERATION);
  glTextureSubImage1D(_directionTexture, 0, 0, MAX_WAVE_ITERATION, GL_RG,
                      GL_FLOAT, waveDirections.data());
  
  glTextureParameteri(_directionTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTextureParameteri(_directionTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  shader.enable();
  shader.setInt("u_skybox", 0);
  shader.setInt("u_directionTexture", 1);
}

Water::~Water() {}

/* ========================================================================== */
/*                                    INIT                                    */
/* ========================================================================== */

void Water::init() {
  for (unsigned int z = 0; z < height; z++) {
    for (unsigned int x = 0; x < width; x++) {
      _vertices.push_back(vec2((float)x * gridSize, (float)z * gridSize));

      if (x != width - 1 and z != height - 1) {
        unsigned int i = (z * width) + x;

        _indices.push_back(i);
        _indices.push_back(i + width);
        _indices.push_back(i + 1);

        _indices.push_back(i + width);
        _indices.push_back(i + width + 1);
        _indices.push_back(i + 1);
      }
    }
  }

  _indicesCount = _indices.size();

  _vbo.bindData(_vertices);
  _ebo.bindData(_indices);

  _vao.linkAttribute(0, 2, GL_FLOAT, 0);
  _vao.linkVBO(_vbo, 0, 0, sizeof(glm::vec2));
  _vao.linkEBO(_ebo);

  _vertices.clear();
  _vertices.shrink_to_fit();
  _indices.clear();
  _indices.shrink_to_fit();
}

/* ========================================================================== */
/*                                   UPDATE                                   */
/* ========================================================================== */

void Water::render(Camera& camera, Environment& environment) const {
  _vao.bind();

  shader.enable();
  camera.updateShaderMatrix(shader, "u_projection");
  shader.setVec3("u_color", color);
  shader.setMat4("u_model", model);
  shader.setMat3("u_imodel", imodel);
  shader.setVec3("u_lightDirection", environment.lightDirection);
  shader.setVec3("u_lightColor", environment.lightColor);
  shader.setVec3("u_viewPosition", camera.position);

  shader.setInt("u_waveIteration", iteration);
  shader.setFloat("u_amplitude", amplitude);
  shader.setFloat("u_frequency", frequency);
  shader.setFloat("u_speed", speed);
  shader.setFloat("u_drag", drag);
  shader.setFloat("u_peakMax", peakMax);
  shader.setFloat("u_peakOffset", peakOffset);

  shader.setFloat("u_amplitudeMult", amplitudeMult);
  shader.setFloat("u_frequencyMult", frequencyMult);
  shader.setFloat("u_speedMult", speedMult);

  shader.setFloat("u_ambientStrength", ambientStrength);
  shader.setVec3("u_ambientColor", ambientColor);
  shader.setFloat("u_specularStrength", specularStrength);
  shader.setInt("u_shininess", shininess);

  shader.setFloat("u_time", glfwGetTime());

  glBindTextureUnit(0, environment.skybox.texture);
  glBindTextureUnit(1, _directionTexture);
  glDrawElements(GL_TRIANGLES, _indicesCount, GL_UNSIGNED_INT, 0);
}
