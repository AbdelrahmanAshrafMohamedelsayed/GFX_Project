#include "rerender.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {
    // Reads linearVelocity & angularVelocity from the given json object
    void RerenderComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
    }

    RerenderComponent::~RerenderComponent(){
        //delete mesh;
        //delete material;
    }
    
}