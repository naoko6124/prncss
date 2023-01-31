#pragma once
#include "pch.h"

namespace prncss
{
    struct transform
    {
        glm::vec3 position = glm::vec3(0);
        glm::vec3 rotation = glm::vec3(0);
        glm::vec3 scale = glm::vec3(1);

        glm::mat4 get_matrix()
        {
            glm::mat4 m_scale = glm::scale(glm::mat4(1), scale);
            glm::mat4 m_rotation = glm::mat4(1);
            m_rotation = glm::rotate(m_rotation, glm::radians(rotation.x), glm::vec3(1, 0, 0));
            m_rotation = glm::rotate(m_rotation, glm::radians(rotation.y), glm::vec3(0, 1, 0));
            m_rotation = glm::rotate(m_rotation, glm::radians(rotation.z), glm::vec3(0, 0, 1));
            glm::mat4 m_position = glm::translate(glm::mat4(1), position);
            glm::mat4 model = m_position * m_rotation * m_scale;
            return model;
        }
    };
}