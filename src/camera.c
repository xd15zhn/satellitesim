#include "raylib.h"
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

float pit, rol, yaw;
Vector2 previousMousePosition;
const int moveControl[6] = { 'W', 'S', 'D', 'A', 'E', 'Q' };
float CameraMoveSensitivity;

float GetCameraSpeed() { return CameraMoveSensitivity; }

void Init_Camera(Camera *camera)
{
    camera->position = (Vector3){ 0.0f, -25.0f, 25.0f };
    camera->target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera->up = (Vector3){ 0.0f, 0.0f, 1.0f };
    camera->fovy = 45.0f;
    pit = -45.0f*DEG2RAD;
    rol = 0;
    yaw = 0;
    previousMousePosition = GetMousePosition();
    CameraMoveSensitivity = 0.1f;
    DisableCursor();
}

void Update_Camera(Camera *camera)
{
    Vector2 mousePositionDelta;
    Vector2 mousePosition = GetMousePosition();
    float mouseWheelMove = GetMouseWheelMove();
    CameraMoveSensitivity *= pow(CameraMoveExponential, mouseWheelMove);
    char direction[3] = {
        IsKeyDown(moveControl[MOVE_RIGHT]) - IsKeyDown(moveControl[MOVE_LEFT]),
        IsKeyDown(moveControl[MOVE_FRONT]) - IsKeyDown(moveControl[MOVE_BACK]),
        IsKeyDown(moveControl[MOVE_UP])    - IsKeyDown(moveControl[MOVE_DOWN]),
    };
    mousePositionDelta.x = mousePosition.x - previousMousePosition.x;
    mousePositionDelta.y = mousePosition.y - previousMousePosition.y;
    previousMousePosition = mousePosition;
    float dx = direction[0] * CameraMoveSensitivity;
    float dy = direction[1] * CameraMoveSensitivity;
    float dz = direction[2] * CameraMoveSensitivity;
    camera->position.x += dx * cos(pit)*cos(yaw) - dy * cos(pit)*sin(yaw);
    camera->position.y += dx * cos(pit)*sin(yaw) + dy * cos(pit)*cos(yaw);
    camera->position.z += dz + dy*sin(pit);
    pit -= (MouseMoveSensitivity * mousePositionDelta.y);
    yaw -= (MouseMoveSensitivity * mousePositionDelta.x);
    pit = VAL_LIMIT(pit, -1.57f, 1.57f);
    camera->target.x = camera->position.x - cosf(pit)*sinf(yaw);
    camera->target.y = camera->position.y + cosf(pit)*cosf(yaw);
    camera->target.z = camera->position.z + sinf(pit);
}
