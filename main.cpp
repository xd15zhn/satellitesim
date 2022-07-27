#include <cmath>
#include "rcamera.h"
#include "rlgl.h"
#include "orbitalsim.hpp"

int main(void) {
    Orbital_Simulator shuffle;
	SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(960, 540, "RayLib-3D");
    SetTargetFPS(60);

    Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
    Model skybox = LoadModelFromMesh(cube);
    int id, a[1];
    skybox.materials[0].shader = LoadShader("skyboxvs.glsl", "skyboxfs.glsl");
    id = GetShaderLocation(skybox.materials[0].shader, "environmentMap"); a[0] = MATERIAL_MAP_CUBEMAP;
    SetShaderValue(skybox.materials[0].shader, id, a, SHADER_UNIFORM_INT);
    id = GetShaderLocation(skybox.materials[0].shader, "doGamma"); a[0] = 0;
    SetShaderValue(skybox.materials[0].shader, id, a, SHADER_UNIFORM_INT);
    id = GetShaderLocation(skybox.materials[0].shader, "vflipped"); a[0] = 0;
    SetShaderValue(skybox.materials[0].shader, id, a, SHADER_UNIFORM_INT);
    Shader shdrCubemap = LoadShader("cubemapvs.glsl", "cubemapfs.glsl");
    id = GetShaderLocation(shdrCubemap, "equirectangularMap"); a[0] = 0;
    SetShaderValue(shdrCubemap, id, a, SHADER_UNIFORM_INT);
    char skyboxFileName[256] = { 0 };
    Texture2D panorama;
    Image img = LoadImage("skybox.png");
    skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(img, CUBEMAP_LAYOUT_AUTO_DETECT);    // CUBEMAP_LAYOUT_PANORAMA
    UnloadImage(img);

    Model model = LoadModel("SpaceShuttle.obj");
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 10.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
	SetCamera(&camera);
    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
    Vector3 ShuttlePosition, ShuttleVelocity;
    Vector3d vec3d;
    double t;
    // ToggleFullscreen();
    while (!WindowShouldClose()) {
        t = GetTime();
        shuffle.Simulate(10);
        vec3d = shuffle.Get_R();
        ShuttlePosition.x = vec3d._x;
        ShuttlePosition.y = vec3d._z;
        ShuttlePosition.z = vec3d._y;
        vec3d = shuffle.Get_V();
        ShuttleVelocity.x = vec3d._x;
        ShuttleVelocity.y = vec3d._z;
        ShuttleVelocity.z = vec3d._y;
        model.transform = MatrixRotateXYZ((Vector3){0, atan2f(-ShuttleVelocity.x, ShuttleVelocity.z), 0});
		UpdateCamera(&camera);

        if (IsFileDropped()) {
            int count = 0;
            char **droppedFiles = GetDroppedFiles(&count);
            if (count == 1) {
                if (IsFileExtension(droppedFiles[0], ".png;.jpg;.hdr;.bmp;.tga")) {
                    UnloadTexture(skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);
                    Image img = LoadImage(droppedFiles[0]);
                    skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(img, CUBEMAP_LAYOUT_AUTO_DETECT);
                    UnloadImage(img);
                    TextCopy(skyboxFileName, droppedFiles[0]);
                }
            }
            ClearDroppedFiles();
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode3D(camera);
                DrawGrid(100, 5);
                DrawSphere(cubePosition, 2.0f, ORANGE);
                DrawModel(model, ShuttlePosition, 1.0, BLUE);

                rlDisableBackfaceCulling();
                rlDisableDepthMask();
                DrawModel(skybox, (Vector3){0, 0, 0}, 1.0f, WHITE);
                rlEnableBackfaceCulling();
                rlEnableDepthMask();

            EndMode3D();
            DrawFPS(10, 10);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}