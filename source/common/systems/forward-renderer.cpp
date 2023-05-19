#include "forward-renderer.hpp"
#include "../mesh/mesh-utils.hpp"
#include "../texture/texture-utils.hpp"

namespace our
{

    void ForwardRenderer::initialize(glm::ivec2 windowSize, const nlohmann::json &config)
    {
        // First, we store the window size for later use
        this->windowSize = windowSize;

        // Then we check if there is a sky texture in the configuration
        if (config.contains("sky"))
        {
            // First, we create a sphere which will be used to draw the sky
            this->skySphere = mesh_utils::sphere(glm::ivec2(16, 16));

            // We can draw the sky using the same shader used to draw textured objects
            ShaderProgram *skyShader = new ShaderProgram();
            skyShader->attach("assets/shaders/textured.vert", GL_VERTEX_SHADER);
            skyShader->attach("assets/shaders/textured.frag", GL_FRAGMENT_SHADER);
            skyShader->link();

            // TODO: (Req 10) Pick the correct pipeline state to draw the sky
            //  Hints: the sky will be draw after the opaque objects so we would need depth testing but which depth funtion should we pick?

            //  We will draw the sphere from the inside, so what options should we pick for the face culling
            PipelineState skyPipelineState{}; // Here we create a pipeline state object to store the pipeline state

            // this two lines to enable depth testing and set the depth function to GL_LEQUAL (the sky will be drawn after the opaque objects)
            skyPipelineState.depthTesting.enabled = true;
            skyPipelineState.depthTesting.function = GL_LEQUAL;
            // here we enable face culling and set the culled face to GL_FRONT (the sphere will be drawn from the inside)
            skyPipelineState.faceCulling.enabled = true;
            skyPipelineState.faceCulling.culledFace = GL_FRONT;

            // Load the sky texture (note that we don't need mipmaps since we want to avoid any unnecessary blurring while rendering the sky)
            std::string skyTextureFile = config.value<std::string>("sky", "");
            Texture2D *skyTexture = texture_utils::loadImage(skyTextureFile, false);

            // Setup a sampler for the sky
            Sampler *skySampler = new Sampler();
            skySampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            skySampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            skySampler->set(GL_TEXTURE_WRAP_S, GL_REPEAT);
            skySampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Combine all the aforementioned objects (except the mesh) into a material
            this->skyMaterial = new TexturedMaterial();
            this->skyMaterial->shader = skyShader;
            this->skyMaterial->texture = skyTexture;
            this->skyMaterial->sampler = skySampler;
            this->skyMaterial->pipelineState = skyPipelineState;
            this->skyMaterial->tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            this->skyMaterial->alphaThreshold = 1.0f;
            this->skyMaterial->transparent = false;
        }

        // Then we check if there is a postprocessing shader in the configuration
        if (config.contains("postprocess"))
        {
            // TODO: (Req 11) Create a framebuffer

            // here we create a framebuffer object and bind it to GL_DRAW_FRAMEBUFFER
            glGenFramebuffers(1, &postprocessFrameBuffer);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postprocessFrameBuffer);

            // TODO: (Req 11) Create a color and a depth texture and attach them to the framebuffer
            //  Hints: The color format can be (Red, Green, Blue and Alpha components with 8 bits for each channel).
            //  The depth format can be (Depth component with 24 bits).

            // here we create a color texture and a depth texture and attach them to the framebuffer
            colorTarget = texture_utils::empty(GL_RGBA8, windowSize);
            depthTarget = texture_utils::empty(GL_DEPTH_COMPONENT24, windowSize);
            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTarget->getOpenGLName(), 0);
            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTarget->getOpenGLName(), 0);

            // TODO: (Req 11) Unbind the framebuffer just to be safe

            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

            // Create a vertex array to use for drawing the texture
            glGenVertexArrays(1, &postProcessVertexArray);

            // Create a sampler to use for sampling the scene texture in the post processing shader
            Sampler *postprocessSampler = new Sampler();
            postprocessSampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            postprocessSampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            postprocessSampler->set(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            postprocessSampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Create the post processing shader
            ShaderProgram *postprocessShader = new ShaderProgram();
            postprocessShader->attach("assets/shaders/fullscreen.vert", GL_VERTEX_SHADER);
            postprocessShader->attach(config.value<std::string>("postprocess", ""), GL_FRAGMENT_SHADER);
            postprocessShader->link();

            // Create a post processing material
            postprocessMaterial = new TexturedMaterial();
            postprocessMaterial->shader = postprocessShader;
            postprocessMaterial->texture = colorTarget;
            postprocessMaterial->sampler = postprocessSampler;
            // The default options are fine but we don't need to interact with the depth buffer
            // so it is more performant to disable the depth mask
            postprocessMaterial->pipelineState.depthMask = false;
        }
    }

    void ForwardRenderer::destroy()
    {
        // Delete all objects related to the sky
        if (skyMaterial)
        {
            delete skySphere;
            delete skyMaterial->shader;
            delete skyMaterial->texture;
            delete skyMaterial->sampler;
            delete skyMaterial;
        }
        // Delete all objects related to post processing
        if (postprocessMaterial)
        {
            glDeleteFramebuffers(1, &postprocessFrameBuffer);
            glDeleteVertexArrays(1, &postProcessVertexArray);
            delete colorTarget;
            delete depthTarget;
            delete postprocessMaterial->sampler;
            delete postprocessMaterial->shader;
            delete postprocessMaterial;
        }
    }

    void ForwardRenderer::render(World *world)
    {
        // First of all, we search for a camera and for all the mesh renderers
        CameraComponent *camera = nullptr;
        opaqueCommands.clear();
        transparentCommands.clear();
        lights.clear();
        for (auto entity : world->getEntities())
        {
            // If we hadn't found a camera yet, we look for a camera in this entity
            if (!camera)
                camera = entity->getComponent<CameraComponent>();
            // If this entity has a mesh renderer component
            if (auto meshRenderer = entity->getComponent<MeshRendererComponent>(); meshRenderer)
            {
                // We construct a command from it
                RenderCommand command;
                command.localToWorld = meshRenderer->getOwner()->getLocalToWorldMatrix();
                command.center = glm::vec3(command.localToWorld * glm::vec4(0, 0, 0, 1));
                command.mesh = meshRenderer->mesh;
                command.material = meshRenderer->material;
                // if it is transparent, we add it to the transparent commands list
                if (command.material->transparent)
                {
                    transparentCommands.push_back(command);
                }
                else
                {
                    // Otherwise, we add it to the opaque command list
                    opaqueCommands.push_back(command);
                }
            }
            if (auto light = entity->getComponent<Lightning_Component>(); light)
            {
                lights.push_back(light);
            }
        }

        // If there is no camera, we return (we cannot render without a camera)
        if (camera == nullptr)
            return;

        // TODO: (Req 9) Modify the following line such that "cameraForward" contains a vector pointing the camera forward direction
        //  HINT: See how you wrote the CameraComponent::getViewMatrix, it should help you solve this one
        // glm::vec3 cameraForward = glm::vec3(0.0, 0.0, -1.0f);
        glm::vec3 eye = camera->getOwner()->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1.0);
        glm::vec3 center = camera->getOwner()->getLocalToWorldMatrix() * glm::vec4(0, 0, -1, 1.0);
        glm::vec3 cameraForward = -glm::vec3(camera->getViewMatrix()[2]);
        auto owner = camera->getOwner();

        // here we are getting the camera forward vector from the camera's view matrix
        // by normalizing the center of the camera -position where the camera is looking at-
        // to the eye of the camera -position of the camera-
        // glm::vec3 cameraForward = glm::normalize(center - eye);
        std::sort(transparentCommands.begin(), transparentCommands.end(), [cameraForward](const RenderCommand &first, const RenderCommand &second)
                  {
            //TODO: (Req 9) Finish this function
            // HINT: the following return should return true "first" should be drawn before "second". 
            
            bool checkDraw = glm::dot(cameraForward,first.center) > glm::dot(cameraForward , second.center) ? true : false; 
            return checkDraw; });

        // TODO: (Req 9) Get the camera ViewProjection matrix and store it in VP

        // here we are getting the camera view projection matrix by multiplying
        // the camera's projection matrix by the camera's view matrix
        glm::mat4 VP = camera->getProjectionMatrix(windowSize) * camera->getViewMatrix();

        // TODO: (Req 9) Set the OpenGL viewport using viewportStart and viewportSize
        glViewport(0, 0, windowSize.x, windowSize.y); // here we are setting the viewport to the window size

        // TODO: (Req 9) Set the clear color to black and the clear depth to 1

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearDepth(1.0);

        // TODO: (Req 9) Set the color mask to true and the depth mask to true (to ensure the glClear will affect the framebuffer)

        glColorMask(true, true, true, true);
        glDepthMask(true);

        // If there is a postprocess material, bind the framebuffer
        if (postprocessMaterial)
        {
            // TODO: (Req 11) bind the framebuffer
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postprocessFrameBuffer); // here we are binding the framebuffer
        }

        // TODO: (Req 9) Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // TODO: (Req 9) Draw all the opaque commands
        //  Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command
        // this loop to draw all the opaque commands by setting the transform uniform to be equal to the model-view-projection matrix and then drawing the mesh
        for (auto &command : opaqueCommands)
        {
            command.material->setup();

            // here we want to do downcasting to check if the material is a holding light material inside or not
            if (auto lightMaterial = dynamic_cast<LightMaterial *>(command.material); lightMaterial)
            {
                lightMaterial->shader->set("VP", VP);
                lightMaterial->shader->set("eye", eye);
                lightMaterial->shader->set("M", command.localToWorld);
                lightMaterial->shader->set("M_IT", glm::transpose(glm::inverse(command.localToWorld)));

                // if it is a light material then we want to set the
                // light material's light position to the camera's position
                lightMaterial->shader->set("light_count", (int)lights.size());

                // lightMaterial->shader->set("sky.top", glm::vec3(0.7, 0.3, 0.8));
                // lightMaterial->shader->set("sky.middle", glm::vec3(0.7, 0.3, 0.8));
                // lightMaterial->shader->set("sky.bottom", glm::vec3(0.7, 0.3, 0.8));
                for (unsigned i = 0; i < lights.size(); i++)
                {
                    glm::vec3 light_position = lights[i]->getOwner()->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1);
                    glm::vec3 light_direction = lights[i]->getOwner()->getLocalToWorldMatrix() * glm::vec4(lights[i]->direction, 0);

                    std::string light_name = "lights[" + std::to_string(i) + "]";

                    lightMaterial->shader->set(light_name + ".LightType", (GLint)lights[i]->light_type);
                    lightMaterial->shader->set(light_name + ".diffuse", lights[i]->diffuse);
                    lightMaterial->shader->set(light_name + ".specular", lights[i]->specular);
                    lightMaterial->shader->set(light_name + ".attenuation", lights[i]->attenuation);

                    switch (lights[i]->light_type)
                    {
                    case LightType::DirectionalLight:
                        lightMaterial->shader->set(light_name + ".direction", light_direction);
                        break;
                    case LightType::SpotLight:
                        lightMaterial->shader->set(light_name + ".position", light_position);
                        lightMaterial->shader->set(light_name + ".direction", light_direction);
                        lightMaterial->shader->set(light_name + ".cone_angles", lights[i]->cone_angles);
                        break;
                    case LightType::PointLight:
                        lightMaterial->shader->set(light_name + ".position", light_position);

                        break;
                    }
                }
            }
            else
            {
                command.material->shader->set("transform", VP * command.localToWorld);
            }

            // else{
            //     command.material->shader->set("transform", VP * command.localToWorld);
            // }
            command.mesh->draw();
        }

        // If there is a sky material, draw the sky
        if (this->skyMaterial)
        {
            // TODO: (Req 10) setup the sky material
            skyMaterial->setup();

            // TODO: (Req 10) Get the camera position
            // glm::vec3 cameraPosition = camera->getOwner()->localTransform.position;
            glm::vec3 cameraPosition = camera->getOwner()->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1.0);

            // TODO: (Req 10) Create a model matrix for the sy such that it always follows the camera (sky sphere center = camera position)
            Transform skyModelMat;                 // here we are creating a model matrix for the sky sphere
            skyModelMat.position = cameraPosition; // setting the sky sphere position to the camera position

            // TODO: (Req 10) We want the sky to be drawn behind everything (in NDC space, z=1)
            //  We can acheive the is by multiplying by an extra matrix after the projection but what values should we put in it?
            glm::mat4 alwaysBehindTransform = glm::mat4(
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 1.0f);
            // TODO: (Req 10) set the "transform" uniform
            glm::mat4 combineMat = alwaysBehindTransform * VP * skyModelMat.toMat4(); // here we are get the transform matrix
            skyMaterial->shader->set("transform", combineMat);                        // here we are setting the transform uniform to the transform matrix
            // TODO: (Req 10) draw the sky sphere
            // at last we are drawing the sky sphere
            skySphere->draw();
        }
        // TODO: (Req 9) Draw all the transparent commands
        //  Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command
        // and this loop to draw all the transparent commands by setting the transform uniform to be equal to the model-view-projection matrix and then drawing the mesh
        for (auto &command : transparentCommands)
        {
            command.material->setup();
            if (auto lightMaterial = dynamic_cast<LightMaterial *>(command.material); lightMaterial)
            {
                lightMaterial->shader->set("VP", VP);
                lightMaterial->shader->set("eye", eye);
                lightMaterial->shader->set("M", command.localToWorld);
                lightMaterial->shader->set("M_IT", glm::transpose(glm::inverse(command.localToWorld)));

                // if it is a light material then we want to set the
                // light material's light position to the camera's position
                lightMaterial->shader->set("light_count", (int)lights.size());

                lightMaterial->shader->set("Sky.top", glm::vec3(0.7, 0.3, 0.8));
                lightMaterial->shader->set("Sky.middle", glm::vec3(0.7, 0.3, 0.8));
                lightMaterial->shader->set("Sky.bottom", glm::vec3(0.7, 0.3, 0.8));
                for (unsigned i = 0; i < lights.size(); i++)
                {
                    glm::vec3 light_position = lights[i]->getOwner()->getLocalToWorldMatrix() * glm::vec4(0, 0, 0, 1);
                    glm::vec3 light_direction = lights[i]->getOwner()->getLocalToWorldMatrix() * glm::vec4(lights[i]->direction, 0);

                    std::string light_name = "lights[" + std::to_string(i) + "]";

                    lightMaterial->shader->set(light_name + ".LightType", (GLint)lights[i]->light_type);
                    lightMaterial->shader->set(light_name + ".diffuse", lights[i]->diffuse);
                    lightMaterial->shader->set(light_name + ".specular", lights[i]->specular);
                    lightMaterial->shader->set(light_name + ".attenuation", lights[i]->attenuation);

                    switch (lights[i]->light_type)
                    {
                    case LightType::DirectionalLight:
                        lightMaterial->shader->set(light_name + ".direction", light_direction);
                        break;
                    case LightType::SpotLight:
                        lightMaterial->shader->set(light_name + ".position", light_position);
                        lightMaterial->shader->set(light_name + ".direction", light_direction);
                        lightMaterial->shader->set(light_name + ".cone_angles", lights[i]->cone_angles);
                        break;
                    case LightType::PointLight:
                        lightMaterial->shader->set(light_name + ".position", light_position);

                        break;
                    }
                }
            }
            else
            {
                command.material->shader->set("transform", VP * command.localToWorld);
            }
            // command.material->shader->set("transform", VP * command.localToWorld);
            command.mesh->draw();
        }

        // If there is a postprocess material, apply postprocessing
        if (postprocessMaterial)
        {
            // TODO: (Req 11) Return to the default framebuffer
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

            // TODO: (Req 11) Setup the postprocess material and draw the fullscreen triangle
            postprocessMaterial->setup();
            glBindVertexArray(postProcessVertexArray);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
    }

}