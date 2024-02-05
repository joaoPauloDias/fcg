#include "VirtualScene.h"
#include "GameObject.h"
#include <iostream>

void VirtualScene::RenderScene() {
    for (auto &&[name, object]: objects) {
        object->Render();
    }
}

void VirtualScene::UpdateScene(float dt) {
    camera->update(dt);
    for (auto &&[name, object]: objects) {
        object->Update(dt);
    }
}

void VirtualScene::AddObject(std::string name, GameObject* object) {
    objects[name] = object;
    object->SetVirtualScene(this);
}

GameObject *VirtualScene::GetObject(std::string name) {
    if (objects.find(name) != objects.end()) {
        return objects[name];
    }
    return nullptr;
}

void VirtualScene::RemoveObject(std::string name) {
    objects.erase(name);
}

void VirtualScene::SetCamera(Camera* camera) {
    this->camera = camera;
}