#pragma once

#include "../ecs/world.hpp"
#include "../components/randrender.hpp"
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
    class RandRenderSystem {
    public:

        // This should be called every frame to update all entities containing a MovementComponent. 
        void update(World* world, float deltaTime) {
          FreeCameraControllerComponent *controller = nullptr;
          std::vector<RandRenderComponent*> cars;


          for(auto entity : world->getEntities()){
                // Get the Car
                controller = entity->getComponent<FreeCameraControllerComponent>();
                if(controller)break;
            }

            // For each entity in the world
            for(auto entity : world->getEntities()){
                // Get the movement component if it exists
                RandRenderComponent* Rerender = entity->getComponent<RandRenderComponent>();

                // If the movement component exists
                if(Rerender ){
                    cars.push_back(Rerender);
                }
            }
            
            for (auto car : cars)
            {
              if((car->getOwner()->localTransform.position.z - controller->getOwner()->localTransform.position.z) >= 10){
                
                int min_z = -(controller->getOwner()->localTransform.position.z-50);
                int max_z = -(controller->getOwner()->localTransform.position.z-80);

                int output = min_z + (rand() % static_cast<int>(max_z - min_z + 1));

                car->getOwner()->localTransform.position.z = -output;

                int min_x = -7;
                int max_x = 7;
                int output_x = min_x + (rand() % static_cast<int>(max_x - min_x + 1));
                car->getOwner()->localTransform.position.x = output_x;
              }
            }

        }

    };

}
