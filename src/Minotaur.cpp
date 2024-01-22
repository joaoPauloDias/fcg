#include "Minotaur.h"

#include "matrices.h"

Minotaur::Minotaur(texture::TextureLoader textureLoader) :
    model("../../assets/models/minotaur.obj") 
{
    modelMatrix = Matrix_Translate(0.0f, 2.0f, 0.0f) * 
                  Matrix_Scale(0.02f, 0.02f, 0.02f);

    model.GetPart("Eyes_mesh")->setTextures(
        textureLoader.GetTexture("minotaur_diffuse"),
        textureLoader.GetTexture("minotaur_specular"),
        textureLoader.GetTexture("minotaur_normals")
    );
    model.GetPart("Body_mesh")->setTextures(
        textureLoader.GetTexture("minotaur_diffuse"),
        textureLoader.GetTexture("minotaur_specular"),
        textureLoader.GetTexture("minotaur_normals")
    );
    model.GetPart("Teeth_mesh")->setTextures(
        textureLoader.GetTexture("minotaur_diffuse"),
        textureLoader.GetTexture("minotaur_specular"),
        textureLoader.GetTexture("minotaur_normals")
    );
    model.GetPart("Pants_mesh")->setTextures(
        textureLoader.GetTexture("pants_diffuse"),
        textureLoader.GetTexture("pants_specular"),
        textureLoader.GetTexture("pants_normals")
    );
}

void Minotaur::Render() {
    glDisable(GL_CULL_FACE);
    model.ApplyModelMatrix(modelMatrix);
    model.Draw();
    glEnable(GL_CULL_FACE);
}

void Minotaur::Update(float dt) {
    modelMatrix = Matrix_Rotate_Y(3.141592f * dt) * modelMatrix;
}