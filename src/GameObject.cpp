#include "GameObject.h"


void GameObject::SetVirtualScene(VirtualScene* scene) {
    this->scene = scene;
}

VirtualScene* GameObject::GetVirtualScene() { 
    return this->scene;
}
