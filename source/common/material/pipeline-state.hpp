#pragma once

#include <glad/gl.h>
#include <glm/vec4.hpp>
#include <json/json.hpp>

namespace our
{
    // There are some options in the render pipeline that we cannot control via shaders
    // such as blending, depth testing and so on
    // Since each material could require different options (e.g. transparent materials usually use blending),
    // we will encapsulate all these options into a single structure that will also be responsible for configuring OpenGL's pipeline
    struct PipelineState
    {
        // This set of pipeline options specifies whether face culling will be used or not and how it will be configured
        struct
        {
            bool enabled = false;
            GLenum culledFace = GL_BACK;
            GLenum frontFace = GL_CCW;
        } faceCulling;

        // This set of pipeline options specifies whether depth testing will be used or not and how it will be configured
        struct
        {
            bool enabled = false;
            GLenum function = GL_LEQUAL;
        } depthTesting;

        // This set of pipeline options specifies whether blending will be used or not and how it will be configured
        struct
        {
            bool enabled = false;
            GLenum equation = GL_FUNC_ADD;
            GLenum sourceFactor = GL_SRC_ALPHA;
            GLenum destinationFactor = GL_ONE_MINUS_SRC_ALPHA;
            glm::vec4 constantColor = {0, 0, 0, 0};
        } blending;

        // These options specify the color and depth mask which can be used to
        // prevent the rendering/clearing from modifying certain channels of certain targets in the framebuffer
        glm::bvec4 colorMask = {true, true, true, true}; // To know how to use it, check glColorMask
        bool depthMask = true;                           // To know how to use it, check glDepthMask

        // This function should set the OpenGL options to the values specified by this structure
        // For example, if faceCulling.enabled is true, you should call glEnable(GL_CULL_FACE), otherwise, you should call glDisable(GL_CULL_FACE)
        void setup() const
        {
            // TODO: (Req 4) Write this function
            if (faceCulling.enabled)
            {
                // If face culling is enabled, enable face culling in OpenGL and set the cull face and front face.
                glEnable(GL_CULL_FACE);
                glCullFace(faceCulling.culledFace);
                glFrontFace(faceCulling.frontFace);
            }
            else
            {
                // Otherwise, disable face culling in OpenGL.
                glDisable(GL_CULL_FACE);
            }
            if (depthTesting.enabled)
            {
                // If depth testing is enabled, enable depth testing in OpenGL and set the depth function.
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(depthTesting.function);
            }
            else
            {
                // Otherwise, disable depth testing in OpenGL.
                glDisable(GL_DEPTH_TEST);
            }
            if (blending.enabled)
            {
                // If blending is enabled, enable blending in OpenGL and set the blend equation, blend function and blend color.
                glEnable(GL_BLEND);
                glBlendEquation(blending.equation);
                glBlendFunc(blending.sourceFactor, blending.destinationFactor);
                glBlendColor(blending.constantColor.r, blending.constantColor.g, blending.constantColor.b, blending.constantColor.a);
            }
            else
            {
                // Otherwise, disable blending in OpenGL.
                glDisable(GL_BLEND);
            }
            // Set the color mask and depth mask.
            // Set the color mask based on the PipelineState's configuration
            glColorMask(colorMask.r, colorMask.g, colorMask.b, colorMask.a);
            glDepthMask(depthMask);
        }

        // Given a json object, this function deserializes a PipelineState structure
        void deserialize(const nlohmann::json &data);
    };

}