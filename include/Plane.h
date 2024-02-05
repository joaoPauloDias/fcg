#pragma once

#include "GameObject.h"
#include "glm/vec4.hpp"
#include "ObjModel.h"
#include "TextureLoader.h"


namespace plane {
    class Plane: public GameObject {
        private:
            glm::vec4 position;
            glm::mat4 modelMatrix;
            ObjModel model;
            texture::TextureLoader textureLoader;

        public:
            Plane(texture::TextureLoader textureLoader, glm::vec4 position);
            void Render();
            void Update(float dt);
    };
};