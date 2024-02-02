#include "glm/vec4.hpp"
#include <vector>

class BezierCurve {
    private:
        glm::vec4 p1;
        glm::vec4 p2;
        glm::vec4 p3;
        glm::vec4 p4;
    
    public:
        BezierCurve(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3, glm::vec4 p4);
        glm::vec4 getPosition(float t);
};
