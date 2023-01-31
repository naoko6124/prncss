#pragma once
#include "pch.h"

namespace prncss
{
    class mesh
    {
    public:
        struct vertex
        {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 uv;
        };
    public:
        mesh()
        {
            glGenVertexArrays(1, &vertex_array);
        }
        ~mesh()
        {
            glDeleteVertexArrays(1, &vertex_array);
        }
        void set_vertices(std::vector<vertex>& vertices)
        {
            glBindVertexArray(vertex_array);
            unsigned int vertex_buffer;
            glGenBuffers(1, &vertex_buffer);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, position));
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, normal));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, uv));
            glEnableVertexAttribArray(2);
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDeleteBuffers(1, &vertex_buffer);
        }
        void set_indices(std::vector<uint32_t>& indices)
        {
            size = indices.size();
            glBindVertexArray(vertex_array);
            unsigned int index_buffer;
            glGenBuffers(1, &index_buffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
            glBindVertexArray(0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glDeleteBuffers(1, &index_buffer);
        }
        void set_texture(const char* path)
        {
            int width, height, channels;
            unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            if (channels == 4)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            if (channels == 3)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glBindTexture(GL_TEXTURE_2D, 0);
            stbi_image_free(data);
        }
        void draw()
        {
            glBindVertexArray(vertex_array);
            glBindTexture(GL_TEXTURE_2D, texture);
            glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    private:
        unsigned int vertex_array;
        unsigned int texture;
        unsigned int size;
    };
}