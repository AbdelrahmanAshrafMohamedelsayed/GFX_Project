#include "lightning.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include "../deserialize-utils.hpp"
#include <asset-loader.hpp>

namespace our
{
    // Reads camera parameters from the given json object
    void Lightning_Component::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;
        std::string lightTypeStr = data.value("lightType", "directional");
        if (lightTypeStr == "directional")
        {
            light_type = LightType::DirectionalLight;
        }
        else if (lightTypeStr == "point")
        {
            light_type = LightType::PointLight;
        }
        else
        {
            light_type = LightType::SpotLight;
        }
        diffuse = glm::vec3 (data.value("diffuse", glm::vec3(1, 0.9, 0.7)));
        specular = glm::vec3 (data.value("specular", glm::vec3(1, 0.9, 0.7)));
        direction = glm::vec3 (data.value("direction",  glm::vec3(0, -1, 0)));
        attenuation = glm::vec3 (data.value("attenuation", glm::vec3(1, 0, 0)));
        // cone_angles = glm::vec2(data.value("cone_angles", glm::vec2(glm::radians(90.0f), glm::radians(120.0f))));
        cone_angles.x = glm::radians((float)data.value("cone_angles.inner",90));
        cone_angles.y = glm::radians((float)data.value("cone_angles.outer",120));
        color = glm::vec3 (data.value("color", glm::vec3(0.0f, 0.0f, 0.0f)));
    }
}