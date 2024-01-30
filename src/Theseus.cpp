#include "Theseus.h"

#include "Maze.h"
#include "matrices.h"
#include "globals.h"

#include "GLFW/glfw3.h"

#define ATTACK_ACTIVE_DURATION 0.3
#define ATTACK_COOLDOWN_DURATION 0.5

#define CAMERA_RADIUS 0.5f
#define SWORD_SCALE_FACTOR 0.04f
#define SWORD_X_ANGLE (-M_PI_2 / 9)

using namespace theseus;

extern bool g_LeftMouseButtonPressed;
extern bool g_RightMouseButtonPressed;

Theseus::Theseus(texture::TextureLoader textureLoader, FreeCamera* camera)
    :
      swordModel("../../assets/models/sword.obj"), freeCamera(camera)
{
    swordModel.GetPart("sword")->setTextures(textureLoader.GetTexture("sword_normals"),NULL,NULL);
    position = glm::vec4(2.0f, 2.0f, 2.0f, 1.0f);
}


void Theseus::Render() {
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "illumination"), 1);
    swordModel.ApplyModelMatrix(modelMatrix);
    swordModel.Draw();
}

void Theseus::Update(float dt) {
    float t;
    switch (attackStatus) {
        case ATTACK_AVAILABLE:
            if (g_LeftMouseButtonPressed) {
                attackTime = 0;
                attackStatus = ATTACK_ACTIVE;
            }
            break;

        case ATTACK_ACTIVE:
            attackTime += dt;

            t = attackTime / ATTACK_ACTIVE_DURATION;

            if (attackTime >= ATTACK_ACTIVE_DURATION) {
                attackTime = ATTACK_COOLDOWN_DURATION;
                attackStatus = ATTACK_IN_COOLDOWN;
            }
            break;
        
        case ATTACK_IN_COOLDOWN:
            attackTime -= dt;

            t = attackTime / ATTACK_COOLDOWN_DURATION;

            if (attackTime <= 0) {
                attackTime = 0;
                attackStatus = ATTACK_AVAILABLE;
            }
            break;
        default:
            break;
    }

    // It is important to compute the sword model matrix before updating the position attribute
    // to draw the sword in the correct position
    glm::vec4 swordPosition = 
        position + 
        freeCamera->view_vector * 0.2f + 
        freeCamera->u * 0.1f + 
        glm::vec4(0.0f, -0.1f, 0.0f, 0.0f);

    //float t = attackTime/ATTACK_ACTIVE_DURATION;
    modelMatrix = 
        Matrix_Translate(swordPosition.x, swordPosition.y, swordPosition.z) *
        Matrix_Rotate_Y(freeCamera->theta + M_PI_2 + 0.12f) *
        Matrix_Rotate_Z(M_PI_2 * t) *
        Matrix_Rotate_X(SWORD_X_ANGLE) *
        Matrix_Scale(SWORD_SCALE_FACTOR, SWORD_SCALE_FACTOR, SWORD_SCALE_FACTOR);

    glm::vec4 newCameraPosition = freeCamera->getNewPosition(dt);
    
    maze::Maze* maze = (maze::Maze*) GetVirtualScene()->GetObject("maze");

    if(!maze->checkCollision(newCameraPosition, CAMERA_RADIUS)) {
        position = newCameraPosition;
        freeCamera->position = newCameraPosition;
    }
}