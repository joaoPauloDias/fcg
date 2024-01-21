#pragma once

#include <vector>
#include <string>

#include <tiny_obj_loader.h>
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

// Definimos uma estrutura que armazenará dados necessários para renderizar
// cada parte de um modelo da cena virtual
struct ModelPart {
    std::string  name;        // Nome da parte
    size_t       first_index; // Índice do primeiro vértice dentro do vetor indices[] definido em BuildTrianglesAndAddToVirtualScene()
    size_t       num_indices; // Número de índices do objeto dentro do vetor indices[] definido em BuildTrianglesAndAddToVirtualScene()
    GLenum       rendering_mode; // Modo de rasterização (GL_TRIANGLES, GL_TRIANGLE_STRIP, etc.)
    GLuint       vertex_array_object_id; // ID do VAO onde estão armazenados os atributos do modelo
    glm::vec3    bbox_min; // Axis-Aligned Bounding Box do objeto
    glm::vec3    bbox_max;
    GLuint       texture_diffuse;
    GLuint       texture_specular;
    GLuint       texture_normals;

    bool useDiffuseTexture = false;
    bool useSpecularTexture = false;
    bool useNormalsTexture = false;
 
    glm::mat4x4 model_matrix;
    int id; 
    void Draw();
    void ApplyModelMatrix(glm::mat4x4 model_matrix);
    void setTextures(GLuint* diffuse, GLuint* specular, GLuint* normals);
};

// Estrutura que representa um modelo geométrico carregado a partir de um
// arquivo ".obj". Veja https://en.wikipedia.org/wiki/Wavefront_.obj_file .
struct ObjModel {
    int id;

    tinyobj::attrib_t                 attrib;
    std::vector<tinyobj::shape_t>     shapes;
    std::vector<tinyobj::material_t>  materials;
    
    std::map<std::string, ModelPart>  parts;
    glm::mat4 modelMatrix;

    // Este construtor lê o modelo de um arquivo utilizando a biblioteca tinyobjloader.
    // Veja: https://github.com/syoyo/tinyobjloader
    ObjModel(const char* filename, const char* basepath = NULL, bool triangulate = true);
    void ComputeNormals();
    void BuildTriangles();
    void PrintObjModelInfo();
    void ApplyModelMatrix(glm::mat4x4 model_matrix);
    void Draw();
    ModelPart* GetPart(std::string name);
};
