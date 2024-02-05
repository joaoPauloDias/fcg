#pragma once

#include "VirtualScene.h"
#include "TextureLoader.h"
#include "VirtualScene.h"
#include "Maze.h"
#include "Theseus.h"
#include "Minotaur.h"
#include "SkyBox.h"

class GameScene : public VirtualScene {
    private:
        FreeCamera camera;
        skybox::SkyBox mySkyBox;
        maze::Maze myMaze;
        theseus::Theseus myTheseus;
        minotaur::Minotaur myMinotaur;

    public:
        GameScene(texture::TextureLoader);
        void UpdateScene(float dt);
};