#include "GameObject.h"

void GameObject::setScene(VirtualScene *scene) {
    this->scene = scene;
}

VirtualScene* GameObject::getScene() {
    return scene;
}