#ifndef UTILS_H
#define UTILS_H
#include "raylib.h"

typedef struct {
    Vector3 r;
    Vector3 v;
} RadiusVelocity;

#if defined(__cplusplus)
extern "C" {
#endif

void Init_Skybox();
void Update_Skybox();
void Print(float* a);

void Init_Camera(Camera *camera, RadiusVelocity PosTgt);
void Update_Camera(Camera *camera, RadiusVelocity RefFrame);
void SetCameraSpeed(float speed);
float GetCameraSpeed();
void SetReferenceFrame(RadiusVelocity rv);

#if defined(__cplusplus)
}
#endif

#endif // UTILS_H
