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
        diffuse = data.value("diffuse", glm::vec3(1, 0.9, 0.7)));
        specular = data.value("specular", glm::vec3(1, 0.9, 0.7)));
        direction = data.value("direction",  glm::vec3(0, -1, 0)));
        attenuation = data.value("attenuation", glm::vec3(1, 0, 0)));
        cone_angles = data.value("cone_angles", glm::vec2(0.0f, 0.0f));
        color = data.value("color", glm::vec3(0.0f, 0.0f, 0.0f));
    }
}