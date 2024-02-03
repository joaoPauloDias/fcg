
#include "collisions.h"

#include <algorithm>


bool cubeSphereCollision(
    const glm::vec4& center, float radius, 
    const glm::vec3& bbox_min, const glm::vec3& bbox_max, const glm::mat4& modelMatrix
) {
    // Inverse transform the camera position
    glm::vec4 transformedCenter = glm::inverse(modelMatrix) * center;

    // Clamping the sphere's center to the closest point inside the cube
    float clampedX = std::max(bbox_min.x, std::min(transformedCenter.x, bbox_max.x));
    float clampedY = std::max(bbox_min.y, std::min(transformedCenter.y, bbox_max.y));
    float clampedZ = std::max(bbox_min.z, std::min(transformedCenter.z, bbox_max.z));

    // Calculate the distance between the sphere's center and this clamped point
    glm::vec3 closestPointInCube = glm::vec3(clampedX, clampedY, clampedZ);
    glm::vec3 sphereCenter = glm::vec3(transformedCenter);
    float distanceSquared = glm::length(sphereCenter - closestPointInCube);

    // Check if the distance is less than or equal to the radius
    return distanceSquared <= (radius * radius);
}

bool checkCollision(Cylinder cylinder, glm::vec4 point) {
    float minCylinderHeight = cylinder.center.y - cylinder.height/2;
    float maxCylinderHeight = cylinder.center.y + cylinder.height/2;
    float distanceFromAxis = pow(cylinder.center.x - point.x, 2) + pow(cylinder.center.z - point.z, 2);

    return minCylinderHeight <= point.y && point.y <= maxCylinderHeight && distanceFromAxis <= (cylinder.radius * cylinder.radius);
}