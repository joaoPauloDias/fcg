#pragma once

#include <map>
#include <iostream>
#include <memory>
#include <glad/glad.h>

namespace texture
{
    class TextureLoader
    {
    private:
        std::map<std::string, GLuint> textures_;
        GLuint LoadTexture(const char* filename);
        // Número de texturas carregadas pela função LoadTextureImage()
        GLuint g_NumLoadedTextures_;
    public:
        TextureLoader();
        void LoadTexture(std::string texture, const char* filename);
        GLuint* GetTexture(std::string texture);
    };
} 
