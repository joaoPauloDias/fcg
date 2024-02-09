#include "GameObject.h"
#include "SoundPlayer.h"
#include "glm/vec4.hpp"
#include "ObjModel.h"
#include "TextureLoader.h"
#include "collisions.h"
#include <random>


namespace minotaur {
    class Minotaur: public GameObject {
        private:
            glm::vec4 position;
            glm::mat4 modelMatrix;
            ObjModel model;
            collisions::Cylinder hitBox;
            int health;
            int attackDamage = 1;
            float distance;
            float velocity;
            Sound *death;
            Sound *hit;
            Sound *roar;
            Sound *step;
            std::pair<int, int> nextDirection = {0, 0};
            std::default_random_engine randomGenerator_;
            bool damageAnimation = false;
            float damageAnimationTime = 0;
            texture::TextureLoader textureLoader;
            long unsigned int maxDistanceDetectable = 15;
            void UpdateSounds();

        public:
            Minotaur(texture::TextureLoader textureLoader, glm::vec4 position);
            collisions::Cylinder getHitbox();
            std::pair<int, int> GetNextPosition();
            void SetPosition(glm::vec4 position);
            void GetHit(int damage);
            void Render();
            void Update(float dt);
    };
};