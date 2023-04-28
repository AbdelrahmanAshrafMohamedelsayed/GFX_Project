#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our
{

    class ShaderProgram
    {

    private:
        // Shader Program Handle (OpenGL object name)
        GLuint program;

    public:
        ShaderProgram()
        {
            // TODO: (Req 1) Create A shader program
            //  we create a shader program by calling glCreateProgram() and storing the resulting id in program of type GLuint
            program = glCreateProgram();
        }
        ~ShaderProgram()
        {
            // TODO: (Req 1) Delete a shader program
            //  we delete a shader program by calling glDeleteProgram() and passing the id of the program we want to delete
            glDeleteProgram(program);
        }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use()
        {
            glUseProgram(program);
        }

        GLuint getUniformLocation(const std::string &name)
        {
            // TODO: (Req 1) Return the location of the uniform with the given name
            //  we get the location of a uniform by calling glGetUniformLocation() and passing the id of the program and the name of the uniform
            return glGetUniformLocation(program, name.c_str());
        }

        void set(const std::string &uniform, GLfloat value)
        {
            // TODO: (Req 1) Send the given float value to the given uniform
            //  we get the location of a uniform by calling glGetUniformLocation() and passing the id of the program and the name of the uniform
            GLint uniform_loc = glGetUniformLocation(program, uniform.c_str());
            // we set the value of a uniform by calling glUniform1f() and passing the location of the uniform and the value we want to set 1f means  float
            glUniform1f(uniform_loc, value);
        }

        void set(const std::string &uniform, GLuint value)
        {
            // TODO: (Req 1) Send the given unsigned integer value to the given uniform
            //  we get the location of a uniform by calling glGetUniformLocation() and passing the id of the program and the name of the uniform
            GLint uniform_loc = glGetUniformLocation(program, uniform.c_str());
            // we set the value of a uniform by calling glUniform1ui() and passing the location of the uniform and the value we want to set 1ui means  unsigned integer
            glUniform1ui(uniform_loc, value);
        }

        void set(const std::string &uniform, GLint value)
        {
            // TODO: (Req 1) Send the given integer value to the given uniform
            //  we get the location of a uniform by calling glGetUniformLocation() and passing the id of the program and the name of the uniform
            GLint uniform_loc = glGetUniformLocation(program, uniform.c_str());
            // we set the value of a uniform by calling glUniform1i() and passing the location of the uniform and the value we want to set 1i means  integer
            glUniform1i(uniform_loc, value);
        }

        void set(const std::string &uniform, glm::vec2 value)
        {
            // TODO: (Req 1) Send the given 2D vector value to the given uniform
            //  we get the location of a uniform by calling glGetUniformLocation() and passing the id of the program and the name of the uniform
            GLint uniform_loc = glGetUniformLocation(program, uniform.c_str());
            // we set the value of a uniform by calling glUniform2f() and passing the location of the uniform and the value we want to set 2f means  2D vector
            glUniform2f(uniform_loc, value.x, value.y);
        }

        void set(const std::string &uniform, glm::vec3 value)
        {
            // TODO: (Req 1) Send the given 3D vector value to the given uniform
            //  we get the location of a uniform by calling glGetUniformLocation() and passing the id of the program and the name of the uniform
            GLint uniform_loc = glGetUniformLocation(program, uniform.c_str());
            // we set the value of a uniform by calling glUniform3f() and passing the location of the uniform and the value we want to set 3f means  3D vector
            glUniform3f(uniform_loc, value.x, value.y, value.z);
        }

        void set(const std::string &uniform, glm::vec4 value)
        {
            // TODO: (Req 1) Send the given 4D vector value to the given uniform
            //  we get the location of a uniform by calling glGetUniformLocation() and passing the id of the program and the name of the uniform
            GLint uniform_loc = glGetUniformLocation(program, uniform.c_str());
            // we set the value of a uniform by calling glUniform4f() and passing the location of the uniform and the value we want to set 4f means  4D vector
            glUniform4f(uniform_loc, value.x, value.y, value.z, value.w);
        }

        void set(const std::string &uniform, glm::mat4 matrix)
        {
            // TODO: (Req 1) Send the given matrix 4x4 value to the given uniform
            //  we get the location of a uniform by calling glGetUniformLocation() and passing the id of the program and the name of the uniform
            GLint uniform_loc = glGetUniformLocation(program, uniform.c_str());
            // glUniformMatrix4fv(uniform_loc, 1, GL_FALSE, glm::value_ptr(matrix));
            // we set the value of a uniform by calling glUniformMatrix4fv() and passing the location of the uniform and the value we want to set 4fv means  4x4 matrix
            // we pass the matrix as a pointer to the first element of the matrix
            // we pass 1 because we want to set only one matrix
            // we pass GL_FALSE because we don't want to transpose the matrix
            glUniformMatrix4fv(uniform_loc, 1, GL_FALSE, &matrix[0][0]);
        }

        // TODO: (Req 1) Delete the copy constructor and assignment operator.
        //  we delete the copy constructor and assignment operator because we don't want to copy the shader program. We want to use the same shader program for all the objects.
        //  we delete the copy constructor and assignment operator by assigning them to delete
        ShaderProgram(const ShaderProgram &) = delete;
        ShaderProgram &operator=(const ShaderProgram &) = delete;
        // Question: Why do we delete the copy constructor and assignment operator?
        // Answer: Because we don't want to copy the shader program. We want to use the same shader program for all the objects.
    };

}

#endif