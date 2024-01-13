#include <string>
#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

// Definimos uma estrutura que armazenará dados necessários para renderizar
// cada objeto da cena virtual.
struct SceneObject
{
    std::string  name;        // Nome do objeto
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