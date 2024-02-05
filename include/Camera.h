#pragma once
#include <glm/vec4.hpp>

struct Camera
{
    float theta; // Ângulo no plano ZX em relação ao eixo Z
    float phi;   // Ângulo em relação ao eixo Y
    glm::vec4 position;
    glm::vec4 view_vector;
    glm::vec4 u;
    glm::vec4 w;

    virtual ~Camera() {}

    virtual void handleScroll(double xoffset, double yoffset) = 0;
    virtual void handleCursor(double dx, double dy) = 0;
    virtual void handleKey(int key, int scancode, int action, int mod) = 0;
    virtual void update(float dt) = 0;
};

struct LookAtCamera : public Camera
{
    float distance; // Distância da câmera para a origem
    LookAtCamera(float theta, float phi, float distance);
    void handleScroll(double xoffset, double yoffset) override;
    void handleCursor(double dx, double dy) override;
    void handleKey(int key, int scancode, int action, int mod) override;
    void update(float dt) override;
};

struct FreeCamera : public Camera
{
    float speed;
    FreeCamera(float theta, float phi, glm::vec4 position, float speed = 0.5f);
    glm::vec4 getNewPosition(float dt, bool update_x = true, bool update_y = true, bool update_z = true);
    void handleScroll(double xoffset, double yoffset) override;
    void handleCursor(double dx, double dy) override;
    void handleKey(int key, int scancode, int action, int mod) override;
    void update(float dt) override;
};