#include "Engine.h"
#include <cstdio>

extern void LoadShadersFromFiles();
extern float g_ScreenRatio;

namespace engine{
    Engine::Engine():camera(3.14f, -0.7f, glm::vec4(0.0f, 2.0f, 3.0f, 1.0f), 1.0f){
        // TODO Initialization
        g_LeftMouseButtonPressed = false;
        g_RightMouseButtonPressed = false; // Análogo para botão direito do mouse
        g_MiddleMouseButtonPressed = false;
        g_ShowInfoText = false;

    }

        
    // Definição da função que será chamada sempre que a janela do sistema
    // operacional for redimensionada, por consequência alterando o tamanho do
    // "framebuffer" (região de memória onde são armazenados os pixels da imagem).
    void Engine::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        // Indicamos que queremos renderizar em toda região do framebuffer. A
        // função "glViewport" define o mapeamento das "normalized device
        // coordinates" (NDC) para "pixel coordinates".  Essa é a operação de
        // "Screen Mapping" ou "Viewport Mapping" vista em aula ({+ViewportMapping2+}).
        glViewport(0, 0, width, height);

        // Atualizamos também a razão que define a proporção da janela (largura /
        // altura), a qual será utilizada na definição das matrizes de projeção,
        // tal que não ocorra distorções durante o processo de "Screen Mapping"
        // acima, quando NDC é mapeado para coordenadas de pixels. Veja slides 205-215 do documento Aula_09_Projecoes.pdf.
        //
        // O cast para float é necessário pois números inteiros são arredondados ao
        // serem divididos!
        g_ScreenRatio = (float)width / height;
    }

    // Variáveis globais que armazenam a última posição do cursor do mouse, para
    // que possamos calcular quanto que o mouse se movimentou entre dois instantes
    // de tempo. Utilizadas no callback CursorPosCallback() abaixo.
    double g_LastCursorPosX, g_LastCursorPosY;

    // Função callback chamada sempre que o usuário aperta algum dos botões do mouse
    void Engine::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            // Se o usuário pressionou o botão esquerdo do mouse, guardamos a
            // posição atual do cursor nas variáveis g_LastCursorPosX e
            // g_LastCursorPosY.  Também, setamos a variável
            // g_LeftMouseButtonPressed como true, para saber que o usuário está
            // com o botão esquerdo pressionado.
            glfwGetCursorPos(window, &g_LastCursorPosX, &g_LastCursorPosY);
            g_LeftMouseButtonPressed = true;
        }
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        {
            // Quando o usuário soltar o botão esquerdo do mouse, atualizamos a
            // variável abaixo para false.
            g_LeftMouseButtonPressed = false;
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        {
            // Se o usuário pressionou o botão esquerdo do mouse, guardamos a
            // posição atual do cursor nas variáveis g_LastCursorPosX e
            // g_LastCursorPosY.  Também, setamos a variável
            // g_RightMouseButtonPressed como true, para saber que o usuário está
            // com o botão esquerdo pressionado.
            glfwGetCursorPos(window, &g_LastCursorPosX, &g_LastCursorPosY);
            g_RightMouseButtonPressed = true;
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        {
            // Quando o usuário soltar o botão esquerdo do mouse, atualizamos a
            // variável abaixo para false.
            g_RightMouseButtonPressed = false;
        }
        if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
        {
            // Se o usuário pressionou o botão esquerdo do mouse, guardamos a
            // posição atual do cursor nas variáveis g_LastCursorPosX e
            // g_LastCursorPosY.  Também, setamos a variável
            // g_MiddleMouseButtonPressed como true, para saber que o usuário está
            // com o botão esquerdo pressionado.
            glfwGetCursorPos(window, &g_LastCursorPosX, &g_LastCursorPosY);
            g_MiddleMouseButtonPressed = true;
        }
        if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
        {
            // Quando o usuário soltar o botão esquerdo do mouse, atualizamos a
            // variável abaixo para false.
            g_MiddleMouseButtonPressed = false;
        }
    }

    // Função callback chamada sempre que o usuário movimentar o cursor do mouse em
    // cima da janela OpenGL.
    void Engine::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
    {
        // Abaixo executamos o seguinte: caso o botão esquerdo do mouse esteja
        // pressionado, computamos quanto que o mouse se movimento desde o último
        // instante de tempo, e usamos esta movimentação para atualizar os
        // parâmetros que definem a posição da câmera dentro da cena virtual.
        // Assim, temos que o usuário consegue controlar a câmera.

        if (g_LeftMouseButtonPressed)
        {
            // Deslocamento do cursor do mouse em x e y de coordenadas de tela!
            float dx = xpos - g_LastCursorPosX;
            float dy = ypos - g_LastCursorPosY;
        
            camera.handleCursor(dx, dy);
        
            // Atualizamos as variáveis globais para armazenar a posição atual do
            // cursor como sendo a última posição conhecida do cursor.
            g_LastCursorPosX = xpos;
            g_LastCursorPosY = ypos;
        }

        if (g_RightMouseButtonPressed)
        {
            // Atualizamos as variáveis globais para armazenar a posição atual do
            // cursor como sendo a última posição conhecida do cursor.
            g_LastCursorPosX = xpos;
            g_LastCursorPosY = ypos;
        }

        if (g_MiddleMouseButtonPressed)
        {    
            // Atualizamos as variáveis globais para armazenar a posição atual do
            // cursor como sendo a última posição conhecida do cursor.
            g_LastCursorPosX = xpos;
            g_LastCursorPosY = ypos;
        }
    }

    // Função callback chamada sempre que o usuário movimenta a "rodinha" do mouse.
    void Engine::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        camera.handleScroll(xoffset, yoffset);
    }

    // Definição da função que será chamada sempre que o usuário pressionar alguma
    // tecla do teclado. Veja http://www.glfw.org/docs/latest/input_guide.html#input_key
    void Engine::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod)
    {    
        // Se o usuário pressionar a tecla ESC, fechamos a janela.
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);

        camera.handleKey(key, scancode, action, mod);

        // Se o usuário apertar a tecla H, fazemos um "toggle" do texto informativo mostrado na tela.
        if (key == GLFW_KEY_H && action == GLFW_PRESS)
        {
            g_ShowInfoText = !g_ShowInfoText;
        }

        // Se o usuário apertar a tecla R, recarregamos os shaders dos arquivos "shader_fragment.glsl" e "shader_vertex.glsl".
        if (key == GLFW_KEY_R && action == GLFW_PRESS)
        {
            LoadShadersFromFiles();
            fprintf(stdout,"Shaders recarregados!\n");
            fflush(stdout);
        }
    }
}