#include "Engine.h"
#include <cstdio>
#include <map>
#include <string>
#include <stdexcept>

#include "matrices.h"
#include "globals.h"

#include "SceneObject.h"
#include "TextureLoader.h"
#include "Maze.h"

void LoadShadersFromFiles();
void TextRendering_Init();
void TextRendering_ShowFramesPerSecond(GLFWwindow *window);
extern float g_ScreenRatio;
extern std::map<std::string, SceneObject> g_VirtualScene;
extern texture::TextureLoader textureLoader;
extern FreeCamera camera;
extern bool g_LeftMouseButtonPressed;
extern bool g_RightMouseButtonPressed; // Análogo para botão direito do mouse
extern bool g_MiddleMouseButtonPressed;
extern bool g_ShowInfoText;
extern double g_LastCursorPosX;
extern double g_LastCursorPosY;
extern maze::Maze myMaze;

namespace engine
{
    GLFWwindow *Init()
    {
        // Inicializamos a biblioteca GLFW, utilizada para criar uma janela do
        // sistema operacional, onde poderemos renderizar com OpenGL.
        int success = glfwInit();
        if (!success)
        {
            fprintf(stderr, "ERROR: glfwInit() failed.\n");
            std::exit(EXIT_FAILURE);
        }

        // Pedimos para utilizar OpenGL versão 3.3 (ou superior)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        // Pedimos para utilizar o perfil "core", isto é, utilizaremos somente as
        // funções modernas de OpenGL.
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Criamos uma janela do sistema operacional, com 800 colunas e 600 linhas
        // de pixels, e com título "INF01047 ...".
        GLFWwindow *window = glfwCreateWindow(800, 600, "Tower Defense", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            fprintf(stderr, "ERROR: glfwCreateWindow() failed.\n");
            std::exit(EXIT_FAILURE);
        }

        // Definimos o callback para impressão de erros da GLFW no terminal
        glfwSetErrorCallback(ErrorCallback);

        // Indicamos que as chamadas OpenGL deverão renderizar nesta janela
        glfwMakeContextCurrent(window);

        // Carregamento de todas funções definidas por OpenGL 3.3, utilizando a
        // biblioteca GLAD.
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        // Definimos a função de callback que será chamada sempre que a janela for
        // redimensionada, por consequência alterando o tamanho do "framebuffer"
        // (região de memória onde são armazenados os pixels da imagem).
        glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
        FramebufferSizeCallback(window, 800, 600); // Forçamos a chamada do callback acima, para definir g_ScreenRatio.

        // Imprimimos no terminal informações sobre a GPU do sistema
        const GLubyte *vendor = glGetString(GL_VENDOR);
        const GLubyte *renderer = glGetString(GL_RENDERER);
        const GLubyte *glversion = glGetString(GL_VERSION);
        const GLubyte *glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);

        printf("GPU: %s, %s, OpenGL %s, GLSL %s\n", vendor, renderer, glversion, glslversion);

        // auto keyCallback = std::bind(&engine::Engine::KeyCallback, TheEngine);
        glfwSetKeyCallback(window, KeyCallback);
        // ... ou clicar os botões do mouse ...
        glfwSetMouseButtonCallback(window, MouseButtonCallback);
        // ... ou movimentar o cursor do mouse em cima da janela ...
        glfwSetCursorPosCallback(window, CursorPosCallback);
        // ... ou rolar a "rodinha" do mouse.
        glfwSetScrollCallback(window, ScrollCallback);

        return window;
    }

    // Definimos o callback para impressão de erros da GLFW no terminal
    void ErrorCallback(int error, const char *description)
    {
        fprintf(stderr, "ERROR: GLFW: %s\n", description);
    }

    // Definição da função que será chamada sempre que a janela do sistema
    // operacional for redimensionada, por consequência alterando o tamanho do
    // "framebuffer" (região de memória onde são armazenados os pixels da imagem).
    void FramebufferSizeCallback(GLFWwindow *window, int width, int height)
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

    // Função callback chamada sempre que o usuário aperta algum dos botões do mouse
    void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
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
    void CursorPosCallback(GLFWwindow *window, double xpos, double ypos)
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
    void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
    {
        camera.handleScroll(xoffset, yoffset);
    }

    // Definição da função que será chamada sempre que o usuário pressionar alguma
    // tecla do teclado. Veja http://www.glfw.org/docs/latest/input_guide.html#input_key
    void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mod)
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
            fprintf(stdout, "Shaders recarregados!\n");
            fflush(stdout);
        }
    }

    void Run(GLFWwindow *window)
    {
        // Inicializamos o código para renderização de texto.
        TextRendering_Init();

        // Habilitamos o Z-buffer. Veja slides 104-116 do documento Aula_09_Projecoes.pdf.
        glEnable(GL_DEPTH_TEST);

        // Habilitamos o Backface Culling. Veja slides 23-34 do documento Aula_13_Clipping_and_Culling.pdf e slides 112-123 do documento Aula_14_Laboratorio_3_Revisao.pdf.
        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_BACK);
        // glFrontFace(GL_CCW);

        double prev_time = glfwGetTime();

        // Ficamos em um loop infinito, renderizando, até que o usuário feche a janela
        while (!glfwWindowShouldClose(window))
        {
            double cur_time = glfwGetTime();
            double dt = cur_time - prev_time;
            prev_time = cur_time;
            // Aqui executamos as operações de renderização

            // Definimos a cor do "fundo" do framebuffer como branco.  Tal cor é
            // definida como coeficientes RGBA: Red, Green, Blue, Alpha; isto é:
            // Vermelho, Verde, Azul, Alpha (valor de transparência).
            // Conversaremos sobre sistemas de cores nas aulas de Modelos de Iluminação.
            //
            //           R     G     B     A
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

            // "Pintamos" todos os pixels do framebuffer com a cor definida acima,
            // e também resetamos todos os pixels do Z-buffer (depth buffer).
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Pedimos para a GPU utilizar o programa de GPU criado acima (contendo
            // os shaders de vértice e fragmentos).
            glUseProgram(g_GpuProgramID);

            camera.update(dt);

            glm::mat4 model = Matrix_Identity(); // Transformação identidade de modelagem

            model = Matrix_Translate(0.0f, -1.0f, 0.0f) * Matrix_Scale(0.03f, 0.03f, 0.03f);
            g_VirtualScene["Eyes_mesh"].ApplyModelMatrix(model);
            g_VirtualScene["Eyes_mesh"].Draw();

            g_VirtualScene["Body_mesh"].ApplyModelMatrix(model);
            g_VirtualScene["Body_mesh"].Draw();

            g_VirtualScene["Teeth_mesh"].ApplyModelMatrix(model);
            g_VirtualScene["Teeth_mesh"].Draw();

            g_VirtualScene["Pants_mesh"].ApplyModelMatrix(model);
            g_VirtualScene["Pants_mesh"].Draw();

            for (auto [isWall, m] : myMaze.blocks)
            {
                g_VirtualScene[isWall ? "Wall" : "Ground"].ApplyModelMatrix(m);
                g_VirtualScene[isWall ? "Wall" : "Ground"].Draw();
            }

            // myMaze.renderMaze();

            TextRendering_ShowFramesPerSecond(window);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        // Finalizamos o uso dos recursos do sistema operacional
        glfwTerminate();
    }
}