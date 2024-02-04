#include "Theseus.h"

#include "Maze.h"
#include "Minotaur.h"
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
extern bool FREE;

Theseus::Theseus(texture::TextureLoader textureLoader, FreeCamera* camera)
    : swordModel("../../assets/models/sword.obj"), 
      shieldModel("../../assets/models/shield.obj"),
      freeCamera(camera)

{
    swordModel.GetPart("sword")->setTextures(textureLoader.GetTexture("sword_diffuse"),NULL,NULL);
    swordModel.GetPart("Shield_Sphere.001")->setTextures(textureLoader.GetTexture("shield_diffuse"),NULL,NULL);
    position = glm::vec4(2.0f, 2.0f, 2.0f, 1.0f);
    hitBox.center = position;
    hitBox.radius = CAMERA_RADIUS;
}


void Theseus::Render() {
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "illumination"), ILLUMINATION_BLINN_PHONG);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "interpolation"), INTERPOLATION_PHONG);
    swordModel.ApplyModelMatrix(swordModelMatrix);
    swordModel.Draw();

    glUniform1i(glGetUniformLocation(g_GpuProgramID, "illumination"), ILLUMINATION_BLINN_PHONG);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "interpolation"), INTERPOLATION_PHONG);
    shieldModel.ApplyModelMatrix(shieldModelMatrix);
    shieldModel.Draw();
}

void Theseus::Update(float dt) {
    float t = 0;
    switch (attackStatus) {
        case ATTACK_AVAILABLE:
            AttackAvailable();
            break;

        case ATTACK_ACTIVE:
            AttackActive(dt, t);
            break;
        
        case ATTACK_IN_COOLDOWN:
            AttackCooldown(dt, t);
            break;
        default:
            break;
    }

    // It is important to compute the sword model matrix before updating the position attribute
    // to draw the sword in the correct position
    glm::vec4 swordPosition = 
        position + 
        freeCamera->view_vector * (0.2f + 0.15f*t)+ 
        freeCamera->u * 0.1f + 
        glm::vec4(0.0f, -0.1f, 0.0f, 0.0f);

    //float t = attackTime/ATTACK_ACTIVE_DURATION;
    swordModelMatrix = 
        Matrix_Translate(swordPosition.x, swordPosition.y, swordPosition.z) *
        Matrix_Rotate_Y(freeCamera->theta + M_PI_2 + 0.12f) *
        Matrix_Rotate_Z(M_PI_2 * t) *
        Matrix_Rotate_X(SWORD_X_ANGLE) *
        Matrix_Scale(SWORD_SCALE_FACTOR, SWORD_SCALE_FACTOR, SWORD_SCALE_FACTOR);
    
    glm::vec4 shieldPosition = 
        position + 
        freeCamera->view_vector * (0.2f + 0.15f*t)- 
        freeCamera->u * 0.1f + 
        glm::vec4(0.0f, -0.1f, 0.0f, 0.0f);
    shieldModelMatrix = 
        Matrix_Translate(shieldPosition.x, shieldPosition.y, shieldPosition.z) *
        Matrix_Rotate_Y(freeCamera->theta) * 
        Matrix_Scale(SWORD_SCALE_FACTOR, SWORD_SCALE_FACTOR, SWORD_SCALE_FACTOR);
    
    minotaur::Minotaur* minotaur = (minotaur::Minotaur*) GetVirtualScene()->GetObject("minotaur");
    if (minotaur != nullptr) {
        float swordLength = swordModel.GetPart("sword")->bbox_max.y;
        glm::vec4 swordTip = swordModelMatrix * glm::vec4(0.0f, swordLength, 0.0f, 1.0f);

        if (attackStatus == ATTACK_ACTIVE && !inflictedDamage && collisions::checkCollision(minotaur->getHitbox(), swordTip)) {
            minotaur->ReceiveHit(1);
            inflictedDamage = true;
        }
    }

    maze::Maze* maze = (maze::Maze*) GetVirtualScene()->GetObject("maze");

    std::pair<bool, bool> dirs[] = {{true, true}, {true, false}, {false, true}};

    for (auto [update_x, update_z] : dirs) {
        hitBox.center = freeCamera->getNewPosition(dt, update_x, FREE, update_z);
        if(!maze->checkCollision(hitBox)) {
            position = hitBox.center;
            freeCamera->position = hitBox.center;
            break;
        }
    }
}

void Theseus::AttackAvailable() {
    if (g_LeftMouseButtonPressed) {
        attackTime = 0;
        attackStatus = ATTACK_ACTIVE;
    }
}

void theseus::Theseus::AttackActive(float dt, float& t) {
    attackTime += dt;

    t = attackTime / ATTACK_ACTIVE_DURATION;

    if (attackTime >= ATTACK_ACTIVE_DURATION) {
        attackTime = ATTACK_COOLDOWN_DURATION;
        attackStatus = ATTACK_IN_COOLDOWN;
    }
}


void theseus::Theseus::AttackCooldown(float dt, float& t) {
    attackTime -= dt;

    t = attackTime / ATTACK_COOLDOWN_DURATION;

    if (attackTime <= 0) {
        attackTime = 0;
        attackStatus = ATTACK_AVAILABLE;
        inflictedDamage = false;
    }
}
