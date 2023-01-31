#pragma once
#include "pch.h"

namespace prncss
{
    class shader
    {
    public:
        shader()
        {
            program = glCreateProgram();
        }
        ~shader()
        {
            glDeleteProgram(program);
        }
        void set_vertex(std::string vertex_code)
        {
            unsigned int vertex_shader;
            vertex_shader = glCreateShader(GL_VERTEX_SHADER);
            const char* c_vertex_code = vertex_code.c_str();
            glShaderSource(vertex_shader, 1, &c_vertex_code, 0);
            glCompileShader(vertex_shader);

            glAttachShader(program, vertex_shader);
            glLinkProgram(program);
            glDeleteShader(vertex_shader);
        }
        void set_fragment(std::string fragment_code)
        {
            unsigned int fragment_shader;
            fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
            const char* c_fragment_code = fragment_code.c_str();
            glShaderSource(fragment_shader, 1, &c_fragment_code, 0);
            glCompileShader(fragment_shader);

            glAttachShader(program, fragment_shader);
            glLinkProgram(program);
            glDeleteShader(fragment_shader);
        }
        void use()
        {
            glUseProgram(program);
        }
        void set_mat4(const char* location, glm::mat4 value)
        {
            int loc = glGetUniformLocation(program, location);
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
        }
    private:
        unsigned int program;
    };
}