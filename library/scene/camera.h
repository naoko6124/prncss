#pragma once
#include "glm/gtc/quaternion.hpp"
#include "pch.h"
#include "scene/transform.h"

namespace prncss
{
    struct camera
    {
        float fov = 45.0f;
        float aspect = 16.0f/9.0f;
        float near = 0.1f;
        float far = 100.0f;

        glm::vec3 right = glm::vec3(1, 0, 0);
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 front = glm::vec3(0, 0, 1);

        glm::mat4 get_matrix(transform* t)
        {
            glm::vec3 direction;
            direction.x = cos(glm::radians(t->rotation.y - 90.0f)) * cos(glm::radians(t->rotation.x));
            direction.y = sin(glm::radians(t->rotation.x));
            direction.z = sin(glm::radians(t->rotation.y - 90.0f)) * cos(glm::radians(t->rotation.x));
            front = glm::normalize(direction);
            right = glm::normalize(glm::cross(front, up));
            glm::mat4 view = glm::lookAt(t->position, t->position + front, up);
            glm::mat4 proj = glm::perspective(glm::radians(fov), aspect, near, far);
            glm::mat4 result = proj * view;
            return result;
        }
    };
}