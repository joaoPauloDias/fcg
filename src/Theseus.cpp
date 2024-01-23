#include "Theseus.h"
#include "matrices.h"

using namespace theseus;

Theseus::Theseus(texture::TextureLoader textureLoader, FreeCamera* camera)
    :
      swordModel("../../assets/models/sword.obj"), freeCamera(camera)
{
    swordModel.GetPart("sword")->setTextures(textureLoader.GetTexture("sword_normals"),NULL,NULL);
}


void Theseus::Render() {
    swordModel.ApplyModelMatrix(swordModel.modelMatrix);
    swordModel.Draw();
}

void Theseus::Update(float dt) {
    float r = 0.2f;
    float y = r * sin(freeCamera->phi);
    float z = r * cos(freeCamera->phi) * cos(freeCamera->theta);
    float x = r * cos(freeCamera->phi) * sin(freeCamera->theta);

    glm::vec4 direction = glm::vec4(x, y, z, 0.0f); 
    direction = direction / norm(direction);
    glm::vec4 modelPosition = glm::vec4(freeCamera->position) + direction * r;

    glm::vec4 cameraToModel = (modelPosition - glm::vec4(freeCamera->position)) /  norm((modelPosition - glm::vec4(freeCamera->position)));

    glm::vec4 modelUp = glm::vec4(0, 1, 0, 0); 
    glm::vec4 modelRight = crossproduct(cameraToModel, modelUp);
    modelRight = modelRight / norm(modelRight);

    glm::mat4 rotationMatrix = glm::lookAt(glm::vec3(0), glm::vec3(modelRight.x, modelRight.y, modelRight.z), glm::vec3(modelUp.x, modelUp.y, modelUp.z));

    glm::mat4 modelMatrix = Matrix_Translate(modelPosition.x, modelPosition.y - 0.1, modelPosition.z)
                            * glm::inverse(rotationMatrix)
                            * Matrix_Scale(0.04f, 0.04f, 0.04f);

    swordModel.modelMatrix = modelMatrix;
}