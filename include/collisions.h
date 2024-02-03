#pragma once

#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"


struct Cylinder {
    float radius;
    float height;
    glm::vec4 center;
};


bool cubeSphereCollision(
    const glm::vec4& center, float radius, 
    const glm::vec3& bbox_min, const glm::vec3& bbox_max, const glm::mat4& modelMatrix
);

bool checkCollision(Cylinder cylinder, glm::vec4 point);