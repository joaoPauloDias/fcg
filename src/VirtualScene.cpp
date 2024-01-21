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
}

GameObject *VirtualScene::GetObject(std::string name) {
    return objects[name];
}

