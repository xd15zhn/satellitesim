#include "raylib.h"
#include "utils.h"
#include <math.h>
#include <stdio.h>

#define CameraMoveSpeedInit              0.01f
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
float CameraMoveSpeed = CameraMoveSpeedInit;
Matrix referenceFrame;
Vector3 frameCameraPos, frameCameraTgt;


void SetCameraSpeed(float speed) {CameraMoveSpeed=speed;}
float GetCameraSpeed() { return CameraMoveSpeed; }


/**********************
地球的速度向量定为y轴，x轴在黄道平面朝外与y轴垂直，z轴朝上。
**********************/
void SetReferenceFrame(RadiusVelocity rv) {
#if 1  // 相机坐标系建立在地球上
    Vector3 refZ = Vector3CrossProduct(rv.r, rv.v);
    Vector3 refX = Vector3CrossProduct(rv.v, refZ);
    refX = Vector3Normalize(refX);
    refZ = Vector3Normalize(refZ);
    Vector3 refY = Vector3Normalize(rv.v);
    referenceFrame.m0  = refX.x;
    referenceFrame.m1  = refX.y;
    referenceFrame.m2  = refX.z;
    referenceFrame.m4  = refY.x;
    referenceFrame.m5  = refY.y;
    referenceFrame.m6  = refY.z;
    referenceFrame.m8  = refZ.x;
    referenceFrame.m9  = refZ.y;
    referenceFrame.m10 = refZ.z;
    referenceFrame.m12 = rv.r.x;
    referenceFrame.m13 = rv.r.y;
    referenceFrame.m14 = rv.r.z;
    referenceFrame.m3  = 0;
    referenceFrame.m7  = 0;
    referenceFrame.m11 = 0;
    referenceFrame.m15 = 1;
#else  // 相机坐标系建立在太阳上
    referenceFrame = MatrixIdentity();
#endif
}

void Init_Camera(Camera *camera, RadiusVelocity PosTgt)
{
    frameCameraPos = PosTgt.r;
    frameCameraTgt = PosTgt.v;
    camera->position = Vector3Transform(frameCameraPos, referenceFrame);
    camera->target = Vector3Transform(frameCameraTgt, referenceFrame);
    camera->up = (Vector3){ 0.0f, 0.0f, 1.0f };
    camera->fovy = 45.0f;
    Vector3 delta = Vector3Subtract(camera->target, camera->position);
    pit = atan(delta.z / sqrt(delta.x*delta.x + delta.y*delta.y));
    rol = 0;
    yaw = atan2(delta.y, delta.x) - PI/2;
    pit = VAL_LIMIT(pit, -1.57f, 1.57f);
    previousMousePosition = GetMousePosition();
    CameraMoveSpeed = CameraMoveSpeed;
    DisableCursor();
}

void Update_Camera(Camera *camera, RadiusVelocity refFrame)
{
    SetReferenceFrame(refFrame);
    Vector2 mousePositionDelta;
    Vector2 mousePosition = GetMousePosition();
    float mouseWheelMove = GetMouseWheelMove();
    CameraMoveSpeed *= pow(CameraMoveExponential, mouseWheelMove);
    char direction[3] = {
        IsKeyDown(moveControl[MOVE_RIGHT]) - IsKeyDown(moveControl[MOVE_LEFT]),
        IsKeyDown(moveControl[MOVE_FRONT]) - IsKeyDown(moveControl[MOVE_BACK]),
        IsKeyDown(moveControl[MOVE_UP])    - IsKeyDown(moveControl[MOVE_DOWN]),
    };
    mousePositionDelta.x = mousePosition.x - previousMousePosition.x;
    mousePositionDelta.y = mousePosition.y - previousMousePosition.y;
    previousMousePosition = mousePosition;
    float dx = direction[0] * CameraMoveSpeed;
    float dy = direction[1] * CameraMoveSpeed;
    float dz = direction[2] * CameraMoveSpeed;
    frameCameraPos.x += dx * cos(pit)*cos(yaw) - dy * cos(pit)*sin(yaw);
    frameCameraPos.y += dx * cos(pit)*sin(yaw) + dy * cos(pit)*cos(yaw);
    frameCameraPos.z += dz + dy*sin(pit);
    pit -= (MouseMoveSensitivity * mousePositionDelta.y);
    yaw -= (MouseMoveSensitivity * mousePositionDelta.x);
    pit = VAL_LIMIT(pit, -1.57f, 1.57f);
    frameCameraTgt.x = frameCameraPos.x - cosf(pit)*sinf(yaw);
    frameCameraTgt.y = frameCameraPos.y + cosf(pit)*cosf(yaw);
    frameCameraTgt.z = frameCameraPos.z + sinf(pit);
    camera->position = Vector3Transform(frameCameraPos, referenceFrame);
    camera->target = Vector3Transform(frameCameraTgt, referenceFrame);
}
