#include "Plane.h"
#include "matrices.h"
#include "globals.h"

#define SCALE_FACTOR_X 3.3f * 0.5f
#define SCALE_FACTOR_Z 1.0f * 0.5f

extern bool g_LeftMouseButtonPressed;
using namespace plane;

Plane::Plane(texture::TextureLoader textureLoader, glm::vec4 position, Camera *camera) : model("../../assets/models/plane.obj")
{
    this->position = position;
    this->textureLoader = textureLoader;
    this->camera = camera;
    

    modelMatrix = Matrix_Translate(position.x, position.y, position.z) *
                  Matrix_Scale(SCALE_FACTOR_X, 1.0f, SCALE_FACTOR_Z);

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
    glm::mat4 m = Matrix_Camera_View(camera->position, camera->view_vector, glm::vec4(0, 1, 0, 0));

    modelMatrix = glm::inverse(m) *
                  Matrix_Translate(0, -0.5, -4) *
                  Matrix_Rotate_X(M_PI_2) *
                  Matrix_Scale(SCALE_FACTOR_X, 1.0f, SCALE_FACTOR_Z);

    if (activeScene == MENU_SCENE && activeMenu == MENU && g_LeftMouseButtonPressed) {
        activeScene = GAME_SCENE;
    }else if(activeMenu == GAME_OVER){
        model.GetPart("plane")->setTextures(textureLoader.GetTexture("game_over_diffuse"), NULL, NULL);
    }else if(activeMenu == VICTORY){
        model.GetPart("plane")->setTextures(textureLoader.GetTexture("victory_diffuse"), NULL, NULL);
    }
}