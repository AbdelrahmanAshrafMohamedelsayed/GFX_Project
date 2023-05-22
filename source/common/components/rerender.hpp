#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our {

    // this component denotes infinity  system for rerendering cars in a in an infinity 
    class RerenderComponent : public Component {
    public:
        // The ID of this component type is "Rerender"
        static std::string getID() { return "Rerender"; }

        void deserialize(const nlohmann::json& data) override;

        ~RerenderComponent()override;
    };

}