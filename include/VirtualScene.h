#pragma once

#include <map>
#include <string>
#include "Camera.h"

class GameObject;

class VirtualScene {
    private:
        std::map<std::string, GameObject*> objects;
        Camera* camera;
	
    public:
        void RenderScene();
        void UpdateScene(float dt);
        void AddObject(std::string name, GameObject *object);
        GameObject* GetObject(std::string name);
        void RemoveObject(std::string name);
        void SetCamera(Camera* camera);
        Camera* GetCamera() {
            return camera;
        };
};
