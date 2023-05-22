#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our {

    // this component denotes randomization system for rerendering cars in a random places with random speed
    class RandRenderComponent : public Component {
    public:
        // The ID of this component type is "RandRender"
        static std::string getID() { return "RandRender"; }

        void deserialize(const nlohmann::json& data) override;

    };

}