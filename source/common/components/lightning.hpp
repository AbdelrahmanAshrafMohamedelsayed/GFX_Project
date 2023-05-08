#pragma once

#include "../ecs/component.hpp"
#include "../shader/shader.hpp"
#include <glm/glm.hpp>

namespace our {

   enum class LightType
   {
       DirectionalLight, 
       PointLight,
       SpotLight
   };


    class Lightning_Component : public Component {
    public:
        
        LightType light_type;
        // ambient, diffuse, specular
        glm::vec3 diffuse = glm::vec3(0, 0, 0); 
        glm::vec3 specular = glm::vec3(0, 0, 0); 
        glm::vec3 direction = glm::vec3(0, 0, 0); 

        // attenuation, cone angles
        glm::vec3 attenuation = glm::vec3(0.0f, 0.0f, 0.0f); 
        glm::vec2 cone_angles = glm::vec2(0.0f, 0.0f);

        // color
        glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);
        
        // The ID of this component type is "Light"
        static std::string getID() { return "Light"; }

        // Reads Light data from the given json object
        void deserialize (const nlohmann::json& data) override ;
    };

}