#include "Minotaur.h"
#include "matrices.h"
#include "glad/glad.h"
#include "globals.h"
#include <algorithm>
#include "TextureLoader.h"
#include "Maze.h"
#include "Theseus.h"
#include "collisions.h"


using namespace minotaur;

#define SCALE_FACTOR 0.018f

int auxCount = 0;

Minotaur::Minotaur(texture::TextureLoader textureLoader, glm::vec4 position) : model("../../assets/models/minotaur.obj")
{
    this->position = position;
    this->textureLoader = textureLoader;
    health = 5;
    velocity = 2.0f;
    randomGenerator_ = std::default_random_engine{};

    modelMatrix = Matrix_Translate(position.x, position.y, position.z) *
                  Matrix_Scale(SCALE_FACTOR, SCALE_FACTOR, SCALE_FACTOR);

    model.GetPart("Pants_mesh")->setTextures(textureLoader.GetTexture("pants_diffuse"), textureLoader.GetTexture("pants_specular"), textureLoader.GetTexture("pants_normals"));

    model.GetPart("Body_mesh")->setTextures(textureLoader.GetTexture("minotaur_diffuse"), textureLoader.GetTexture("minotaur_specular"), textureLoader.GetTexture("minotaur_normals"));

    model.GetPart("Body_teeth")->setTextures(textureLoader.GetTexture("minotaur_diffuse"), textureLoader.GetTexture("minotaur_specular"), textureLoader.GetTexture("minotaur_normals"));

    model.GetPart("Body_eyes")->setTextures(textureLoader.GetTexture("minotaur_diffuse"), textureLoader.GetTexture("minotaur_specular"), textureLoader.GetTexture("minotaur_normals"));

    hitBox.height = 1.8f;
    hitBox.radius = 0.6f;
}

collisions::Cylinder Minotaur::getHitbox()
{
    hitBox.center = this->position + glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    return hitBox;
}

void Minotaur::Update(float dt)
{
    if (health <= 0)
    {
        std::cout << "Minotauro morreu\n"
                  << std::endl;
        GetVirtualScene()->RemoveObject("minotaur");
        activeScene = MENU_SCENE;
        activeMenu = VICTORY;
        return;
    }

    if (damageAnimationTime < 1) {
        damageAnimationTime += dt;
    } else {
        damageAnimation = false;
        damageAnimationTime = 0;
        model.GetPart("Body_mesh")->setTextures(textureLoader.GetTexture("minotaur_diffuse"), textureLoader.GetTexture("minotaur_specular"), textureLoader.GetTexture("minotaur_normals"));
    }

    float pos_x = position.x / 2.0f;
    float pos_z = position.z / 2.0f;
    int grid_x = std::round(pos_x);
    int grid_z = std::round(pos_z);

    // Warning: Must use an abs function that supports floating point numbers
    bool center_x = std::abs(pos_x - grid_x) <= 1e-2;
    bool center_z = std::abs(pos_z - grid_z) <= 1e-2;

    if (center_x && center_z) {
        nextDirection = GetNextPosition();
    }

    // Orientation towards Theseus
    theseus::Theseus* theseus = dynamic_cast<theseus::Theseus*>(GetVirtualScene()->GetObject("theseus"));
    glm::vec4 theseusPosition = theseus->getPosition();
    glm::vec4 directionToTheseus = (theseusPosition - position)/norm(theseusPosition - position);
    float angleToTheseus = atan2(directionToTheseus.x, directionToTheseus.z);

    // collision with theseus
    if(!collisions::checkCollision(getHitbox(), theseus->getHitBox())){
        position.x += nextDirection.first*dt*velocity;
        position.z += nextDirection.second*dt*velocity;
    }else{
        theseus->GetHit(attackDamage);
        std::cout<<"MINOHIT "<< auxCount++<<std::endl;
    }


    modelMatrix = Matrix_Translate(position.x, position.y, position.z) *
                  Matrix_Rotate_Y(angleToTheseus) *
                  Matrix_Scale(SCALE_FACTOR, SCALE_FACTOR, SCALE_FACTOR);
    
    if (damageAnimation) {
        modelMatrix = Matrix_Translate(0.0f, 0.1f * damageAnimationTime, 0.0f) * modelMatrix;
    }

}

void Minotaur::Render()
{
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "illumination"), ILLUMINATION_BLINN_PHONG);
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "interpolation"), INTERPOLATION_PHONG);
    model.ApplyModelMatrix(modelMatrix);
    glDisable(GL_CULL_FACE);
    model.Draw();
    glEnable(GL_CULL_FACE);
}

void Minotaur::GetHit(int damage)
{
    health -= damage;
    damageAnimation = true;
    model.GetPart("Body_mesh")->setTextures(
        textureLoader.GetTexture("minotaur_damage"), NULL, NULL);
    std::cout << "ouch\n";
}

std::pair<int, int> Minotaur::GetNextPosition()
{
    maze::Maze *maze = dynamic_cast<maze::Maze *>(GetVirtualScene()->GetObject("maze"));
    std::pair<int, int> theseusPosition = maze->getMazeIndex(dynamic_cast<theseus::Theseus *>(GetVirtualScene()->GetObject("theseus"))->getPosition());
    std::pair<int, int> minotaurPosition = maze->getMazeIndex(position);
    std::vector<std::vector<bool>> walls = maze->getWalls();
    std::vector<std::vector<std::pair<int, int>>> visited(maze->size_, std::vector<std::pair<int, int>>(maze->size_, {-1, -1}));

    int di[] = {-1, 1, 0, 0};
    int dj[] = {0, 0, -1, 1};
    std::vector<int> directions = {0, 1, 2, 3};
    std::stack<std::pair<int, int>> s;
    s.push(minotaurPosition);
    visited[minotaurPosition.first][minotaurPosition.second] = {0, 0};

    while (!s.empty())
    {
        auto [i, j] = s.top();
        if (i == theseusPosition.first && j == theseusPosition.second)
        {
            break;
        }
        s.pop();
        for (auto direction : directions)
        {
            int new_i = i + di[direction];
            int new_j = j + dj[direction];
            if (!walls[new_i][new_j] && visited[new_i][new_j].first == -1)
            {
                visited[new_i][new_j] = {i, j};
                s.push({new_i, new_j});
            }
        }
    }

    std::pair<int, int> atual = theseusPosition;
    std::vector<std::pair<int, int>> path;
    path.push_back(atual);
    while ((visited[atual.first][atual.second].first != 0 && visited[atual.first][atual.second].second != 0))
    {
        atual = visited[atual.first][atual.second];

        // It is impossible to reach theseus
        if (atual.first == -1) {
            return {0, 0};
        }
        path.push_back(atual);

    }


    if(path.size()>1)atual = path[path.size()-2];
    
    if(abs(theseusPosition.first - minotaurPosition.first) + abs(theseusPosition.second - minotaurPosition.second) <= maxDistanceDetectable){
        for (auto direction : directions)
        {
            if (minotaurPosition.first + di[direction] == atual.first && minotaurPosition.second + dj[direction] == atual.second){
                return {di[direction], dj[direction]};
            }
        }
    }else{
        std::vector<int> newDirection = {0, 1, 2, 3};
        std::shuffle(std::begin(newDirection), std::end(newDirection), randomGenerator_);
        for (auto direction : newDirection)
        {
            if (!walls[minotaurPosition.first + di[direction]][minotaurPosition.second + dj[direction]]){
                return {di[direction], dj[direction]};
            }
        }
    }

    return {0, 0};
}

void Minotaur::SetPosition(glm::vec4 position) {
    this->position = position;
}