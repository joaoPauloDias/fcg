#include "Plane.h"
#include "matrices.h"
#include "globals.h"

#define SCALE_FACTOR_X 3.3f * 0.5f
#define SCALE_FACTOR_Z 1.0f * 0.5f

extern bool g_LeftMouseButtonPressed;
using namespace plane;

Plane::Plane(texture::TextureLoader textureLoader, glm::vec4 position, Camera *camera) : 
        model("../../assets/models/plane.obj"),
        laugh(SoundPlayer::initSound("../../assets/audio/laugh.mp3", false, 1.0)),
        win(SoundPlayer::initSound("../../assets/audio/congratulations.mp3", false, 1.0))
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
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "applyInverseView"), true);
    model.ApplyModelMatrix(modelMatrix);
    glDisable(GL_CULL_FACE);
    model.Draw();
    glEnable(GL_CULL_FACE);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "applyInverseView"), false);
}

void Plane::Update(float dt){
    modelMatrix = Matrix_Translate(0, -0.5, -4) *
                  Matrix_Rotate_X(M_PI_2) *
                  Matrix_Scale(SCALE_FACTOR_X, 1.0f, SCALE_FACTOR_Z);

    if (activeScene == MENU_SCENE && activeMenu == MENU && g_LeftMouseButtonPressed) {
        activeScene = GAME_SCENE;
    }else if(activeMenu == GAME_OVER){
        if (!SoundPlayer::isPlaying(this->laugh))
            SoundPlayer::playSound(this->laugh);
        model.GetPart("plane")->setTextures(textureLoader.GetTexture("game_over_diffuse"), NULL, NULL);
    }else if(activeMenu == VICTORY){
        if (!SoundPlayer::isPlaying(this->win))
            SoundPlayer::playSound(this->win);
        model.GetPart("plane")->setTextures(textureLoader.GetTexture("victory_diffuse"), NULL, NULL);
    }
}