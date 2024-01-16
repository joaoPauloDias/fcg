#pragma once

#include "matrices.h"
#include <vector> 
#include <stack>
#include <random>



namespace maze{
    class Maze
    {
    private:
        std::vector<std::pair<bool, glm::mat4>> blockMatrices_;
        std::vector<std::vector<bool>> walls_;
        std::mt19937 randomGenerator_;
        int size_;
    public:
        Maze(int n);
        void generateDisplay();
        void generateBlocks();
        void display() const;
        const std::vector<std::pair<bool, glm::mat4>>& getBlockMatrices() const {
            return blockMatrices_;
        }
    };
}