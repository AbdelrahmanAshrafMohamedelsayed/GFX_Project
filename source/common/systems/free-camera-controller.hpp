#pragma once

#include "../ecs/world.hpp"
#include "../ecs/component.hpp"
#include "../components/camera.hpp"
#include "../components/mesh-renderer.hpp"
#include "../components/free-camera-controller.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our
{

    // The free camera controller system is responsible for moving every entity which contains a FreeCameraControllerComponent.
    // This system is added as a slightly complex example for how use the ECS framework to implement logic.
    // For more information, see "common/components/free-camera-controller.hpp"
    class FreeCameraControllerSystem
    {
        Application *app;          // The application in which the state runs
        bool mouse_locked = false; // Is the mouse locked

    public:
        // define the FreeCameraControllerSystem::score of the player
        int score = 0;
        float speed = 5.0f; // The speed of the car
        int prev_time = 0;
        float time = 0.0f;  // The time since the beginning of the game
        // static float speedStatic; // The speed of the car
        // static float timeStatic;  // The time since the beginning of the game
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application *app)
        {
            this->app = app;
        }
        // give initial values to the static variables

        // This should be called every frame to update all entities containing a FreeCameraControllerComponent
        void update(World *world, float deltaTime, float &velocity)
        {
            //  make the FreeCameraControllerSystem::score increase with the speed and the time in a linear way and suitable way and print it

            // std::cout << "FreeCameraControllerSystem::score: " << FreeCameraControllerSystem::score << std::endl;
            // speedStatic = speed;
            // timeStatic = time;
            time += deltaTime;
            if(time-prev_time >=1){
                prev_time = time;
                score += (int)(speed * 0.1);
                app->score = score;
            }
            // increase speed by 5 every 1 seconds
            // if (time > 1.0f)
            // {
            //     speed += 5.0f;
            //     time = 0.0f;
            // }
            // inrease the speed linearly with time

            

            // First of all, we search for an entity containing both a CameraComponent and a FreeCameraControllerComponent
            // As soon as we find one, we break
            CameraComponent *camera = nullptr;
            FreeCameraControllerComponent *controller = nullptr;
            MeshRendererComponent *MeshRenderer = nullptr;
            for (auto entity : world->getEntities())
            {
                // camera = entity->getComponent<CameraComponent>();
                MeshRenderer = entity->getComponent<MeshRendererComponent>();
                controller = entity->getComponent<FreeCameraControllerComponent>();
                if (MeshRenderer && controller)
                    break;
            }
            // If there is no entity with both a CameraComponent and a FreeCameraControllerComponent, we can do nothing so we return
            if (!(MeshRenderer && controller))
                return;
            // Get the entity that we found via getOwner of camera (we could use controller->getOwner())
            Entity *entity = MeshRenderer->getOwner();

            // // If the left mouse button is pressed, we lock and hide the mouse. This common in First Person Games.

            // We get a reference to the entity's position and rotation
            glm::vec3 &position = entity->localTransform.position;
            glm::vec3 &rotation = entity->localTransform.rotation;

            // We get the camera model matrix (relative to its parent) to compute the front, up and right directions
            glm::mat4 matrix = entity->localTransform.toMat4();

            glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)),
                      up = glm::vec3(matrix * glm::vec4(0, 1, 0, 0)),
                      right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));

            glm::vec3 current_sensitivity = controller->positionSensitivity;
            current_sensitivity += glm::vec3(0, 0, 6);

            if (app->getKeyboard().isPressed(GLFW_KEY_SPACE))
            {
                // current_sensitivity -= glm::vec3(0, 0, 3);
                // decrease the speed linearly with time
                if (FreeCameraControllerSystem::speed > 0.0f){
                    
                    FreeCameraControllerSystem::speed -= 3.0f * deltaTime;
                    velocity = speed;
                }
            }
            else
            {
                // increase the speed linearly with time
                if (FreeCameraControllerSystem::speed < 200.0f){
                    FreeCameraControllerSystem::speed += 1.0f * deltaTime;
                    velocity = speed;
                }
            }

            position += front * (deltaTime * FreeCameraControllerSystem::speed);
            // A & D moves the player left or right
            if (app->getKeyboard().isPressed(GLFW_KEY_A) || app->getKeyboard().isPressed(GLFW_KEY_LEFT))
            {
                // check if the player is not going out of the road
                if (position.x > -8.0f)
                    position -= right * (deltaTime * current_sensitivity.x);
                // position -= right * (deltaTime * current_sensitivity.x);
                else
                {
                    // first decrease the speed linearly with time
                    if (FreeCameraControllerSystem::speed > 0.0f)
                        FreeCameraControllerSystem::speed -= 3.0f * deltaTime;
                }
            }

            if (app->getKeyboard().isPressed(GLFW_KEY_D) || app->getKeyboard().isPressed(GLFW_KEY_RIGHT))
            {
                // check if the player is not going out of the road
                if (position.x < 8.0f)
                    position += right * (deltaTime * current_sensitivity.x);
                else
                {
                    // first decrease the speed linearly with time
                    if (FreeCameraControllerSystem::speed > 0.0f)
                        FreeCameraControllerSystem::speed -= 3.0f * deltaTime;
                }
                // position += right * (deltaTime * current_sensitivity.x);
            }
        }

        // When the state exits, it should call this function to ensure the mouse is unlocked
        void exit()
        {
            if (mouse_locked)
            {
                mouse_locked = false;
                app->getMouse().unlockMouse(app->getWindow());
            }
            score = 0;
            app->score = 0;
            speed = 0;
            time=0;
            prev_time = 0;
        }
    };

}
