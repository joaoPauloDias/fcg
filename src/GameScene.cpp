#include "GameScene.h"
#include "globals.h"

extern ActiveScene activeScene;

GameScene::GameScene(texture::TextureLoader textureLoader) :
    camera(3.14f, -0.7f, glm::vec4(1.7f, 0.2f, 1.7f, 1.0f), 5.0f),
    mySkyBox(textureLoader, &camera),
    myMaze(textureLoader, 21),
    myTheseus(textureLoader, &camera),
    myMinotaur(textureLoader, glm::vec4(0, 0, 0, 0))
{    
    std::pair<int, int> minotaurStartPosition = myMaze.getRandomFreePosition();
    std::cout<<minotaurStartPosition.first<<' '<< minotaurStartPosition.second<<std::endl;
    myMinotaur.SetPosition(glm::vec4(2.0f * minotaurStartPosition.first, -1.0f, 2.0f * minotaurStartPosition.second, 1.0f));

    SetCamera(&camera);
    AddObject("skybox", &mySkyBox);
    AddObject("maze", &myMaze);
    AddObject("theseus", &myTheseus);
    AddObject("minotaur", &myMinotaur);
};
