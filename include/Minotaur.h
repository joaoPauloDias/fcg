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
            Cylinder hitBox;
            int health;
            std::pair<int, int> nextDirection = {0, 0};

        public:
            Minotaur(texture::TextureLoader textureLoader, glm::vec4 position);
            Cylinder getHitbox();
            std::pair<int, int> GetNextPosition();
            void ReceiveHit(int damage);
            void Render();
            void Update(float dt);
    };
};