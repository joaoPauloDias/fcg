#pragma once

#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"


struct Cylinder {
    float radius;
    float height;
    glm::vec4 center;
};


struct Sphere {
    glm::vec4 center;
    float radius;
};


bool cubeSphereCollision(
    const Sphere& sphere, 
    const glm::vec3& bbox_min, const glm::vec3& bbox_max, const glm::mat4& modelMatrix
);

bool cylinderPointCollision(const Cylinder& cylinder, const  glm::vec4& point);

bool cylinderSphereCollision(const Cylinder& cylinder, const Sphere& sphere);
