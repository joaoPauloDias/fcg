#pragma once

#include "GameObject.h"
#include "ObjModel.h"
#include "TextureLoader.h"
#include "matrices.h"
#include "collisions.h"
#include <vector> 
#include <stack>
#include <random>



namespace maze{
    class Maze : public GameObject {
    private:
        std::vector<std::pair<bool, glm::mat4>> blockMatrices_;
        std::vector<std::vector<bool>> walls_;
        std::mt19937 randomGenerator_;
        ObjModel wallModel;
        ObjModel groundModel;
    public:
        int size_;
        Maze(texture::TextureLoader textureLoader, int n);
        void generateDisplay();
        void generateBlocks();
        void display() const;
        std::pair<int, int> getMazeIndex(glm::vec4 position);
        const std::vector<std::pair<bool, glm::mat4>>& getBlockMatrices() const {
            return blockMatrices_;
        }
        const  std::vector<std::vector<bool>>& getWalls() const {
            return walls_;
        }
        const ObjModel& getModel() const {
            return wallModel;
        }
        std::pair<int, int> getRandomFreePosition();
        void Render();
        void Update(float dt);
        bool checkCollision(const collisions::Sphere& sphere);
    };
}