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

    // The free camera controller system is responsible for moving every entity which contains a FreeCameraControllerComponent.
    // This system is added as a slightly complex example for how use the ECS framework to implement logic.
    // For more information, see "common/components/free-camera-controller.hpp"

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
        bool update(World *world, float deltaTime)
        {
            vector<collisionComponent *> colliders;

            // store all the existed colliders in the scene
            for (auto entity : world->getEntities())
            {
                if (auto collider = entity->getComponent<collisionComponent>(); collider)
                {
                    colliders.emplace_back(collider);
                }
            }

            // here is the logic of the collision: if 2 types of colliders are collides the action should be taken:
            for (auto collider_1 : colliders)
            {
                string collider1_type = collider_1->getOwner()->name;

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

                    // get the collider's position
                    glm::vec3 collider2_position = collider_2->getOwner()->localTransform.position;

                    // get the collider's size
                    glm::vec3 collider2_size = collider_2->getOwner()->localTransform.scale;

                    // get the collider's max and min position
                    glm::vec3 collider2_max = collider2_position + collider2_size;
                    glm::vec3 collider2_min = collider2_position - collider2_size;

                    // if the robot is in the range of the obstacle, take an action
                    if (collider1_max.x == collider2_min.x || collider1_min.x == collider2_max.x ||
                        collider1_max.y == collider2_min.y || collider1_min.y == collider2_max.y ||
                        collider1_max.z == collider2_min.z || collider1_min.z == collider2_max.z)
                    {
                        if ((collider1_type == "car" && collider2_type == "racer")
                            || (collider1_type == "racer" && collider2_type == "racer") 
                            || (collider1_type == "me" && collider2_type == "racer")
                            || (collider1_type == "me" && collider2_type == "car"))
                        {
                            app->registerState<Menustate>("menu");
                            app->changeState("menu");
                            return false;
                        }
                    }
                }
            }
            return false;
        }
    };
}