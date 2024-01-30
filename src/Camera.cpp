#include "Camera.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "matrices.h"
#include "globals.h"

extern float g_ScreenRatio;

bool FREE = false;

LookAtCamera::LookAtCamera(float t, float p, float d) {
    theta = t;
    phi = p;
    distance = d;
}

void LookAtCamera::update(float dt) {
    float r = distance;
    float y = r*sin(phi);
    float z = r*cos(phi)*cos(theta);
    float x = r*cos(phi)*sin(theta);

    // Abaixo definimos as varáveis que efetivamente definem a câmera virtual.
    // Veja slides 195-227 e 229-234 do documento Aula_08_Sistemas_de_Coordenadas.pdf.
    glm::vec4 camera_position_c  = glm::vec4(x,y,z,1.0f); // Ponto "c", centro da câmera
    glm::vec4 camera_lookat_l    = glm::vec4(0.0f,0.0f,0.0f,1.0f); // Ponto "l", para onde a câmera (look-at) estará sempre olhando
    glm::vec4 camera_view_vector = camera_lookat_l - camera_position_c; // Vetor "view", sentido para onde a câmera está virada
    glm::vec4 camera_up_vector   = glm::vec4(0.0f,1.0f,0.0f,0.0f); // Vetor "up" fixado para apontar para o "céu" (eito Y global)

    // Computamos a matriz "View" utilizando os parâmetros da câmera para
    // definir o sistema de coordenadas da câmera.  Veja slides 2-14, 184-190 e 236-242 do documento Aula_08_Sistemas_de_Coordenadas.pdf.
    glm::mat4 view = Matrix_Camera_View(camera_position_c, camera_view_vector, camera_up_vector);

    // Note que, no sistema de coordenadas da câmera, os planos near e far
    // estão no sentido negativo! Veja slides 176-204 do documento Aula_09_Projecoes.pdf.
    float nearplane = -0.1f;  // Posição do "near plane"
    float farplane  = -10.0f; // Posição do "far plane"

    float field_of_view = 3.141592 / 3.0f;
    glm::mat4 projection = Matrix_Perspective(field_of_view, g_ScreenRatio, nearplane, farplane);  

    // Enviamos as matrizes "view" e "projection" para a placa de vídeo
    // (GPU). Veja o arquivo "shader_vertex.glsl", onde estas são
    // efetivamente aplicadas em todos os pontos.
    glUniformMatrix4fv(g_view_uniform       , 1 , GL_FALSE , glm::value_ptr(view));
    glUniformMatrix4fv(g_projection_uniform , 1 , GL_FALSE , glm::value_ptr(projection));
}

void LookAtCamera::handleScroll(double xoffset, double yoffset) {
    distance -= 0.1f*yoffset;

    const float verysmallnumber = std::numeric_limits<float>::epsilon();
    if (distance < verysmallnumber)
        distance = verysmallnumber;
}

void LookAtCamera::handleCursor(double dx, double dy) {
    // Atualizamos parâmetros da câmera com os deslocamentos
    theta -= 0.01f*dx;
    phi   += 0.01f*dy;

    // Em coordenadas esféricas, o ângulo phi deve ficar entre -pi/2 e +pi/2.
    float phimax = 3.141592f/2;
    float phimin = -phimax;

    if (phi > phimax)
        phi = phimax;

    if (phi < phimin)
        phi = phimin;
}

void LookAtCamera::handleKey(int key, int scancode, int action, int mod) {}

FreeCamera::FreeCamera(float t, float p, glm::vec4 pos, float s) {
    theta = t;
    phi = p;
    position = pos;
    speed = s;
}


