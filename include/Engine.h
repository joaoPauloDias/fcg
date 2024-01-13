#pragma once

#include "GLFW/glfw3.h"
#include "Camera.h"

enum class GameState {
    THESEU,
    DAEDALUS,
    GAME_OVER
};

namespace engine
{
    class Engine
    {
    private:
        GameState state;
        FreeCamera camera;
        bool g_LeftMouseButtonPressed;
        bool g_RightMouseButtonPressed; // Análogo para botão direito do mouse
        bool g_MiddleMouseButtonPressed;
        bool g_ShowInfoText;
    public:
        Engine();
        void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
        void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod);
        void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
        void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
        void Run();



    };
} // namespace engine


