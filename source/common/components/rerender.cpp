#include "rerender.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {
    void RerenderComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
    }

    RerenderComponent::~RerenderComponent(){
    }
    
}