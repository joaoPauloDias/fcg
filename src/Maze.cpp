#include "Maze.h"
#include <iostream>
#include <algorithm>
using namespace maze;

Maze::Maze(texture::TextureLoader textureLoader, int n)
    : size_(n), randomGenerator_(std::random_device{}()), 
      wallModel("../../assets/models/wall.obj"), 
      groundModel("../../assets/models/ground.obj")
{
    wallModel.GetPart("Wall")->setTextures(textureLoader.GetTexture("wall_normals"), NULL, textureLoader.GetTexture("wall_normals"));
    groundModel.GetPart("Ground")->setTextures(textureLoader.GetTexture("ground_normals"), NULL, textureLoader.GetTexture("ground_normals"));

    walls_ = std::vector<std::vector<bool>>(size_, std::vector<bool>(size_, true));
    generateDisplay();
    display();
    generateBlocks();
}

void Maze::generateDisplay()
{
    int di[] = {-1, 1, 0, 0};
    int dj[] = {0, 0, -1, 1};
    std::vector<int> directions = {0, 1, 2, 3};
    std::stack<std::pair<int, int>> s;
    s.push({1, 1});

    while (!s.empty())
    {
        auto [i, j] = s.top();
        s.pop();

        std::shuffle(directions.begin(), directions.end(), randomGenerator_);

        for (auto direction : directions)
        {
            int new_i = i + 2 * di[direction];
            int new_j = j + 2 * dj[direction];
            if (new_i > 0 && new_i < size_ - 1 && new_j > 0 && new_j < size_ - 1 && walls_[new_i][new_j])
            {
                walls_[new_i - di[direction]][new_j - dj[direction]] = false;
                walls_[new_i][new_j] = false;
                s.push({new_i, new_j});
            }
        }
    }
}

void Maze::display() const
{
    for (int i = 0; i < size_; i++)
    {
        for (int j = 0; j < size_; j++)
        {
            std::cout << (walls_[i][j] ? '#' : ' ');
        }
        std::cout << std::endl;
    }
}

void Maze::generateBlocks()
{
    for (int i = 0; i < size_; ++i)
    {
        for (int j = 0; j < size_; ++j)
        {
            blockMatrices_.push_back({walls_[i][j], walls_[i][j] ? Matrix_Translate(i * 2.0f, 0.0f, j * 2.0f) : Matrix_Translate(i * 2.0f, -2.0f, j * 2.0f)});
        }
    }
}

void Maze::Render() {
    for (auto &&[isWall, m] : getBlockMatrices()) {
        if (isWall) {
            wallModel.ApplyModelMatrix(m);
            wallModel.Draw();
        } else {
            groundModel.ApplyModelMatrix(m);
            groundModel.Draw();
        }
    }
}

void Maze::Update(float dt) {
    std::cout << "Updating maze\n";
}