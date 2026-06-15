#include <components/Water.hpp>

constexpr const char* VERTEX_SHADER = "shader/water-vertex.glsl";
constexpr const char* FRAGMENT_SHADER = "shader/water-fragment.glsl";

using namespace glm;

/* ========================================================================== */
/*                         CONSTRUCTOR AND DESTRUCTOR                         */
/* ========================================================================== */

Water::Water(unsigned int width, unsigned int height, float gridSize,
             vec3 position)
    : width(width), height(height), gridSize(gridSize), iteration(32),
      amplitude(13.0), frequency(0.02), speed(2.0), drag(1.4), peakMax(1.0),
      peakOffset(1.0), amplitudeMult(0.82), frequencyMult(1.18),
      speedMult(1.07), iterationMult(1232.399963), position(position),
      color(0.172f, 0.3412f, 0.4667f), model(1.0f),
      shader(VERTEX_SHADER, FRAGMENT_SHADER) {
  model = translate(model, position);
}

Water::~Water() {}

/* ========================================================================== */
/*                                    INIT                                    */
/* ========================================================================== */

void Water::init() {
  for (unsigned int z = 0; z < height; z++) {
    for (unsigned int x = 0; x < width; x++) {
      Vertex vertex;
      vertex.position = vec3((float)x * gridSize, 0.0f, (float)z * gridSize);
      vertex.color = vec3(0.172f, 0.3412f, 0.4667f);
      _vertices.push_back(vertex);

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

  _vao.bind();
  VBO vbo(_vertices);
  vbo.bind();
  EBO ebo(_indices);
  ebo.bind();

  _vao.linkAttribute(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)(0));
  _vao.linkAttribute(vbo, 1, 3, GL_FLOAT, sizeof(Vertex),
                     (void*)(3 * sizeof(float)));

  _vao.unbind();
  vbo.unbind();
  ebo.unbind();
}

/* ========================================================================== */
/*                                   UPDATE                                   */
/* ========================================================================== */

void Water::render(Camera& camera, const vec3 lightDirection,
                   const vec3 lightColor, const Fog fogParam) const {
  _vao.bind();

  shader.enable();
  camera.updateShaderMatrix(shader, "u_projection");
  shader.setVec3("u_color", color);
  shader.setMat4("u_model", model);
  shader.setMat3("u_imodel", transpose(inverse(mat3(model))));
  shader.setVec3("u_lightDirection", lightDirection);
  shader.setVec3("u_lightColor", lightColor);
  shader.setVec3("u_viewPosition", camera.position);

  shader.setVec3("u_fogColor", fogParam.color);
  shader.setFloat("u_fogNear", fogParam.near);
  shader.setFloat("u_fogFar", fogParam.far);
  shader.setFloat("u_fogSteepness", fogParam.steepnees);
  shader.setFloat("u_fogOffset", fogParam.offset);

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
  shader.setFloat("u_iterationMult", iterationMult);

  shader.setFloat("u_time", glfwGetTime());

  glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
}