glm::vec4 FreeCamera::getNewPosition(float dt) {
    float y = sin(phi);
    float z = cos(phi)*cos(theta);
    float x = cos(phi)*sin(theta);

    // Abaixo definimos as varáveis que efetivamente definem a câmera virtual.
    // Veja slides 195-227 e 229-234 do documento Aula_08_Sistemas_de_Coordenadas.pdf.
    glm::vec4 camera_view_vector = glm::vec4(x, FREE ? y : .0f, z, 0.0f); // Vetor "view", sentido para onde a câmera está virada
    glm::vec4 camera_up_vector   = glm::vec4(0.0f,1.0f,0.0f,0.0f); // Vetor "up" fixado para apontar para o "céu" (eito Y global)

    w = -camera_view_vector/norm(camera_view_vector);
    u = crossproduct(camera_up_vector, w);
    glm::vec4 new_position = position;

    if (wPressed) {
        new_position -= w * speed * dt;
    }
    if (sPressed) {
        new_position += w * speed * dt;
    }
    if (aPressed) {
        new_position -= u * speed * dt;
    }
    if (dPressed) {
        new_position += u * speed * dt;
    }

    return new_position;
}


void FreeCamera::update(float dt) {
    float y = sin(phi);
    float z = cos(phi)*cos(theta);
    float x = cos(phi)*sin(theta);

    // Abaixo definimos as varáveis que efetivamente definem a câmera virtual.
    // Veja slides 195-227 e 229-234 do documento Aula_08_Sistemas_de_Coordenadas.pdf.
    view_vector = glm::vec4(x, y, z, 0.0f); // Vetor "view", sentido para onde a câmera está virada
    glm::vec4 camera_up_vector   = glm::vec4(0.0f,1.0f,0.0f,0.0f); // Vetor "up" fixado para apontar para o "céu" (eito Y global)

    // Computamos a matriz "View" utilizando os parâmetros da câmera para
    // definir o sistema de coordenadas da câmera.  Veja slides 2-14, 184-190 e 236-242 do documento Aula_08_Sistemas_de_Coordenadas.pdf.
    glm::mat4 view = Matrix_Camera_View(position, view_vector, camera_up_vector);

    // Note que, no sistema de coordenadas da câmera, os planos near e far
    // estão no sentido negativo! Veja slides 176-204 do documento Aula_09_Projecoes.pdf.
    float nearplane = -0.01f;  // Posição do "near plane"
    float farplane  = -100.0f; // Posição do "far plane"

    float field_of_view = 3.141592 / 3.0f;
    glm::mat4 projection = Matrix_Perspective(field_of_view, g_ScreenRatio, nearplane, farplane);  

    // Enviamos as matrizes "view" e "projection" para a placa de vídeo
    // (GPU). Veja o arquivo "shader_vertex.glsl", onde estas são
    // efetivamente aplicadas em todos os pontos.
    glUniformMatrix4fv(g_view_uniform       , 1 , GL_FALSE , glm::value_ptr(view));
    glUniformMatrix4fv(g_projection_uniform , 1 , GL_FALSE , glm::value_ptr(projection));
}

void FreeCamera::handleScroll(double xoffset, double yoffset) {}


void FreeCamera::handleCursor(double dx, double dy) {
    // Atualizamos parâmetros da câmera com os deslocamentos
    theta -= 0.01f*dx;
    phi   -= 0.01f*dy;

    // Em coordenadas esféricas, o ângulo phi deve ficar entre -pi/2 e +pi/2.
    float phimax = 3.141592f/2;
    float phimin = -phimax;

    if (phi > phimax)
        phi = phimax;

    if (phi < phimin)
        phi = phimin;
}

void FreeCamera::handleKey(int key, int scancode, int action, int mod) {
    if (key == GLFW_KEY_W) {
        if (action == GLFW_PRESS) {
            wPressed = true;
        } else if (action == GLFW_RELEASE) {
            wPressed = false;
        }
    }
    if (key == GLFW_KEY_S) {
        if (action == GLFW_PRESS) {
            sPressed = true;
        } else if (action == GLFW_RELEASE) {
            sPressed = false;
        }
    }
    if (key == GLFW_KEY_A) {
        if (action == GLFW_PRESS) {
            aPressed = true;
        } else if (action == GLFW_RELEASE) {
            aPressed = false;
        }
    }
    if (key == GLFW_KEY_D) {
        if (action == GLFW_PRESS) {
            dPressed = true;
        } else if (action == GLFW_RELEASE) {
            dPressed = false;
        }
    }

    if (key == GLFW_KEY_LEFT_SHIFT) {
        if (action == GLFW_PRESS) {
            FREE = !FREE;
        }
    }
}