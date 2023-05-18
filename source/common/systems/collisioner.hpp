#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/collision.hpp"
#include "../systems/forward-renderer.hpp"
#include "../application.hpp"
#include "../../states/play-state.hpp"
#include "../../states/menu-state.hpp"
#include "../../states/game-over-state.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <vector>
#include <iostream>
#include <chrono>
using namespace std;
namespace our
{

    class collisionSystem
    {
        Application *app; // The application in which the state runs
    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application *app)
        {
            this->app = app;
        }

        // This should be called every frame to update all entities have any sort of colliders
        bool update(World *world, float deltaTime, float &velocity)
        {
            vector<collisionComponent *> colliders;
            // FreeCameraControllerComponent *controller = nullptr;
            // // std::vector<RerenderComponent*> roads;


            // for(auto entity : world->getEntities()){
            //       // Get the Car
            //       controller = entity->getComponent<FreeCameraControllerComponent>();
            //       if(controller)break;
            //   }

            // Store the colliders from the screen
            for (auto entity : world->getEntities())
            {
                if (auto collider = entity->getComponent<collisionComponent>(); collider)
                {
                    colliders.emplace_back(collider);
                }
            }

            // loop through the colliders to see if they are colliding
            for (auto collider_1 : colliders)
            {
                string collider1_type = collider_1->getOwner()->name;
                // std::cout << collider1_type << endl;

                // get the collider's position
                glm::vec3 collider1_position = collider_1->getOwner()->localTransform.position;

                // get the collider's size
                glm::vec3 collider1_size = collider_1->getOwner()->localTransform.scale;

                // get th collider's max and min position
                glm::vec3 collider1_max = collider1_position + collider1_size;
                glm::vec3 collider1_min = collider1_position - collider1_size;

                // check for collision with other colliders
                for (auto collider_2 : colliders)
                {
                    string collider2_type = collider_2->getOwner()->name;
                    // std::cout << velocity << endl;
                    // get the collider's position
                    glm::vec3 collider2_position = collider_2->getOwner()->localTransform.position;

                    // get the collider's size
                    glm::vec3 collider2_size = collider_2->getOwner()->localTransform.scale;

                    // get the collider's max and min position
                    glm::vec3 collider2_max = collider2_position + collider2_size;
                    glm::vec3 collider2_min = collider2_position - collider2_size;

                    // i want to check if a car hit a car then game over
                    if ((collider1_type == "me" && collider2_type == "racer") ) 
                    {
                        // i want to check if a collider1 hit a collider2 then game over
                        if ((collider1_max.x >= collider2_min.x && collider1_min.x <= collider2_max.x) &&
                            (collider1_max.z >= (collider2_min.z) && collider1_min.z <= (collider2_max.z + 1.5f)))
                        {
                            // if the car hit another car then game over
                            // app->getStateMachine()->changeState(new gameOverState());
                            if (velocity >= 25.0f){
                                app->registerState<GameOverstate>("game-over");
                                app->changeState("game-over");
                                return true;
                            } else {
                                // from 0 - 10 -> zero else -> 5
                                if (velocity >= 0.0f || velocity <= 10.0f)
                                    velocity = 0.0f;
                                else if (velocity > 10.0f || velocity <= 20.0f)
                                    velocity = 5.0f;

                                // collider_1->getOwner()->localTransform.position.z += 20.0f * deltaTime;
                                if ((velocity >= 0.0f || velocity <= 1.0f))
                                    collider_2->getOwner()->localTransform.position.z -= 5.0f * 1.0 * deltaTime;
                                else
                                    collider_2->getOwner()->localTransform.position.z -= 5.0f * velocity * deltaTime;

                                // std::cout << velocity << std::endl;
                            }
                            
                            
                        }
                    }
                    if ((collider1_type == "racer" && collider2_type == "racer") ) {
                        if ((collider1_max.x >= collider2_min.x && collider1_min.x <= collider2_max.x) &&
                            (collider1_max.z >= (collider2_min.z) && collider1_min.z <= (collider2_max.z + 1.5f))) {
                                 collider_1->getOwner()->localTransform.position.z -= 5.0f;
                                 collider_2->getOwner()->localTransform.position.z += 5.0f;
                            }
                    }

                    
                }
            }
            return false;
        }
    };
}