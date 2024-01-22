#include "VirtualScene.h"

void VirtualScene::RenderScene() {
    for (auto &&[name, object]: objects) {
        object->Render();
    }
}

void VirtualScene::Update(float dt) {
    for (auto &&[name, object]: objects) {
        object->Update(dt);
    }
}

void VirtualScene::AddObject(std::string name, GameObject *object) {
    objects[name] = object;
    object->setScene(this);
}

GameObject *VirtualScene::GetObject(std::string name) {
    if (objects.find(name) == objects.end())
        return NULL;
    return objects[name];
}
