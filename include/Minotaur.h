#pragma once

#include "GameObject.h"
#include "ObjModel.h"
#include "TextureLoader.h"

class Minotaur : public GameObject {
    private:
        ObjModel model;
        glm::mat4 modelMatrix;

    public:
        Minotaur(texture::TextureLoader textureLoader);
        void Render();
        void Update(float dt);
};