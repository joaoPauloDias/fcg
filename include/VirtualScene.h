#pragma once

#include <map>
#include <string>

class GameObject;

class VirtualScene {
    private:
        std::map<std::string, GameObject*> objects;
	
    public:
        void RenderScene();
        void UpdateScene(float dt);
        void AddObject(std::string name, GameObject *object);
        GameObject* GetObject(std::string name);
        void RemoveObject(std::string name);
};
