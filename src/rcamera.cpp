#include <cmath>  // Required for: sinf(), cosf(), sqrtf()
#include "rcamera.h"

CameraData CAMERA = {        // Global CAMERA state context
    .mode = 0,
    .targetDistance = 0,
    .playerEyesPosition = 1.85f,
    .angle = { 0 },
    .previousMousePosition = { 0 },
    .moveControl = { 'W', 'S', 'D', 'A', 'E', 'Q' },
    .smoothZoomControl = 341,       // raylib: KEY_LEFT_CONTROL
    .altControl = 342,              // raylib: KEY_LEFT_ALT
    .panControl = 2                 // raylib: MOUSE_BUTTON_MIDDLE
};

void SetCamera(Camera camera)
{
    Vector3 v1 = camera.position;
    Vector3 v2 = camera.target;
    float dx = v2.x - v1.x;
    float dy = v2.y - v1.y;
    float dz = v2.z - v1.z;
    CAMERA.targetDistance = sqrtf(dx*dx + dy*dy + dz*dz);   // Distance to target
    CAMERA.angle.x = atan2f(dx, dz);                        // Camera angle in plane XZ (0 aligned with Z, move positive CCW)
    CAMERA.angle.y = atan2f(dy, sqrtf(dx*dx + dz*dz));      // Camera angle in plane XY (0 aligned with X, move positive CW)
    CAMERA.playerEyesPosition = camera.position.y;          // Init player eyes position to camera Y position
    CAMERA.previousMousePosition = GetMousePosition();      // Init mouse position
    DisableCursor();
}

void UpdateCamera(Camera *camera)
{
    // static int swingCounter = 0;    // Used for 1st person swinging movement
    // Mouse movement detection
    Vector2 mousePositionDelta = { 0.0f, 0.0f };
    Vector2 mousePosition = GetMousePosition();
    float mouseWheelMove = GetMouseWheelMove();

    // Keys input detection
    bool keyPan = IsMouseButtonDown(CAMERA.panControl);
    bool keyAlt = IsKeyDown(CAMERA.altControl);
    bool szoomKey = IsKeyDown(CAMERA.smoothZoomControl);
    bool direction[6] = { IsKeyDown(CAMERA.moveControl[MOVE_FRONT]),
                          IsKeyDown(CAMERA.moveControl[MOVE_BACK]),
                          IsKeyDown(CAMERA.moveControl[MOVE_RIGHT]),
                          IsKeyDown(CAMERA.moveControl[MOVE_LEFT]),
                          IsKeyDown(CAMERA.moveControl[MOVE_UP]),
                          IsKeyDown(CAMERA.moveControl[MOVE_DOWN]) };
    mousePositionDelta.x = mousePosition.x - CAMERA.previousMousePosition.x;
    mousePositionDelta.y = mousePosition.y - CAMERA.previousMousePosition.y;
    CAMERA.previousMousePosition = mousePosition;
    camera->position.x += (sinf(CAMERA.angle.x)*direction[MOVE_BACK] -
                            sinf(CAMERA.angle.x)*direction[MOVE_FRONT] -
                            cosf(CAMERA.angle.x)*direction[MOVE_LEFT] +
                            cosf(CAMERA.angle.x)*direction[MOVE_RIGHT])/PLAYER_MOVEMENT_SENSITIVITY;

    camera->position.y += (sinf(CAMERA.angle.y)*direction[MOVE_FRONT] -
                            sinf(CAMERA.angle.y)*direction[MOVE_BACK] +
                            direction[MOVE_UP] - direction[MOVE_DOWN])/PLAYER_MOVEMENT_SENSITIVITY;

    camera->position.z += (cosf(CAMERA.angle.x)*direction[MOVE_BACK] -
                            cosf(CAMERA.angle.x)*direction[MOVE_FRONT] +
                            sinf(CAMERA.angle.x)*direction[MOVE_LEFT] -
                            sinf(CAMERA.angle.x)*direction[MOVE_RIGHT])/PLAYER_MOVEMENT_SENSITIVITY;

    CAMERA.angle.x -= (CAMERA_MOUSE_MOVE_SENSITIVITY * mousePositionDelta.x);
    CAMERA.angle.y -= (CAMERA_MOUSE_MOVE_SENSITIVITY * mousePositionDelta.y);
    CAMERA.angle.y = VAL_LIMIT(CAMERA.angle.y, -1.57f, 1.57f);

    float cosz = 1.0f;
    float sinz = 0.0f;
    float cosy = cosf(CAMERA.angle.x);
    float siny = sinf(CAMERA.angle.x);
    float cosx = cosf(CAMERA.angle.y);
    float sinx = sinf(CAMERA.angle.y);
    float tx = (cosz*siny*cosx) + (sinz*sinx);
    float ty = (sinz*siny*cosx) - (cosz*sinx);
    float tz = cosy*cosx;
    float dist = (CAMERA.targetDistance/CAMERA_FREE_PANNING_DIVIDER);

    camera->target.x = camera->position.x - tx*dist;
    camera->target.y = camera->position.y - ty*dist;
    camera->target.z = camera->position.z - tz*dist;
}
