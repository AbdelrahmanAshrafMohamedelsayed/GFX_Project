#pragma once

#include <glad/gl.h>

namespace our {

    // This class defined an OpenGL texture which will be used as a GL_TEXTURE_2D
    class Texture2D {
        // The OpenGL object name of this texture 
        GLuint name = 0;
    public:
        // This constructor creates an OpenGL texture and saves its object name in the member variable "name" 
        Texture2D() {
            //TODO: (Req 5) Complete this function
            // generate one texture which will be stored in name
            glGenTextures(1, &name); // generate a texture object name
        };

        // This deconstructor deletes the underlying OpenGL texture
        ~Texture2D() { 
            //TODO: (Req 5) Complete this function
            glDeleteTextures(1, &name); // delete the texture object
        }

        // Get the internal OpenGL name of the texture which is useful for use with framebuffers
        GLuint getOpenGLName() {
            return name;
        }

        // This method binds this texture to GL_TEXTURE_2D
        void bind() const {
            //TODO: (Req 5) Complete this function
            // bind the texture to the texture unit
            // by calling binding function which take type of texture which is 2d texture
            // and the texture to bind it
            glBindTexture(GL_TEXTURE_2D, name);
        }

        // This static method ensures that no texture is bound to GL_TEXTURE_2D
        static void unbind(){
            //TODO: (Req 5) Complete this function
            // unbind texture by passing 0 so it is not binded to any texture now
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        Texture2D(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;
    };
    
}