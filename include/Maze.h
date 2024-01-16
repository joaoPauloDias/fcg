#pragma once

#include "matrices.h"
#include <vector> 
#include <stack>
#include <random>



namespace maze{
    class Maze
    {
    private:
        std::vector<std::vector<bool>> wall;
        std::mt19937 g;
    public:
        std::vector<std::pair<bool, glm::mat4>> blocks;
        int size;
        Maze(int n);
        void generateDisplay();
        void generateBlocks();
        void display() const;
    };
}