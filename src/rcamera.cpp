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
                            1.0f*direction[MOVE_UP] - 1.0f*direction[MOVE_DOWN])/PLAYER_MOVEMENT_SENSITIVITY;

    camera->position.z += (cosf(CAMERA.angle.x)*direction[MOVE_BACK] -
                            cosf(CAMERA.angle.x)*direction[MOVE_FRONT] +
                            sinf(CAMERA.angle.x)*direction[MOVE_LEFT] -
                            sinf(CAMERA.angle.x)*direction[MOVE_RIGHT])/PLAYER_MOVEMENT_SENSITIVITY;

    // Camera orientation calculation
    CAMERA.angle.x -= (CAMERA_MOUSE_MOVE_SENSITIVITY * mousePositionDelta.x);
    CAMERA.angle.y -= (CAMERA_MOUSE_MOVE_SENSITIVITY * mousePositionDelta.y);
    // Angle clamp
    CAMERA.angle.y = VAL_LIMIT(CAMERA.angle.y, -1.57f, 1.57f);
    // Calculate translation matrix
    Matrix matTranslation = { 1.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 1.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 1.0f, (CAMERA.targetDistance/CAMERA_FREE_PANNING_DIVIDER),
                                0.0f, 0.0f, 0.0f, 1.0f };

    // Calculate rotation matrix
    Matrix matRotation = { 1.0f, 0.0f, 0.0f, 0.0f,
                            0.0f, 1.0f, 0.0f, 0.0f,
                            0.0f, 0.0f, 1.0f, 0.0f,
                            0.0f, 0.0f, 0.0f, 1.0f };

    float cosz = cosf(0.0f);
    float sinz = sinf(0.0f);
    float cosy = cosf(CAMERA.angle.x);
    float siny = sinf(CAMERA.angle.x);
    float cosx = cosf(CAMERA.angle.y);
    float sinx = sinf(CAMERA.angle.y);

    matRotation.m0 = cosz*cosy;
    matRotation.m4 = (cosz*siny*sinx) - (sinz*cosx);
    matRotation.m8 = (cosz*siny*cosx) + (sinz*sinx);
    matRotation.m1 = sinz*cosy;
    matRotation.m5 = (sinz*siny*sinx) + (cosz*cosx);
    matRotation.m9 = (sinz*siny*cosx) - (cosz*sinx);
    matRotation.m2 = -siny;
    matRotation.m6 = cosy*sinx;
    matRotation.m10= cosy*cosx;

    // Multiply translation and rotation matrices
    Matrix matTransform = { 0 };
    matTransform.m0 = matTranslation.m0*matRotation.m0 + matTranslation.m1*matRotation.m4 + matTranslation.m2*matRotation.m8 + matTranslation.m3*matRotation.m12;
    matTransform.m1 = matTranslation.m0*matRotation.m1 + matTranslation.m1*matRotation.m5 + matTranslation.m2*matRotation.m9 + matTranslation.m3*matRotation.m13;
    matTransform.m2 = matTranslation.m0*matRotation.m2 + matTranslation.m1*matRotation.m6 + matTranslation.m2*matRotation.m10 + matTranslation.m3*matRotation.m14;
    matTransform.m3 = matTranslation.m0*matRotation.m3 + matTranslation.m1*matRotation.m7 + matTranslation.m2*matRotation.m11 + matTranslation.m3*matRotation.m15;
    matTransform.m4 = matTranslation.m4*matRotation.m0 + matTranslation.m5*matRotation.m4 + matTranslation.m6*matRotation.m8 + matTranslation.m7*matRotation.m12;
    matTransform.m5 = matTranslation.m4*matRotation.m1 + matTranslation.m5*matRotation.m5 + matTranslation.m6*matRotation.m9 + matTranslation.m7*matRotation.m13;
    matTransform.m6 = matTranslation.m4*matRotation.m2 + matTranslation.m5*matRotation.m6 + matTranslation.m6*matRotation.m10 + matTranslation.m7*matRotation.m14;
    matTransform.m7 = matTranslation.m4*matRotation.m3 + matTranslation.m5*matRotation.m7 + matTranslation.m6*matRotation.m11 + matTranslation.m7*matRotation.m15;
    matTransform.m8 = matTranslation.m8*matRotation.m0 + matTranslation.m9*matRotation.m4 + matTranslation.m10*matRotation.m8 + matTranslation.m11*matRotation.m12;
    matTransform.m9 = matTranslation.m8*matRotation.m1 + matTranslation.m9*matRotation.m5 + matTranslation.m10*matRotation.m9 + matTranslation.m11*matRotation.m13;
    matTransform.m10 = matTranslation.m8*matRotation.m2 + matTranslation.m9*matRotation.m6 + matTranslation.m10*matRotation.m10 + matTranslation.m11*matRotation.m14;
    matTransform.m11 = matTranslation.m8*matRotation.m3 + matTranslation.m9*matRotation.m7 + matTranslation.m10*matRotation.m11 + matTranslation.m11*matRotation.m15;
    matTransform.m12 = matTranslation.m12*matRotation.m0 + matTranslation.m13*matRotation.m4 + matTranslation.m14*matRotation.m8 + matTranslation.m15*matRotation.m12;
    matTransform.m13 = matTranslation.m12*matRotation.m1 + matTranslation.m13*matRotation.m5 + matTranslation.m14*matRotation.m9 + matTranslation.m15*matRotation.m13;
    matTransform.m14 = matTranslation.m12*matRotation.m2 + matTranslation.m13*matRotation.m6 + matTranslation.m14*matRotation.m10 + matTranslation.m15*matRotation.m14;
    matTransform.m15 = matTranslation.m12*matRotation.m3 + matTranslation.m13*matRotation.m7 + matTranslation.m14*matRotation.m11 + matTranslation.m15*matRotation.m15;

    camera->target.x = camera->position.x - matTransform.m12;
    camera->target.y = camera->position.y - matTransform.m13;
    camera->target.z = camera->position.z - matTransform.m14;
}
