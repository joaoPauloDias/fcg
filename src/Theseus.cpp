#include "Theseus.h"

#include "Maze.h"
#include "Minotaur.h"
#include "matrices.h"
#include "globals.h"


#include "GLFW/glfw3.h"

#define ATTACK_ACTIVE_DURATION 0.3
#define ATTACK_COOLDOWN_DURATION 0.5

#define ATTACK_SETUP_DURATION 0.3
#define ATTACK_COOLDOWN_DURATION 0.5


#define CAMERA_RADIUS 0.5f
#define SWORD_SCALE_FACTOR 0.04f
#define SHIELD_SCALE_FACTOR 0.07f
#define SWORD_X_ANGLE (-M_PI_2 / 9)

using namespace theseus;

extern bool g_LeftMouseButtonPressed;
extern bool g_RightMouseButtonPressed;
extern bool FREE;
extern bool wPressed;
extern bool sPressed;
extern bool aPressed;
extern bool dPressed;


Theseus::Theseus(texture::TextureLoader textureLoader, FreeCamera* camera)
    : swordModel("../../assets/models/sword.obj"), 
      shieldModel("../../assets/models/shield.obj"),
      freeCamera(camera),
      sword(AudioManager::makeSound("../../assets/audio/sword.wav", false, 0.4)),
      step(AudioManager::makeSound("../../assets/audio/step.wav", false, 0.1)),
      hurt(AudioManager::makeSound("../../assets/audio/hurt.wav", false, 0.8)),
      shield(AudioManager::makeSound("../../assets/audio/shield.wav", false, 0.6))
{
    swordModel.GetPart("sword")->setTextures(textureLoader.GetTexture("sword_diffuse"),NULL,NULL);
    shieldModel.GetPart("shield")->setTextures(textureLoader.GetTexture("shield_diffuse"),NULL,NULL);
    position = glm::vec4(2.0f, 0.5f, 2.0f, 1.0f);
    hitBox.center = position;
    health = 150;
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
    hitBox.center = position;

    if(health<=0){
        GetVirtualScene()->RemoveObject("theseus");
        activeScene = MENU_SCENE;
        activeMenu = GAME_OVER;
    }

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


    switch (defenseStatus) {
        case DEFENSE_AVAILABLE:
            DefenseAvailable(dt);
            break;

        case DEFENSE_ACTIVE:
            DefenseActive(dt, t);
            break;
        
        case DEFENSE_IN_COOLDOWN:
            DefenseCooldown(dt, t);
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
        freeCamera->view_vector * (0.2f + 0.15f * defenseTime) - 
        freeCamera->u * 0.1f + 
        glm::vec4(0.0f, -0.1f, 0.0f, 0.0f);
    shieldModelMatrix = 
        Matrix_Translate(shieldPosition.x, shieldPosition.y, shieldPosition.z) *
        Matrix_Rotate_Y(freeCamera->theta + M_PI + 0.12f) * 
        Matrix_Scale(SHIELD_SCALE_FACTOR, SHIELD_SCALE_FACTOR, SHIELD_SCALE_FACTOR);
    
    CheckMinotaurInteraction(dt);
    HandlePlayerMovement(dt);
}

void Theseus::AttackAvailable() {
    if (g_LeftMouseButtonPressed && defenseStatus == DEFENSE_AVAILABLE) {
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

    if (!AudioManager::isSoundPlaying(this->sword)) {
        AudioManager::playSound(this->sword);
    }
}


void theseus::Theseus::DefenseCooldown(float dt, float& t) {
    if (defenseTime > 0) {
        defenseTime -= dt;
    } else {
        defenseTime = 0;
        defenseStatus = DEFENSE_AVAILABLE;
    }
}

void Theseus::DefenseAvailable(float dt) {
    if (g_RightMouseButtonPressed && defenseTime >= 0.5) {
        defenseTime = 0.5;
        defenseStatus = DEFENSE_ACTIVE;
    } else if (g_RightMouseButtonPressed && defenseTime < 0.5) {
        defenseTime += dt;
    }
    if (!g_RightMouseButtonPressed && defenseTime > 0) {
        defenseStatus = DEFENSE_IN_COOLDOWN;
    }
}

void theseus::Theseus::DefenseActive(float dt, float& t) {
    if (!g_RightMouseButtonPressed) {
        defenseStatus = DEFENSE_IN_COOLDOWN;
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

void theseus::Theseus::GetHit(int damage){
    if(defenseStatus != DEFENSE_ACTIVE){
        health-=damage;
        if (!AudioManager::isSoundPlaying(this->hurt)) 
            AudioManager::playSound(this->hurt);
    }else if(!AudioManager::isSoundPlaying(this->shield))
        AudioManager::playSound(this->shield);
}


void theseus::Theseus::CheckMinotaurInteraction(float dt) {
    minotaur::Minotaur* minotaur = (minotaur::Minotaur*) GetVirtualScene()->GetObject("minotaur");

    if (minotaur == nullptr)
        return;

    float swordLength = swordModel.GetPart("sword")->bbox_max.y;
    glm::vec4 swordTip = swordModelMatrix * glm::vec4(0.0f, swordLength, 0.0f, 1.0f);

    collisions::Cylinder minotaurHitbox = minotaur->getHitbox();

    if (attackStatus == ATTACK_ACTIVE && !inflictedDamage && collisions::checkCollision(minotaurHitbox, swordTip)) {
        minotaur->GetHit(attackDamage);
        inflictedDamage = true;
    }

    if (defenseStatus == DEFENSE_ACTIVE)
        return;

    if (collisions::checkCollision(minotaurHitbox, getHitBox())) {
        glm::vec4 knockbackDirection = (position - minotaurHitbox.center)/norm(position - minotaurHitbox.center);
        knockbackDirection.y = 0;

        SlideMovement(dt, [&](bool update_x, bool update_z, float dt) {
            knockbackDirection.x = update_x;
            knockbackDirection.z = update_z;
            
            return position + knockbackDirection * 1.0f * dt;
        });
    }
}

void theseus::Theseus::SlideMovement(float dt, std::function<glm::vec4(bool, bool, float)> func) {
    std::pair<bool, bool> dirs[] = {{true, true}, {true, false}, {false, true}};
    maze::Maze* maze = (maze::Maze*) GetVirtualScene()->GetObject("maze");
    for (auto [update_x, update_z] : dirs) {
        collisions::Sphere newHitbox = hitBox;

        newHitbox.center = func(update_x, update_z, dt);

        if (!maze->checkCollision(newHitbox)) {
            position = newHitbox.center;
            hitBox = newHitbox;
            break;
        }
    }
}


void theseus::Theseus::HandlePlayerMovement(float dt) {
    SlideMovement(dt, [this](bool update_x, bool update_z, float dt){
        return getNewPosition(dt, update_x, FREE, update_z);
    });
    if(freeCamera->position != position){
        freeCamera->position = position;
        if (!AudioManager::isSoundPlaying(this->step))AudioManager::playSound(this->step);

    }
}

glm::vec4 Theseus::getNewPosition(float dt, bool update_x, bool update_y, bool update_z) {
    // Calculating position based on player movement
    float y = sin(freeCamera->phi);
    float z = cos(freeCamera->phi)*cos(freeCamera->theta);
    float x = cos(freeCamera->phi)*sin(freeCamera->theta);

    // Abaixo definimos as varáveis que efetivamente definem a câmera virtual.
    // Veja slides 195-227 e 229-234 do documento Aula_08_Sistemas_de_Coordenadas.pdf.
    glm::vec4 camera_view_vector = glm::vec4(x, FREE ? y : .0f, z, 0.0f); // Vetor "view", sentido para onde a câmera está virada
    glm::vec4 camera_up_vector   = glm::vec4(0.0f,1.0f,0.0f,0.0f); // Vetor "up" fixado para apontar para o "céu" (eito Y global)

    freeCamera->w = -camera_view_vector/norm(camera_view_vector);
    freeCamera->u = crossproduct(camera_up_vector, freeCamera->w);

    glm::vec4 new_position = position;
    glm::vec4 updateMask = glm::vec4(update_x, update_y, update_z, 1.0f);

    if (wPressed) {
        new_position -= freeCamera->w * freeCamera->speed * dt * updateMask;
    }
    if (sPressed) {
        new_position += freeCamera->w * freeCamera->speed * dt * updateMask;
    }
    if (aPressed) {
        new_position -= freeCamera->u * freeCamera->speed * dt * updateMask;
    }
    if (dPressed) {
        new_position += freeCamera->u * freeCamera->speed * dt * updateMask;
    }

    return new_position;
}