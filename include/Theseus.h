#pragma once

#include "GameObject.h"
#include "ObjModel.h"
#include "TextureLoader.h"
#include "Camera.h"


namespace theseus{
    class Theseus : public GameObject {
    private:
        ObjModel swordModel;
        FreeCamera* freeCamera;
        glm::vec4 position;
        glm::mat4 modelMatrix;
        
    public:
        Theseus(texture::TextureLoader textureLoader, FreeCamera* camera);
        void Render();
        void Update(float dt);
    };
}