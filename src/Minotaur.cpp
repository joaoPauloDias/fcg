#include "Minotaur.h"
#include "matrices.h"
#include "glad/glad.h"
#include "globals.h"
#include <algorithm>
#include "TextureLoader.h"
#include "Maze.h"
#include "Theseus.h"

using namespace minotaur;

#define SCALE_FACTOR 0.018f

Minotaur::Minotaur(texture::TextureLoader textureLoader, glm::vec4 position) : model("../../assets/models/minotaur.obj")
{
    this->position = position;
    health = 5;

    modelMatrix = Matrix_Translate(position.x, position.y, position.z) *
                  Matrix_Scale(SCALE_FACTOR, SCALE_FACTOR, SCALE_FACTOR);

    model.GetPart("Pants_mesh")->setTextures(textureLoader.GetTexture("pants_diffuse"), textureLoader.GetTexture("pants_specular"), textureLoader.GetTexture("pants_normals"));

    model.GetPart("Body_mesh")->setTextures(textureLoader.GetTexture("minotaur_diffuse"), textureLoader.GetTexture("minotaur_specular"), textureLoader.GetTexture("minotaur_normals"));

    model.GetPart("Body_teeth")->setTextures(textureLoader.GetTexture("minotaur_diffuse"), textureLoader.GetTexture("minotaur_specular"), textureLoader.GetTexture("minotaur_normals"));

    model.GetPart("Body_eyes")->setTextures(textureLoader.GetTexture("minotaur_diffuse"), textureLoader.GetTexture("minotaur_specular"), textureLoader.GetTexture("minotaur_normals"));
}

Cylinder Minotaur::getHitbox()
{
    hitBox.center = this->position + glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
    hitBox.height = 1.8f;
    hitBox.radius = 0.4f;
    return hitBox;
}

void Minotaur::Update(float dt)
{
    if (health <= 0)
    {
        std::cout << "Minotauro morreu\n"
                  << std::endl;
        GetVirtualScene()->RemoveObject("minotaur");
    }
    auto [i, j] = GetNextPosition();
    position.x += i*dt;
    position.z += j*dt;
    modelMatrix = Matrix_Translate(position.x, position.y, position.z) *
                  Matrix_Scale(SCALE_FACTOR, SCALE_FACTOR, SCALE_FACTOR);

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

void Minotaur::ReceiveHit(int damage)
{
    health -= damage;
}

std::pair<int, int> Minotaur::GetNextPosition()
{
    maze::Maze *maze = dynamic_cast<maze::Maze *>(GetVirtualScene()->GetObject("maze"));
    std::pair<int, int> theseusPosition = maze->getMazeIndex(dynamic_cast<theseus::Theseus *>(GetVirtualScene()->GetObject("theseus"))->getPosition());
    std::pair<int, int> minotaurPosition = maze->getMazeIndex(position);
    std::vector<std::vector<bool>> walls = maze->getWalls();
    std::vector<std::vector<std::pair<int, int>>> visited(maze->size_, std::vector<std::pair<int, int>>(maze->size_, {-1, -1}));

    int di[] = {-1, 1, 0, 0, 0};
    int dj[] = {0, 0, -1, 1, 0};
    std::vector<int> directions = {0, 1, 2, 3};
    std::vector<std::string> directionsNames = {"OESTE", "LESTE", "SUL", "NORTE"};
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

    // std::vector<std::vector<bool>> isPath(maze->size_, std::vector<bool>(maze->size_, false));

    std::pair<int, int> atual = theseusPosition;
    std::vector<std::pair<int, int>> path;
    path.push_back(atual);
    while ((visited[atual.first][atual.second].first != 0 && visited[atual.first][atual.second].second != 0))
    {
        // isPath[atual.first][atual.second] = true;
        atual = visited[atual.first][atual.second];
        path.push_back(atual);

    }

    if(path.size()>1)atual = path[path.size()-2];
    //return atual;
    

    // bool move = false;
    directions.push_back(4);
    for (auto direction : directions)
    {
        if (minotaurPosition.first + di[direction] == atual.first && minotaurPosition.second + dj[direction] == atual.second){
            return {di[direction], dj[direction]};
        }
    }
    // if(!move)std::cout<<"HIT"<<std::endl;


    // for (int i = 0; i < maze->size_; i++)
    // {
    //     for (int j = 0; j < maze->size_; j++)
    //     {
    //         char cell;
    //         if(walls[i][j])
    //             cell = '#';
    //         else if(isPath[i][j])
    //             cell = 'x';
    //         else
    //             cell = '.';
    //         std::cout << cell;
    //     }
    //     std::cout << std::endl;
    // }
}
