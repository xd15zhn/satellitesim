#include <cmath>
#include "rcamera.h"
#include "raymath.h"
#include "orbitalsim.hpp"

int main(void) {
    Orbital_Simulator shuffle;
	SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(960, 540, "RayLib-3D");
    SetTargetFPS(60);
    Model model = LoadModel("SpaceShuttle.obj");
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 00.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
	SetCamera(camera);
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
        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode3D(camera);
                DrawGrid(100, 5);
                DrawSphere(cubePosition, 2.0f, ORANGE);
                DrawModel(model, ShuttlePosition, 1.0, BLUE);
            EndMode3D();
            DrawFPS(10, 10);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}