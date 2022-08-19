#include "orbitalsim.hpp"
#include "utils.h"
#include "tracelog.h"

int main(void) {
    /*初始化场景*/
    Camera camera;
    SetTraceLogLevel(LOG_WARNING);
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	SetConfigFlags(FLAG_FULLSCREEN_MODE);
    SetTargetFPS(60);
    // InitGraph(1024, 768, "RayLib-3D");
    InitGraph(1920, 1200, "RayLib-3D");
	Init_Camera(&camera);
    Init_Skybox();

    /*加载卫星*/
    Orbital_Object satellites[3];
    satellites[0].Set_initRV(Vector3{15, 0, 0}, Vector3{0, 20, 0});
    satellites[1].Set_initRV(Vector3{15, 0, 5}, Vector3{0, 21, 0});
    satellites[2].Set_initRV(Vector3{15, 1, -5}, Vector3{0, 20, 0});
    for (int i = 0; i < 3; i++) {
        satellites[i].Set_ID(i);
        satellites[i].Load_Model("resources/satellite.obj", "resources/satellite.png");
        satellites[i].Set_Scale(0.01);
    }

    /*加载地球模型和纹理*/
    Model modelEarth = LoadModel("resources/sphere.obj");
    Texture2D imgEarth = LoadTexture("resources/earth.png");
    modelEarth.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = imgEarth;

    /*添加全局变量*/
    Vector3 ShuttleVelocity[3];
    Vector3 vec3d;
    int cnt = 30;
    int showfps=0, showcpu=0;

    /*场景循环*/
    while (!WindowShouldClose()) {
        for (int i = 0; i < 3; i++) {
            satellites[i].Simulate(10);
            vec3d = satellites[i].Get_V();
            ShuttleVelocity[i].x = vec3d.x;
            ShuttleVelocity[i].y = vec3d.z;
            ShuttleVelocity[i].z = vec3d.y;
        }
		Update_Camera(&camera);
        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode3D(camera);
                Update_Skybox();
                DrawModel(modelEarth, (Vector3){ 0.0f, 0.0f, 0.0f }, 10.0f, WHITE);
                for (int i = 0; i < 3; i++)
                    satellites[i].Update_Model(MatrixRotateXYZ((Vector3){0, atan2f(-ShuttleVelocity[i].x, ShuttleVelocity[i].z), 0}));
            EndMode3D();
            cnt++;
            if (cnt>=30) {
                cnt = 0;
                showfps = GetFPS();
                showcpu = GetCPUusage();
            }
            DrawText(TextFormat("%2i FPS", showfps), 0, 0, 20, LIME);
            DrawText(TextFormat("%2i %%", showcpu), 0, 30, 20, LIME);
        EndDrawing();
    }
    CloseGraph();
    return 0;
}