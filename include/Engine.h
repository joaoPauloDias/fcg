#pragma once

#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "Camera.h"
#include "VirtualScene.h"
#include <functional>

enum class GameState {
    THESEU,
    DAEDALUS,
    GAME_OVER
};

namespace engine
{   
    GLFWwindow* Init();
    void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod);
    void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    void ErrorCallback(int error, const char* description);
    void Run(GLFWwindow *window, std::function<VirtualScene*()> getActiveScene);
    void SetActiveScene(VirtualScene* scene);
} // namespace engine


