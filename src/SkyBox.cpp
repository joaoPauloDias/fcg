#include "SkyBox.h"
#include "matrices.h"
#include "globals.h"
#include "BezierCurve.h"

using namespace skybox;

#define WINGS_HEIGHT 30

SkyBox::SkyBox(texture::TextureLoader textureLoader, Camera* camera) :
    sphereModel("../../assets/models/sphere.obj"),
    wingsModel("../../assets/models/wings.obj")
{
    this->camera = camera;


    sphereModel.GetPart("the_sphere")->setTextures(textureLoader.GetTexture("skybox_diffuse"), NULL, NULL);
    std::cout << sphereModel.GetPart("the_sphere")->id << std::endl;

    wingsModel.GetPart("wings")->setTextures(textureLoader.GetTexture("wings_diffuse"), NULL, NULL);
}

void SkyBox::Render() {
    sphereModel.ApplyModelMatrix(modelMatrix);
    glDisable(GL_CULL_FACE);
    sphereModel.Draw();
    glEnable(GL_CULL_FACE);

    if (showIcarus) {
        wingsModel.ApplyModelMatrix(Matrix_Translate(wingsPosition.x, wingsPosition.y, wingsPosition.z));
        wingsModel.Draw();
    }
}

void SkyBox::Update(float dt) {
    modelMatrix = Matrix_Translate(camera->position.x, camera->position.y, camera->position.z) * Matrix_Scale(100, 100, 100);

    float r = ((float) rand())/RAND_MAX;
    if (!showIcarus && r < 0.001) {
        showIcarus = true;
    }

    if (showIcarus) {
        BezierCurve wingsPath(
            glm::vec4(-10.0f, -1.0f, -10.0f, 1.0f),
            glm::vec4(-10.0f, WINGS_HEIGHT, -10.0f, 1.0f),
            glm::vec4(30.0f, WINGS_HEIGHT, 30.0f, 1.0f),
            glm::vec4(30.0f, -1.0, 30.0f, 1.0f)
        );

        t += dt;
        if (t > 10) {
            t = 0;
            showIcarus = false;
        }
        wingsPosition = wingsPath.getPosition(t/10);
    }
}
