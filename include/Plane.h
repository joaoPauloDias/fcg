#pragma once

#include "SoundPlayer.h"
#include "GameObject.h"
#include "glm/vec4.hpp"
#include "ObjModel.h"
#include "TextureLoader.h"
#include "Camera.h"


namespace plane {
    class Plane: public GameObject {
        private:
            glm::vec4 position;
            glm::mat4 modelMatrix;
            ObjModel model;
            texture::TextureLoader textureLoader;
            Camera *camera;
            Sound* laugh;
            Sound* win;


        public:
            Plane(texture::TextureLoader textureLoader, glm::vec4 position, Camera *camera);
            void Render();
            void Update(float dt);
    };
};