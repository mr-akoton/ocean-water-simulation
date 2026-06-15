#include <components/PostProcessFrame.hpp>

const std::vector<glm::vec4> PostProcessFrame::_vertices{
    {1.0f, -1.0f, 1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f, 0.0f},
    {-1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f},
    {1.0f, -1.0f, 1.0f, 0.0f}, {-1.0f, 1.0f, 0.0f, 0.0f},
};

// PostProcessFrame::PostProcessFrame():
//   _vao(), _vbo(_vertices), _fbo(), _rbo() {

// }
