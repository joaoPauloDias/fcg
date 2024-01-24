#pragma once

#include "VirtualScene.h"


class GameObject {
    private:
        VirtualScene *scene;

    public:
        virtual void Render() = 0;
        virtual void Update(float dt) = 0;
        void SetVirtualScene(VirtualScene *scene);
        VirtualScene* GetVirtualScene();
};