#include "raylib.h"
#include <stdio.h>
#include <math.h>

#define VAL_LIMIT(x, min, max)           (((x)<=(min) ? (min) : ((x)>=(max) ? (max) : (x))))
#define MouseMoveSensitivity             0.003f
#define MouseScrolSensitivity            1.5f
#define CameraMoveExponential            1.2f

typedef enum {
    MOVE_FRONT = 0,
    MOVE_BACK,
    MOVE_RIGHT,
    MOVE_LEFT,
    MOVE_UP,
    MOVE_DOWN
} KeyMoves;

Vector2 angle;
Vector2 previousMousePosition;
const int moveControl[6] = { 'W', 'S', 'D', 'A', 'E', 'Q' };
float CameraMoveSensitivity;

float GetCameraSpeed() { return CameraMoveSensitivity; }

void Init_Camera(Camera *camera)
{
    camera->position = (Vector3){ 0.0f, 25.0f, 25.0f };
    camera->target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera->up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera->fovy = 45.0f;
    float dx = camera->position.x - camera->target.x;
    float dy = camera->position.y - camera->target.y;
    float dz = camera->position.z - camera->target.z;
    angle.x = atan2f(dx, dz);
    angle.y = atan2f(dy, sqrtf(dx*dx + dz*dz));
    previousMousePosition = GetMousePosition();
    CameraMoveSensitivity = 0.1f;
    DisableCursor();
}

void Update_Camera(Camera *camera)
{
    Vector2 mousePositionDelta = { 0.0f, 0.0f };
    Vector2 mousePosition = GetMousePosition();
    float mouseWheelMove = GetMouseWheelMove();
    CameraMoveSensitivity *= pow(CameraMoveExponential, mouseWheelMove);
    printf("%f, %f\r", CameraMoveSensitivity, mouseWheelMove);
    bool direction[6] = { IsKeyDown(moveControl[MOVE_FRONT]),
                          IsKeyDown(moveControl[MOVE_BACK]),
                          IsKeyDown(moveControl[MOVE_RIGHT]),
                          IsKeyDown(moveControl[MOVE_LEFT]),
                          IsKeyDown(moveControl[MOVE_UP]),
                          IsKeyDown(moveControl[MOVE_DOWN]) };
    mousePositionDelta.x = mousePosition.x - previousMousePosition.x;
    mousePositionDelta.y = mousePosition.y - previousMousePosition.y;
    previousMousePosition = mousePosition;
    camera->position.x += (sinf(angle.x)*direction[MOVE_BACK] -
                            sinf(angle.x)*direction[MOVE_FRONT] -
                            cosf(angle.x)*direction[MOVE_LEFT] +
                            cosf(angle.x)*direction[MOVE_RIGHT])*CameraMoveSensitivity;
    camera->position.y += (sinf(angle.y)*direction[MOVE_BACK] -
                            sinf(angle.y)*direction[MOVE_FRONT] +
                            direction[MOVE_UP] - direction[MOVE_DOWN])*CameraMoveSensitivity;
    camera->position.z += (cosf(angle.x)*direction[MOVE_BACK] -
                            cosf(angle.x)*direction[MOVE_FRONT] +
                            sinf(angle.x)*direction[MOVE_LEFT] -
                            sinf(angle.x)*direction[MOVE_RIGHT])*CameraMoveSensitivity;
    angle.x -= (MouseMoveSensitivity * mousePositionDelta.x);
    angle.y += (MouseMoveSensitivity * mousePositionDelta.y);
    angle.y = VAL_LIMIT(angle.y, -1.57f, 1.57f);
    camera->target.x = camera->position.x - cosf(angle.y)*sinf(angle.x);
    camera->target.y = camera->position.y - sinf(angle.y);
    camera->target.z = camera->position.z - cosf(angle.y)*cosf(angle.x);
}
