
#include "collisions.h"

#include <algorithm>

namespace collisions
{
    // FONTE: https://www.realtimerendering.com/intersections.html
    // FONTE: https://realtimecollisiondetection.net/books/rtcd/
    float SqDistPointAABB(const glm::vec4 &point, const glm::vec4 &bbox_min, const glm::vec4 &bbox_max)
    {
        float sqDist = 0.0f;

        if (point.x < bbox_min.x)
            sqDist += (bbox_min.x - point.x) * (bbox_min.x - point.x);
        if (point.x > bbox_max.x)
            sqDist += (point.x - bbox_max.x) * (point.x - bbox_max.x);

        if (point.y < bbox_min.y)
            sqDist += (bbox_min.y - point.y) * (bbox_min.y - point.y);
        if (point.y > bbox_max.y)
            sqDist += (point.y - bbox_max.y) * (point.y - bbox_max.y);

        if (point.z < bbox_min.z)
            sqDist += (bbox_min.z - point.z) * (bbox_min.z - point.z);
        if (point.z > bbox_max.z)
            sqDist += (point.z - bbox_max.z) * (point.z - bbox_max.z);
            
        return sqDist;
    }

    // FONTE: https://www.realtimerendering.com/intersections.html
    // FONTE: https://realtimecollisiondetection.net/books/rtcd/
    bool checkCollision(const Sphere &sphere,
                        const glm::vec3 &bbox_min, const glm::vec3 &bbox_max, const glm::mat4 &modelMatrix)
    {
        glm::vec4 minPoint = modelMatrix * glm::vec4(bbox_min, 1.0f);
        glm::vec4 maxPoint = modelMatrix * glm::vec4(bbox_max, 1.0f);

        // Compute squared distance between sphere center and AABB
        float sqDist = SqDistPointAABB(sphere.center, minPoint, maxPoint);

        // Sphere and AABB intersect if the (squared) distance
        // between them is less than the (squared) sphere radius
        return sqDist <= sphere.radius * sphere.radius;
    }

    // Computes the square distance between a point p and an AABB b

    bool checkCollision(const Cylinder &cylinder, const glm::vec4 &point)
    {
        float minCylinderHeight = cylinder.center.y - cylinder.height / 2;
        float maxCylinderHeight = cylinder.center.y + cylinder.height / 2;
        float distanceFromAxis = pow(cylinder.center.x - point.x, 2) + pow(cylinder.center.z - point.z, 2);

        return minCylinderHeight <= point.y && point.y <= maxCylinderHeight && distanceFromAxis <= (cylinder.radius * cylinder.radius);
    }

    float distance(glm::vec4 a, glm::vec4 b)
    {
        return pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2);
    }

    // FONTE: https://www.realtimerendering.com/intersections.html
    // FONTE: https://realtimecollisiondetection.net/books/rtcd/
    bool checkCollision(const Cylinder &cylinder, const Sphere &sphere)
    {

        float minCylinderHeight = cylinder.center.y - cylinder.height / 2;
        float maxCylinderHeight = cylinder.center.y + cylinder.height / 2;
        float distanceFromAxis = pow(cylinder.center.x - sphere.center.x, 2) + pow(cylinder.center.z - sphere.center.z, 2);
        float distanceSquared;

        if (sphere.center.y >= minCylinderHeight && sphere.center.y <= maxCylinderHeight)
        {
            distanceSquared = distanceFromAxis;
        }
        else if (sphere.center.y < minCylinderHeight)
        {
            distanceSquared = distance(sphere.center, (cylinder.center - glm::vec4{0, cylinder.height / 2, 0, 0}));
        }
        else
        {
            distanceSquared = distance(sphere.center, (cylinder.center + glm::vec4{0, cylinder.height / 2, 0, 0}));
        }

        float radiusSum = sphere.radius + cylinder.radius;
        return distanceSquared <= (radiusSum * radiusSum);
    }
}
