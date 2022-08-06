#include <cmath>
#include "orbitalsim.hpp"
#include "camera.h"
#include "skybox.h"

int main(void) {
    /*为每个运动的模型添加仿真器*/
    Orbital_Simulator simSatellite;
    // Orbital_Simulator simEarth;

    /*初始化场景*/
    Camera camera;
	SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetTargetFPS(60);
    InitGraph(960, 540, "RayLib-3D");
    // ToggleFullscreen();
	Init_Camera(&camera);
    Init_Skybox();

    /*加载模型和纹理*/
    // Model modelSun = LoadModel("resources/sphere.obj");
    // Texture2D imgSun = LoadTexture("resources/sun.png");
    // modelSun.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = imgSun;
    Model modelSatellite = LoadModel("resources/satellite.obj");
    Texture2D imgSatellite = LoadTexture("resources/satellite.png");
    modelSatellite.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = imgSatellite;
    Model modelEarth = LoadModel("resources/sphere.obj");
    Texture2D imgEarth = LoadTexture("resources/earth.png");
    modelEarth.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = imgEarth;

    /*添加全局变量*/
    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
    Vector3 ShuttlePosition, ShuttleVelocity;
    Vector3 vec3d;
    float shx, shy, shz;
    double t;

    /*场景循环*/
    while (!WindowShouldClose()) {
        
        t = GetTime();
        simSatellite.Simulate(10);
        vec3d = simSatellite.Get_R();
        ShuttlePosition.x = vec3d.x;
        ShuttlePosition.y = vec3d.z;
        ShuttlePosition.z = vec3d.y;
        vec3d = simSatellite.Get_V();
        ShuttleVelocity.x = vec3d.x;
        ShuttleVelocity.y = vec3d.z;
        ShuttleVelocity.z = vec3d.y;
        modelSatellite.transform = MatrixRotateXYZ((Vector3){0, atan2f(-ShuttleVelocity.x, ShuttleVelocity.z), 0});
		Update_Camera(&camera);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode3D(camera);
                Update_Skybox();
                DrawModel(modelEarth, (Vector3){ 0.0f, 0.0f, 0.0f }, 10.0f, WHITE);
                DrawModel(modelSatellite, ShuttlePosition, 0.01, WHITE);
            EndMode3D();
            DrawFPS(10, 10);
        EndDrawing();
    }
    CloseGraph();
    return 0;
}