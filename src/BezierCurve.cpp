#include "BezierCurve.h"

BezierCurve::BezierCurve(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3, glm::vec4 p4) {
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->p4 = p4;
}

glm::vec4 convexCombination(glm::vec4 p1, glm::vec4 p2, float t) {
    return (1 - t) * p1 + t * p2;
}

glm::vec4 BezierCurve::getPosition(float t) {
    glm::vec4 p12 = convexCombination(p1, p2, t);
    glm::vec4 p23 = convexCombination(p2, p3, t);
    glm::vec4 p34 = convexCombination(p3, p4, t);

    glm::vec4 p123 = convexCombination(p12, p23, t);
    glm::vec4 p234 = convexCombination(p23, p34, t);

    return convexCombination(p123, p234, t);
}