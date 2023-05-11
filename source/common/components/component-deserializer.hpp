#pragma once

#include "../ecs/entity.hpp"
#include "camera.hpp"
#include "mesh-renderer.hpp"
#include "free-camera-controller.hpp"
#include "movement.hpp"
#include "rerender.hpp"
#include "randrender.hpp"
#include "collision.hpp"

namespace our
{

    // Given a json object, this function picks and creates a component in the given entity
    // based on the "type" specified in the json object which is later deserialized from the rest of the json object
    inline void deserializeComponent(const nlohmann::json &data, Entity *entity)
    {
        std::string type = data.value("type", "");
        Component *component = nullptr;
        // TODO: (Req 8) Add an option to deserialize a "MeshRendererComponent" to the following if-else statement
        //  Check the "type" field against different component types

        if (type == CameraComponent::getID())
        {
            // If it matches CameraComponent, create a new CameraComponent and set the pointer to it
            component = entity->addComponent<CameraComponent>();
        }
        else if (type == FreeCameraControllerComponent::getID())
        {
            // If it matches FreeCameraControllerComponent, create a new FreeCameraControllerComponent and set the pointer to it
            component = entity->addComponent<FreeCameraControllerComponent>();
        }
        else if (type == MovementComponent::getID())
        {
            // If it matches MovementComponent, create a new MovementComponent and set the pointer to it
            component = entity->addComponent<MovementComponent>();
        }
        else if (type == MeshRendererComponent::getID())
        {
            // If it matches MeshRendererComponent, create a new MeshRendererComponent and set the pointer to it
            component = entity->addComponent<MeshRendererComponent>();
        }
        else if (type == RerenderComponent::getID())
        {
            // If it matches MeshRendererComponent, create a new MeshRendererComponent and set the pointer to it
            component = entity->addComponent<RerenderComponent>();
        }
        else if (type == RandRenderComponent::getID())
        {
            // If it matches MeshRendererComponent, create a new MeshRendererComponent and set the pointer to it
            component = entity->addComponent<RandRenderComponent>();
        }
        else if (type == collisionComponent::getID())
        {
            // If it matches MeshRendererComponent, create a new MeshRendererComponent and set the pointer to it
            component = entity->addComponent<collisionComponent>();
        }

        // If a component has been created, deserialize the rest of the input json object into it
        if (component)
            component->deserialize(data);
    }

}