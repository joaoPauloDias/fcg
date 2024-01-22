#pragma once

class GameObject; // Declaration to break circular dependency between GameObject and VirtualScene

#include <map>
#include <string>

class VirtualScene {
    private:
        std::map<std::string, GameObject*> objects;
	
    public:
        void RenderScene();
        void Update(float dt);
        void AddObject(std::string name, GameObject *object);
        GameObject* GetObject(std::string name);
};
