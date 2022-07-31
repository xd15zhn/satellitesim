#include <cmath>
#include "orbitalsim.hpp"
#include "camera.h"
#include "skybox.h"

int main(void) {
    /*为每个运动的模型添加仿真器*/
    Orbital_Simulator simSatellite;

    /*初始化场景*/
    Camera camera;
	SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetTargetFPS(60);
    InitWindow(960, 540, "RayLib-3D");
    // ToggleFullscreen();
	Init_Camera(&camera);
    Init_Skybox();

    /*加载模型和纹理*/
    Model modelSun = LoadModel("sun.obj");
    Texture2D imgSun = LoadTexture("sun.png");
    modelSun.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = imgSun;
    Model modelSatellite = LoadModel("satellite.obj");
    Texture2D imgSatellite = LoadTexture("satellite.png");
    modelSatellite.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = imgSatellite;

    /*添加全局变量*/
    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
    Vector3 ShuttlePosition, ShuttleVelocity;
    Vector3d vec3d;
    double t;

    /*场景循环*/
    while (!WindowShouldClose()) {
        
        t = GetTime();
        simSatellite.Simulate(10);
        vec3d = simSatellite.Get_R();
        ShuttlePosition.x = vec3d._x;
        ShuttlePosition.y = vec3d._z;
        ShuttlePosition.z = vec3d._y;
        vec3d = simSatellite.Get_V();
        ShuttleVelocity.x = vec3d._x;
        ShuttleVelocity.y = vec3d._z;
        ShuttleVelocity.z = vec3d._y;
        modelSatellite.transform = MatrixRotateXYZ((Vector3){0, atan2f(-ShuttleVelocity.x, ShuttleVelocity.z), 0});
		Update_Camera(&camera);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode3D(camera);
                Update_Skybox();
                DrawModel(modelSun, (Vector3){ 0.0f, 0.0f, 0.0f }, 10.0f, WHITE);
                DrawModel(modelSatellite, ShuttlePosition, 0.01, WHITE);
            EndMode3D();
            DrawFPS(10, 10);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}