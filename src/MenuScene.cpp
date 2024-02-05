#include "MenuScene.h"

#define MAZE

MenuScene::MenuScene(texture::TextureLoader textureLoader) :
    camera(3.14f, -0.7f, 20.0f),
    mySkyBox(textureLoader, &camera),
    myMaze(textureLoader, 21),
    myPlane(textureLoader, glm::vec4{0.0f, 0.0f, 0.0f, 0.0f}, &camera)
{   
    
    SetCamera(&camera);
    AddObject("skybox", &mySkyBox);
    AddObject("plane", &myPlane);
    AddObject("maze", &myMaze);

};
