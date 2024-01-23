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
        
    public:
        Theseus(texture::TextureLoader textureLoader, FreeCamera* camera);
        void Render();
        void Update(float dt);
    };
}