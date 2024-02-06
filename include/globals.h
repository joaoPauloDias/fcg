#pragma once
#include <glad/glad.h>

enum ActiveScene {
    MENU_SCENE,
    GAME_SCENE,
};

enum ActiveMenu {
    MENU,
    GAME_OVER,
    VICTORY
};

extern GLuint g_GpuProgramID;
extern GLint g_model_uniform;
extern GLint g_view_uniform;
extern GLint g_projection_uniform;
extern GLint g_object_id_uniform;
extern GLint g_bbox_min_uniform;
extern GLint g_bbox_max_uniform;
extern ActiveScene activeScene;
extern ActiveMenu activeMenu;


