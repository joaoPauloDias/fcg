#pragma once

#include "VirtualScene.h"
#include "TextureLoader.h"
#include "Plane.h"
#include "SkyBox.h"
#include "Maze.h"

class MenuScene : public VirtualScene {
    private:
        LookAtCamera camera;
        plane::Plane myPlane;
        skybox::SkyBox mySkyBox;
        maze::Maze myMaze;
    
    public:
        MenuScene(texture::TextureLoader textureLoader);
};