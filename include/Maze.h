#pragma once

#include "GameObject.h"
#include "ObjModel.h"
#include "TextureLoader.h"
#include "matrices.h"
#include <vector> 
#include <stack>
#include <random>



namespace maze{
    class Maze : public GameObject {
    private:
        std::vector<std::pair<bool, glm::mat4>> blockMatrices_;
        std::vector<std::vector<bool>> walls_;
        std::mt19937 randomGenerator_;
        int size_;
        ObjModel wallModel;
        ObjModel groundModel;
    public:
        Maze(texture::TextureLoader textureLoader, int n);
        void generateDisplay();
        void generateBlocks();
        void display() const;
        const std::vector<std::pair<bool, glm::mat4>>& getBlockMatrices() const {
            return blockMatrices_;
        }
        const ObjModel& getModel() const {
            return wallModel;
        }
        void Render();
        void Update(float dt);
        bool checkCollision(glm::vec4 position, float radius);
    };
}