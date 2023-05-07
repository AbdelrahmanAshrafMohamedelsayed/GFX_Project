#pragma once

#include "../ecs/world.hpp"
#include "../components/rerender.hpp"
#include "../components/free-camera-controller.hpp"


#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

#include <iostream>
#include <vector>
#include <chrono>


namespace our
{

    // The movement system is responsible for moving every entity which contains a MovementComponent.
    // This system is added as a simple example for how use the ECS framework to implement logic. 
    // For more information, see "common/components/movement.hpp"
    class RerenderSystem {
    public:

        // This should be called every frame to update all entities containing a MovementComponent. 
        void update(World* world, float deltaTime) {
          FreeCameraControllerComponent *controller = nullptr;
          std::vector<RerenderComponent*> roads;


          for(auto entity : world->getEntities()){
                // Get the Car
                controller = entity->getComponent<FreeCameraControllerComponent>();
                if(controller)break;
            }

            // For each entity in the world
            for(auto entity : world->getEntities()){
                // Get the movement component if it exists
                RerenderComponent* Rerender = entity->getComponent<RerenderComponent>();

                // If the movement component exists
                if(Rerender ){
                    roads.push_back(Rerender);
                }
            }
            int min_z = 100;
            for (auto road : roads)
            {
              if(road->getOwner()->localTransform.position.z < min_z){
                min_z = road->getOwner()->localTransform.position.z;
              }
            }

            if(controller->getOwner()->localTransform.position.z - min_z <= 80 ){
               for (auto road : roads)
              {
                road->getOwner()->localTransform.position.z -= 20;
                
              }
            }
        }

    };

}
