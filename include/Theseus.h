#pragma once

#include "GameObject.h"
#include "ObjModel.h"
#include "TextureLoader.h"
#include "Camera.h"
#include "collisions.h"
#include "AudioManager.h"
#include <functional>

#define ATTACK_AVAILABLE 0
#define ATTACK_ACTIVE 1
#define ATTACK_IN_COOLDOWN 2
#define DEFENSE_AVAILABLE 0
#define DEFENSE_ACTIVE 1
#define DEFENSE_IN_COOLDOWN 2

namespace theseus
{
    class Theseus : public GameObject
    {
    private:
        ObjModel swordModel;
        ObjModel shieldModel;
        FreeCamera *freeCamera;
        glm::vec4 position;
        glm::mat4 modelMatrix;
        collisions::Sphere hitBox;
        glm::mat4 swordModelMatrix;
        glm::mat4 shieldModelMatrix;
        Sound *sword;
        Sound *shield;
        Sound *step;
        Sound *hurt;
        int health;
        int attackDamage = 1;
        int attackStatus = ATTACK_AVAILABLE;
        float attackTime = 0.0f;
        bool inflictedDamage = false;
        int defenseStatus = DEFENSE_AVAILABLE;
        float defenseTime = 0.0f;

    public:
        Theseus(texture::TextureLoader textureLoader, FreeCamera *camera);
        const glm::vec4 &getPosition() const
        {
            return position;
        }
        const collisions::Sphere &getHitBox() const
        {
            return hitBox;
        }
        void Render();
        void Update(float dt);
        void AttackAvailable();
        void AttackCooldown(float dt, float &t);
        void AttackActive(float dt, float &t);
        void DefenseAvailable(float dt);
        void DefenseCooldown(float dt, float &t);
        void DefenseActive(float dt, float &t);
        void CheckMinotaurInteraction(float dt);
        void GetHit(int damage);
        glm::vec4 getNewPosition(float dt, bool update_x, bool update_y,
                                 bool update_z);
        void HandlePlayerMovement(float dt);
        void SlideMovement(float dt, std::function<glm::vec4(bool, bool, float)> func);
    };
    }  // namespace theseus