#pragma once

#include "../ecs/component.hpp"
#include "glm/glm.hpp"

namespace our {

    class collisionComponent : public Component {
    public:
        // The ID of this component type is "Collision"
        static std::string getID() { return "Collision"; }

        // Reads Movement data from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}