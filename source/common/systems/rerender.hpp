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

  // system for infinity illusion  
  class RerenderSystem
  {
  public:
    // This should be called every frame to update all entities containing a MovementComponent.
    void update(World *world, float deltaTime)
    {
      FreeCameraControllerComponent *controller = nullptr;
      std::vector<RerenderComponent *> roads;
      for (auto entity : world->getEntities())
      {
        // Get the Car
        controller = entity->getComponent<FreeCameraControllerComponent>();
        if (controller)
          break;
      }
      // For each entity in the world
      for (auto entity : world->getEntities())
      {
        // Get the rerender component if it exists
        RerenderComponent *Rerender = entity->getComponent<RerenderComponent>();
        // If the rerender component exists
        if (Rerender)
        {
          roads.push_back(Rerender);
        }
      }
      int min_z = 100;
      for (auto road : roads)
      {
        // store the min z where this is the furthest car from our car
        if (road->getOwner()->localTransform.position.z < min_z)
        {
          min_z = road->getOwner()->localTransform.position.z;
        }
      }
      // if this car near a certain value we will move all rerender components 
      if (controller->getOwner()->localTransform.position.z - min_z <= 100)
      {
        for (auto road : roads)
        {
          // be -20 so this make as if we are in infinity
          road->getOwner()->localTransform.position.z -= 20;
        }
      }
    }
  };
}
