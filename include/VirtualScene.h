#pragma once

#include <map>
#include <string>
#include "Camera.h"

class GameObject;

class VirtualScene {
    private:
        std::map<std::string, GameObject*> objects;
        FreeCamera* freeCamera = nullptr;
        LookAtCamera* lookAtCamera = nullptr;
	
    public:
        void RenderScene();
        void UpdateScene(float dt);
        void AddObject(std::string name, GameObject *object);
        GameObject* GetObject(std::string name);
        void RemoveObject(std::string name);
        void SetCamera(FreeCamera* camera);
        FreeCamera* GetFreeCamera() {
            return freeCamera;
        };
        void SetCamera(LookAtCamera* camera);
};
