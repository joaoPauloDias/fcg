#pragma once

#include "VirtualScene.h"
#include "TextureLoader.h"


class MenuScene : public VirtualScene {
    private:
        LookAtCamera camera;

    public:
        MenuScene(texture::TextureLoader textureLoader);
};