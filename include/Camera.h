#pragma once
#include <glm/vec4.hpp>

struct LookAtCamera {
    float theta; // Ângulo no plano ZX em relação ao eixo Z
    float phi;   // Ângulo em relação ao eixo Y
    float distance; // Distância da câmera para a origem

    LookAtCamera(float theta, float phi, float distance);
    void handleScroll(double xoffset, double yoffset);
    void handleCursor(double dx, double dy);
    void handleKey(int key, int scancode, int action, int mod);
    void update(float dt);
};


struct FreeCamera {
    float theta;
    float phi;
    float speed;
    glm::vec4 position;
    glm::vec4 view_vector;

    bool wPressed = false;
    bool sPressed = false;
    bool aPressed = false;
    bool dPressed = false;

    FreeCamera(float theta, float phi, glm::vec4 position, float speed=0.5f);
    void update(float dt);
    void handleScroll(double xoffset, double yoffset);
    void handleCursor(double dx, double dy);
    void handleKey(int key, int scancode, int action, int mod);
    glm::vec4 getNewPosition(float dt);
};