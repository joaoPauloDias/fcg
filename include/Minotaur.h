#include "GameObject.h"
#include "glm/vec4.hpp"
#include "ObjModel.h"
#include "TextureLoader.h"
#include "collisions.h"

namespace minotaur {
    class Minotaur: public GameObject {
        private:
            glm::vec4 position;
            glm::mat4 modelMatrix;
            ObjModel model;
            collisions::Cylinder hitBox;
            int health;
            float velocity;
            std::pair<int, int> nextDirection = {0, 0};
            bool damageAnimation = false;
            float damageAnimationTime = 0;
            texture::TextureLoader textureLoader;

        public:
            Minotaur(texture::TextureLoader textureLoader, glm::vec4 position);
            collisions::Cylinder getHitbox();
            std::pair<int, int> GetNextPosition();
            void ReceiveHit(int damage);
            void Render();
            void Update(float dt);
    };
};