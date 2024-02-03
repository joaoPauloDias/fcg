#pragma once

#include "VirtualScene.h"

#define ILLUMINATION_DIFFUSE 0
#define ILLUMINATION_BLINN_PHONG 1
#define INTERPOLATION_PHONG 0
#define INTERPOLATION_GOURAUD 1

class GameObject {
    private:
        VirtualScene *scene;

    public:
        virtual void Render() = 0;
        virtual void Update(float dt) = 0;
        void SetVirtualScene(VirtualScene *scene);
        VirtualScene* GetVirtualScene();
};