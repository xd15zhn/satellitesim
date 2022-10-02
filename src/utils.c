#include "raylib.h"

Model skybox;

void Init_Skybox() {
    Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
    skybox = LoadModelFromMesh(cube);
    int id, a[1];
    skybox.materials[0].shader = LoadShader("resources/skyboxvs.glsl", "resources/skyboxfs.glsl");
    id = GetShaderLocation(skybox.materials[0].shader, "environmentMap"); a[0] = MATERIAL_MAP_CUBEMAP;
    SetShaderValue(skybox.materials[0].shader, id, a, SHADER_UNIFORM_INT);
    id = GetShaderLocation(skybox.materials[0].shader, "doGamma"); a[0] = 0;
    SetShaderValue(skybox.materials[0].shader, id, a, SHADER_UNIFORM_INT);
    id = GetShaderLocation(skybox.materials[0].shader, "vflipped"); a[0] = 0;
    SetShaderValue(skybox.materials[0].shader, id, a, SHADER_UNIFORM_INT);
    Image imgskybox = LoadImage("resources/skybox.png");
    skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(imgskybox, CUBEMAP_LAYOUT_AUTO_DETECT);
    UnloadImage(imgskybox);
}
void Update_Skybox() {
    BeginBackground();
    DrawModel(skybox, (Vector3){0, 0, 0}, 1.0f, WHITE);
    EndBackground();
}

void DrawFPS(int fps) { DrawText(TextFormat("%2i FPS", fps), 0, 0, 20, LIME); }
void DrawCPUusage(int cpu) { DrawText(TextFormat("%2i %%", cpu), 0, 30, 20, LIME); }