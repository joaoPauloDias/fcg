#include "GameObject.h"

#include <map>
#include <string>

class VirtualScene {
    private:
        std::map<std::string, GameObject*> objects;
    
    public:
        VirtualScene();
        void RenderScene();
        void Update(float dt);
        void AddObject(std::string name, GameObject *object);
        GameObject* GetObject(std::string name);
};