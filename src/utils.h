#ifndef UTILS_H
#define UTILS_H

#if defined(__cplusplus)
extern "C" {
#endif

void Init_Skybox();
void Update_Skybox();

void DrawFPS(int fps);
void DrawCPUusage(int cpu);

void Init_Camera(Camera *camera);
void Update_Camera(Camera *camera);

#if defined(__cplusplus)
}
#endif

#endif // UTILS_H
