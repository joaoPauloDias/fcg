#include "Theseus.h"

#include "Maze.h"
#include "matrices.h"
#include "globals.h"

using namespace theseus;

Theseus::Theseus(texture::TextureLoader textureLoader, FreeCamera* camera)
    :
      swordModel("../../assets/models/sword.obj"), freeCamera(camera)
{
    swordModel.GetPart("sword")->setTextures(textureLoader.GetTexture("sword_normals"),NULL,NULL);
    position = glm::vec4(2.0f, 2.0f, 2.0f, 1.0f);
}


void Theseus::Render() {
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "illumination"), 1);
    swordModel.ApplyModelMatrix(modelMatrix);
    swordModel.Draw();
}

void Theseus::Update(float dt) {
    // It is important to compute the sword model matrix before updating the position attribute
    // to draw the sword in the correct position
    glm::vec4 swordPosition = position + freeCamera->view_vector * 0.2f;
    modelMatrix = 
        Matrix_Translate(swordPosition.x, swordPosition.y - 0.1f, swordPosition.z) * 
        Matrix_Rotate_Y(freeCamera->theta + M_PI_2) *
        Matrix_Scale(0.04f, 0.04f, 0.04f);

    glm::vec4 newCameraPosition = freeCamera->getNewPosition(dt);
    
    maze::Maze* maze = (maze::Maze*) GetVirtualScene()->GetObject("maze");

    if(!maze->checkCollision(newCameraPosition, 0.5f)) {
        position = newCameraPosition;
        freeCamera->position = newCameraPosition;
    }
}