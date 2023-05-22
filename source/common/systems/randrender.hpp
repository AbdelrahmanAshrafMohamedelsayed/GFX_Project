#pragma once

#include "../ecs/world.hpp"
#include "../components/randrender.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/movement.hpp"


#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

#include <iostream>
#include <vector>
#include <chrono>


namespace our
{

    // system for render cars in a randomized pattern
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
                // Get the randrender component if it exists
                RandRenderComponent* Rerender = entity->getComponent<RandRenderComponent>();

                // If the randrender component exists push it our array
                if(Rerender ){
                    cars.push_back(Rerender);
                }
            }
            
            for (auto car : cars)
            {
              // check if our car passes other cars
              if((car->getOwner()->localTransform.position.z - controller->getOwner()->localTransform.position.z) >= 10){
                
                // get range after our car to render cars in a position after our car
                int min_z = -(controller->getOwner()->localTransform.position.z-50);
                int max_z = -(controller->getOwner()->localTransform.position.z-500);

                // get random number in the range
                int output = min_z + (rand() % static_cast<int>(max_z - min_z + 1));

                // set the new position of the car
                car->getOwner()->localTransform.position.z = -output;

                // get range in x-axis
                int min_x = -7;
                int max_x = 7;
                // get a random value within range
                int output_x = min_x + (rand() % static_cast<int>(max_x - min_x + 1));
                // set the new position of the car
                car->getOwner()->localTransform.position.x = output_x;
                MovementComponent *movement =  car->getOwner()->getComponent<MovementComponent>();
                if(movement){
                  // get range for speed
                  int max_speed = 30;
                  int min_speed = 5;
                  // get a random value within range
                  int output_speed = min_speed + (rand() % static_cast<int>(max_speed - min_speed + 1));
                  // set new speed
                  movement->linearVelocity = glm::vec3(0,0,-output_speed);
                }
                
              }
            }

        }

    };

}
