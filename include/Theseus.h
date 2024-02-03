#pragma once

#include "GameObject.h"
#include "ObjModel.h"
#include "TextureLoader.h"
#include "Camera.h"

#define ATTACK_AVAILABLE 0
#define ATTACK_ACTIVE 1
#define ATTACK_IN_COOLDOWN 2


namespace theseus{
class Theseus : public GameObject {
private:
    ObjModel swordModel;
    FreeCamera* freeCamera;
    glm::vec4 position;
    glm::mat4 modelMatrix;

    int attackStatus = ATTACK_AVAILABLE;
    float attackTime = 0.0f;
    bool inflictedDamage = false;

public:
    Theseus(texture::TextureLoader textureLoader, FreeCamera* camera);
    void Render();
    void Update(float dt);
    void AttackAvailable();
    void AttackCooldown(float dt, float& t);
    void AttackActive(float dt, float& t);
};
}  // namespace theseus