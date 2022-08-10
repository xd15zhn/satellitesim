#include <cmath>
#include "orbitalsim.hpp"
#include "camera.h"
#include "skybox.h"

int main(void) {
    /*为每个运动的模型添加仿真器*/
    Orbital_Simulator simSates[3];
    simSates[0].Set_initRV(Vector3{15, 0, 0}, Vector3{0, 20, 0});
    simSates[1].Set_initRV(Vector3{15, -1, 0}, Vector3{0, 20, 0});
    simSates[2].Set_initRV(Vector3{15, 1, 0}, Vector3{0, 20, 0});

    /*初始化场景*/
    Camera camera;
	SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetTargetFPS(60);
    InitGraph(1024, 768, "RayLib-3D");
    // ToggleFullscreen();
	Init_Camera(&camera);
    Init_Skybox();

    /*加载模型和纹理*/
    Model modelEarth = LoadModel("resources/sphere.obj");
    Texture2D imgEarth = LoadTexture("resources/earth.png");
    modelEarth.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = imgEarth;
    Model modelSatellite[3];
    Texture2D imgSatellite = LoadTexture("resources/satellite.png");
    for (int i = 0; i < 3; i++) {
        modelSatellite[i] = LoadModel("resources/satellite.obj");    
        modelSatellite[i].materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = imgSatellite;
    }

    /*添加全局变量*/
    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
    Vector3 ShuttlePosition[3], ShuttleVelocity[3];
    Vector3 vec3d;
    float shx, shy, shz;
    double t;

    /*场景循环*/
    while (!WindowShouldClose()) {
        
        t = GetTime();
        for (int i = 0; i < 3; i++) {
            simSates[i].Simulate(10);
            vec3d = simSates[i].Get_R();
            ShuttlePosition[3].x = vec3d.x;
            ShuttlePosition[3].y = vec3d.z;
            ShuttlePosition[3].z = vec3d.y;
            vec3d = simSates[i].Get_V();
            ShuttleVelocity[3].x = vec3d.x;
            ShuttleVelocity[3].y = vec3d.z;
            ShuttleVelocity[3].z = vec3d.y;
            modelSatellite[i].transform = MatrixRotateXYZ((Vector3){0, atan2f(-ShuttleVelocity[i].x, ShuttleVelocity[i].z), 0});
        }
		Update_Camera(&camera);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode3D(camera);
                Update_Skybox();
                DrawModel(modelEarth, (Vector3){ 0.0f, 0.0f, 0.0f }, 10.0f, WHITE);
                for (int i = 0; i < 3; i++)
                    DrawModel(modelSatellite[i], ShuttlePosition[i], 0.01, WHITE);
            EndMode3D();
            DrawFPS(10, 10);
        EndDrawing();
    }
    CloseGraph();
    return 0;
}