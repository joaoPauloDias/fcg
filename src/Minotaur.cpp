#include "Minotaur.h"
#include "matrices.h"
#include "glad/glad.h"
#include "globals.h"
#include "TextureLoader.h"

using namespace minotaur;

#define SCALE_FACTOR 0.018f

Minotaur::Minotaur(texture::TextureLoader textureLoader, glm::vec4 position) : 
    model("../../assets/models/minotaur.obj") {
    this->position = position;
    health = 5;

    modelMatrix = Matrix_Translate(position.x, position.y, position.z) *
                  Matrix_Scale(SCALE_FACTOR, SCALE_FACTOR, SCALE_FACTOR);

    model.GetPart("Pants_mesh")->setTextures(
        textureLoader.GetTexture("pants_diffuse"), 
        textureLoader.GetTexture("pants_specular"), 
        textureLoader.GetTexture("pants_normals")
    );

    model.GetPart("Body_mesh")->setTextures(
        textureLoader.GetTexture("minotaur_diffuse"), 
        textureLoader.GetTexture("minotaur_specular"), 
        textureLoader.GetTexture("minotaur_normals")
    );

    model.GetPart("Body_teeth")->setTextures(
        textureLoader.GetTexture("minotaur_diffuse"), 
        textureLoader.GetTexture("minotaur_specular"), 
        textureLoader.GetTexture("minotaur_normals")
    );

    model.GetPart("Body_eyes")->setTextures(
        textureLoader.GetTexture("minotaur_diffuse"), 
        textureLoader.GetTexture("minotaur_specular"), 
        textureLoader.GetTexture("minotaur_normals")
    );
}

Cylinder Minotaur::getHitbox() {
    hitBox.center = this->position + glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    hitBox.height = 1.8f;
    hitBox.radius = 0.4f;
    return hitBox;
}

void Minotaur::Update(float dt) {
    if (health <= 0) {
        std::cout << "Minotauro morreu\n" << std::endl;
        GetVirtualScene()->RemoveObject("minotaur");
    }
}

void Minotaur::Render() {
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "illumination"), ILLUMINATION_BLINN_PHONG);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "interpolation"), INTERPOLATION_PHONG);
    model.ApplyModelMatrix(modelMatrix);
    glDisable(GL_CULL_FACE);
    model.Draw();
    glEnable(GL_CULL_FACE);
}

void Minotaur::ReceiveHit(int damage) {
    health -= damage;
}
