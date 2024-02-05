#include "Plane.h"
#include "matrices.h"
#include "globals.h"

#define SCALE_FACTOR_X 3.3f
#define SCALE_FACTOR_Y 1.0f


using namespace plane;

Plane::Plane(texture::TextureLoader textureLoader, glm::vec4 position) : model("../../assets/models/plane.obj")
{
    this->position = position;
    this->textureLoader = textureLoader;
    

    modelMatrix = Matrix_Translate(position.x, position.y, position.z) *
                  Matrix_Scale(SCALE_FACTOR_X, SCALE_FACTOR_Y, 1.0f);

    model.GetPart("plane")->setTextures(textureLoader.GetTexture("plane_diffuse"), NULL, NULL);
}

void Plane::Render(){
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "illumination"), ILLUMINATION_BLINN_PHONG);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "interpolation"), INTERPOLATION_PHONG);
    model.ApplyModelMatrix(modelMatrix);
    glDisable(GL_CULL_FACE);
    model.Draw();
    glEnable(GL_CULL_FACE);
}

void Plane::Update(float dt){
    
}