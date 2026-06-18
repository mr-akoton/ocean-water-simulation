#include <settings/SettingsData.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void glm::to_json(json& j, const glm::vec3& v) {
  j = json::array({v.x, v.y, v.z});
}

void glm::from_json(const json& j, glm::vec3& v) {
  j.at(0).get_to<float>(v.x);
  j.at(1).get_to<float>(v.y);
  j.at(2).get_to<float>(v.z);
}
