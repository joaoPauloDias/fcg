#include "Maze.h"
#include <iostream>
#include <algorithm>
using namespace maze;

Maze::Maze(int n)
    : size(n), g(std::random_device{}())
{
    wall = std::vector<std::vector<bool>>(size, std::vector<bool>(size, true));
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

        std::shuffle(directions.begin(), directions.end(), g);

        for (auto direction : directions)
        {
            int new_i = i + 2 * di[direction];
            int new_j = j + 2 * dj[direction];
            if (new_i > 0 && new_i < size - 1 && new_j > 0 && new_j < size - 1 && wall[new_i][new_j])
            {
                wall[new_i - di[direction]][new_j - dj[direction]] = false;
                wall[new_i][new_j] = false;
                s.push({new_i, new_j});
            }
        }
    }
}

void Maze::display() const
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            std::cout << (wall[i][j] ? '#' : ' ');
        }
        std::cout << std::endl;
    }
}

void Maze::generateBlocks()
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            blocks.push_back({wall[i][j], wall[i][j] ? Matrix_Translate(i * 2.0f, 0.0f, j * 2.0f) : Matrix_Translate(i * 2.0f, -2.0f, j * 2.0f)});
        }
    }
}