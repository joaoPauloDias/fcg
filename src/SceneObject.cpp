#include "SceneObject.h"

#include <globals.h>
#include <glm/gtc/type_ptr.hpp>


void SceneObject::Draw() {
    glUniform1i(glGetUniformLocation(g_GpuProgramID, "useNormalsTexture"), useNormalsTexture);
    if (useNormalsTexture) {
        glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureNormals"), texture_normals);
    }

    glUniform1i(glGetUniformLocation(g_GpuProgramID, "useDiffuseTexture"), useDiffuseTexture);
    if (useDiffuseTexture) {
        glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureDiffuse"), texture_diffuse);
    }

    glUniform1i(glGetUniformLocation(g_GpuProgramID, "useSpecularTexture"), useSpecularTexture);
    if (useSpecularTexture) {
        glUniform1i(glGetUniformLocation(g_GpuProgramID, "TextureSpecular"), texture_specular);
    }

    // "Ligamos" o VAO. Informamos que queremos utilizar os atributos de
    // vértices apontados pelo VAO criado pela função BuildTrianglesAndAddToVirtualScene(). Veja
    // comentários detalhados dentro da definição de BuildTrianglesAndAddToVirtualScene().
    glBindVertexArray(vertex_array_object_id);

    // Setamos as variáveis "bbox_min" e "bbox_max" do fragment shader
    // com os parâmetros da axis-aligned bounding box (AABB) do modelo.
    glUniform4f(g_bbox_min_uniform, bbox_min.x, bbox_min.y, bbox_min.z, 1.0f);
    glUniform4f(g_bbox_max_uniform, bbox_max.x, bbox_max.y, bbox_max.z, 1.0f);

    // Pedimos para a GPU rasterizar os vértices dos eixos XYZ
    // apontados pelo VAO como linhas. Veja a definição de
    // g_VirtualScene[""] dentro da função BuildTrianglesAndAddToVirtualScene(), e veja
    // a documentação da função glDrawElements() em
    // http://docs.gl/gl3/glDrawElements.
    glDrawElements(
        rendering_mode,
        num_indices,
        GL_UNSIGNED_INT,
        (void*)(first_index * sizeof(GLuint))
    );

    // "Desligamos" o VAO, evitando assim que operações posteriores venham a
    // alterar o mesmo. Isso evita bugs.
    glBindVertexArray(0);
}

void SceneObject::ApplyModelMatrix(glm::mat4x4 model_matrix) {
    glUniformMatrix4fv(g_model_uniform, 1 , GL_FALSE , glm::value_ptr(model_matrix));
    glUniform1i(g_object_id_uniform, id);
}

void SceneObject::setTextures(GLuint* diffuse, GLuint* specular, GLuint* normals) {
    if (diffuse != NULL) {
        texture_diffuse = *diffuse;
        useDiffuseTexture = true;
    } else {
        useDiffuseTexture = false;
    }

    if (specular != NULL) {
        texture_specular = *specular;
        useSpecularTexture = true;
    } else {
        useSpecularTexture = false;
    }

    if (normals != NULL) {
        texture_normals = *normals;
        useNormalsTexture = true;
    } else {
        useNormalsTexture = false;
    }
}