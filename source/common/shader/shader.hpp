#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our {

    class ShaderProgram {

    private:
        //Shader Program Handle (OpenGL object name)
        GLuint program;

    public:
        ShaderProgram(){
            //TODO: (Req 1) Create A shader program
            program = glCreateProgram();
        }
        ~ShaderProgram(){
            //TODO: (Req 1) Delete a shader program
            glDeleteProgram(program);
        }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use() { 
            glUseProgram(program);
        }

        GLuint getUniformLocation(const std::string &name) {
            //TODO: (Req 1) Return the location of the uniform with the given name
            return glGetUniformLocation(program, name.c_str());
        }

        void set(const std::string &uniform, GLfloat value) {
            //TODO: (Req 1) Send the given float value to the given uniform
             GLint uniform_loc = glGetUniformLocation(program, uniform.c_str());
             glUniform1f(uniform_loc, value);
        }

        void set(const std::string &uniform, GLuint value) {
            //TODO: (Req 1) Send the given unsigned integer value to the given uniform
            GLint uniform_loc = glGetUniformLocation(program, uniform.c_str());
            glUniform1ui(uniform_loc, value);

        }

        void set(const std::string &uniform, GLint value) {
            //TODO: (Req 1) Send the given integer value to the given uniform
            GLint uniform_loc = glGetUniformLocation(program, uniform.c_str());
            glUniform1i(uniform_loc, value);
        }

        void set(const std::string &uniform, glm::vec2 value) {
            //TODO: (Req 1) Send the given 2D vector value to the given uniform
            GLint uniform_loc = glGetUniformLocation(program, uniform.c_str());
            glUniform2f(uniform_loc, value.x, value.y);
        }

        void set(const std::string &uniform, glm::vec3 value) {
            //TODO: (Req 1) Send the given 3D vector value to the given uniform
            GLint uniform_loc = glGetUniformLocation(program, uniform.c_str());
            glUniform3f(uniform_loc, value.x, value.y, value.z);
        }

        void set(const std::string &uniform, glm::vec4 value) {
            //TODO: (Req 1) Send the given 4D vector value to the given uniform
            GLint uniform_loc = glGetUniformLocation(program, uniform.c_str());
            glUniform4f(uniform_loc, value.x, value.y, value.z, value.w);
        }

        void set(const std::string &uniform, glm::mat4 matrix) {
            //TODO: (Req 1) Send the given matrix 4x4 value to the given uniform
            GLint uniform_loc = glGetUniformLocation(program, uniform.c_str());
            // glUniformMatrix4fv(uniform_loc, 1, GL_FALSE, glm::value_ptr(matrix));
            glUniformMatrix4fv(uniform_loc, 1, GL_FALSE, &matrix[0][0]);

        }

        //TODO: (Req 1) Delete the copy constructor and assignment operator.
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;
        //Question: Why do we delete the copy constructor and assignment operator?
        //Answer: Because we don't want to copy the shader program. We want to use the same shader program for all the objects.
    };

}

#endif