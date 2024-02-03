#pragma once

#include "GameObject.h"
#include "TextureLoader.h"
#include "Camera.h"
#include "ObjModel.h"

namespace skybox {

class SkyBox : public GameObject {
    private:
        ObjModel sphereModel;
        ObjModel wingsModel;
        FreeCamera* camera;
        glm::mat4 modelMatrix;
        glm::vec4 wingsPosition;
        float t = 0;
        bool showIcarus = false;

    public:
        SkyBox(texture::TextureLoader textureLoader, FreeCamera* camera);
        void Render();
        void Update(float dt);
};

};