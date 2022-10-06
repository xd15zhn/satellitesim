#include "orbitalsim.hpp"
#include "utils.h"

int main(void) {
    /*初始化场景*/
    Camera camera;
    SetWindowMonitor(1);
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	SetConfigFlags(FLAG_FULLSCREEN_MODE);
    SetTargetFPS(60);
    InitGraph(0, 0, "RayLib-3D");
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
    int cnt = 30;
    int showfps=0, showcpu=0;
    float showspeed;

    /*场景循环*/
    while (!WindowShouldClose()) {
        for (int i = 0; i < 3; i++) {
            satellites[i].Simulate(10);
            ShuttleVelocity[i] = satellites[i].Get_V();
        }
		Update_Camera(&camera);
        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode3D(camera);
                Update_Skybox();
                DrawModel(modelEarth, (Vector3){ 0.0f, 0.0f, 0.0f }, 10.0f, WHITE);
                for (int i = 0; i < 3; i++) {
                    satellites[i].Set_RotationAngle(atan2f(ShuttleVelocity[i].x, -ShuttleVelocity[i].y));
                    satellites[i].Update_Model();
                }
            EndMode3D();
            cnt++;
            if (cnt>=30) {
                cnt = 0;
                showfps = GetFPS();
                showcpu = GetCPUusage();
                showspeed = GetCameraSpeed();
            }
            DrawText(TextFormat("FPS: %2i", showfps), 0, 0, 20, LIME);
            DrawText(TextFormat("CPU: %2i %%", showcpu), 0, 30, 20, LIME);
            DrawText(TextFormat("Camera Speed: %.4f", showspeed), 0, 60, 20, LIME);
        EndDrawing();
    }
    CloseGraph();
    return 0;
}
