#version 330 core

// Atributos de fragmentos recebidos como entrada ("in") pelo Fragment Shader.
// Neste exemplo, este atributo foi gerado pelo rasterizador como a
// interpolação da posição global e a normal de cada vértice, definidas em
// "shader_vertex.glsl" e "main.cpp".
in vec4 position_world;
in vec4 normal;

// Posição do vértice atual no sistema de coordenadas local do modelo.
in vec4 position_model;

// Coordenadas de textura obtidas do arquivo OBJ (se existirem!)
in vec2 texcoords;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Identificador que define qual objeto está sendo desenhado no momento
#define DIFFUSE 0
#define BLINN_PHONG 1

uniform int object_id;
uniform int illumination;

// Parâmetros da axis-aligned bounding box (AABB) do modelo
uniform vec4 bbox_min;
uniform vec4 bbox_max;

// Variáveis para acesso das imagens de textura
uniform sampler2D TextureDiffuse;
uniform sampler2D TextureSpecular;
uniform sampler2D TextureNormals;

uniform bool useDiffuseTexture;
uniform bool useSpecularTexture;
uniform bool useNormalsTexture;

// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
out vec4 color;

// Constantes
#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

vec3 blinn_phong() {
    // Obtemos a posição da câmera utilizando a inversa da matriz que define o
    // sistema de coordenadas da câmera.
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    // O fragmento atual é coberto por um ponto que percente à superfície de um
    // dos objetos virtuais da cena. Este ponto, p, possui uma posição no
    // sistema de coordenadas global (World coordinates). Esta posição é obtida
    // através da interpolação, feita pelo rasterizador, da posição de cada
    // vértice.
    vec4 p = position_world;

    // Coordenadas de textura U e V
    float U = 0.0;
    float V = 0.0;

    // Coordenadas de textura do plano, obtidas do arquivo OBJ.
    U = texcoords.x;
    V = texcoords.y;

    vec4 n;
    if (useNormalsTexture) {
        n = normalize(texture(TextureNormals, vec2(U,V)).rgba);
    } else {
        n = normalize(normal);
    }
    
    // Normal do fragmento atual, interpolada pelo rasterizador a partir das
    // normais de cada vértice.

    // Vetor que define o sentido da fonte de luz em relação ao ponto atual.
    vec4 l = normalize(vec4(1.0,1.0,0.0,0.0));

    // Vetor que define o sentido da câmera em relação ao ponto atual.
    vec4 v = normalize(camera_position - p);

    // Vetor que define o sentido da reflexão especular ideal.
    vec4 r = -l + 2 * n * dot(n, l);

    // Obtemos a refletância difusa a partir da leitura da imagem TextureImage0
    vec3 Kd;
    if (useDiffuseTexture) {
        Kd = texture(TextureDiffuse, vec2(U,V)).rgb;
    } else {
        Kd = vec3(1.0, 1.0, 1.0);
    }
    vec3 Ks;
    if (useSpecularTexture) {
        Ks = texture(TextureSpecular, vec2(U,V)).rgb;
    } else {
        Ks = vec3(0, 0, 0);
    }
    
    // Equação de Iluminação
    float lambert = max(0,dot(n,l));
    float q = 40;

    return Kd * lambert + Ks * pow(max(0, dot(r, v)), 40) + Kd * 0.3 * lambert;
}

vec3 diffuse() {
    // Obtemos a posição da câmera utilizando a inversa da matriz que define o
    // sistema de coordenadas da câmera.
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    // O fragmento atual é coberto por um ponto que percente à superfície de um
    // dos objetos virtuais da cena. Este ponto, p, possui uma posição no
    // sistema de coordenadas global (World coordinates). Esta posição é obtida
    // através da interpolação, feita pelo rasterizador, da posição de cada
    // vértice.
    vec4 p = position_world;

    // Coordenadas de textura U e V
    float U = 0.0;
    float V = 0.0;

    // Coordenadas de textura do plano, obtidas do arquivo OBJ.
    U = texcoords.x;
    V = texcoords.y;

    vec4 n;
    if (useNormalsTexture) {
        n = normalize(texture(TextureNormals, vec2(U,V)).rgba);
    } else {
        n = normalize(normal);
    }
    
    // Normal do fragmento atual, interpolada pelo rasterizador a partir das
    // normais de cada vértice.

    // Vetor que define o sentido da fonte de luz em relação ao ponto atual.
    vec4 l = normalize(vec4(1.0,1.0,0.0,0.0));

    // Equação de Iluminação
    float lambert = max(0,dot(n,l));

    // Obtemos a refletância difusa a partir da leitura da imagem TextureImage0
    vec3 Kd;
    if (useDiffuseTexture) {
        Kd = texture(TextureDiffuse, vec2(U,V)).rgb;
    } else {
        Kd = vec3(1.0, 1.0, 1.0);
    }

    return Kd * (lambert + 0.3);
}

void main()
{
    if (illumination == DIFFUSE) {
        color.rgb = diffuse();
    } else if (illumination == BLINN_PHONG) {
        color.rgb = blinn_phong();
    }
    color.a = 1;
    color.rgb = pow(color.rgb, vec3(1.0,1.0,1.0)/2.2);
} 

