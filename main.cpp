#include <iostream>
#include <cmath>
#include "rcamera.h"
#include "simucpp.hpp"
using namespace simucpp;
using namespace zhnmat;
using namespace std;

class Orbital_Simulator
{
public:
    Orbital_Simulator() {
        SUIntegrator(intrx, &sim1);
        SUIntegrator(intry, &sim1);
        SUIntegrator(intrz, &sim1);
        SUIntegrator(intvx, &sim1);
        SUIntegrator(intvy, &sim1);
        SUIntegrator(intvz, &sim1);
        SUProduct(prodax, &sim1);
        SUProduct(proday, &sim1);
        SUProduct(prodaz, &sim1);
        SUFcnMISO(misomur, &sim1);
        sim1.connectU(prodax, intvx);
        sim1.connectU(proday, intvy);
        sim1.connectU(prodaz, intvz);
        sim1.connectU(intvx, intrx);
        sim1.connectU(intvy, intry);
        sim1.connectU(intvz, intrz);
        sim1.connectU(intrx, misomur);
        sim1.connectU(intry, misomur);
        sim1.connectU(intrz, misomur);
        sim1.connectU(misomur, prodax);
        sim1.connectU(misomur, proday);
        sim1.connectU(misomur, prodaz);
        sim1.connectU(intrx, prodax);
        sim1.connectU(intry, proday);
        sim1.connectU(intrz, prodaz);
        misomur->Set_Function([](double *u){
            Vector3d r(u[0], u[1], u[2]);
            double ans = r.norm2();
            return -5000.0 / (ans*ans*ans);
        });
        intrx->Set_InitialValue(15);
        intry->Set_InitialValue(0);
        intvx->Set_InitialValue(0);
        intvy->Set_InitialValue(12);
        sim1.Set_SimStep(0.01);
        sim1.Initialize();
        _simstep = sim1.Get_SimStep();
    };
        // for (;;)
    Vector3 Simulate(double time) {
        int stepcnt = (int)(time/_simstep/1000.0+0.5);
        for (int i=0; i<stepcnt; ++i)
            sim1.Simulate_OneStep();
        float x = intrx->Get_OutValue();
        float y = intry->Get_OutValue();
        float z = intrz->Get_OutValue();
        return Vector3{x, y, z};
    }
    void Reset(void){
        sim1.Simulation_Reset();
    }
private:
    Simulator sim1;
    UIntegrator *intrx=nullptr;
    UIntegrator *intry=nullptr;
    UIntegrator *intrz=nullptr;
    UIntegrator *intvx=nullptr;
    UIntegrator *intvy=nullptr;
    UIntegrator *intvz=nullptr;
    UProduct *prodax=nullptr;
    UProduct *proday=nullptr;
    UProduct *prodaz=nullptr;
    UFcnMISO *misomur=nullptr;
    double _simstep;
};

int main(void) {
    Orbital_Simulator shuffle;
	SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(960, 540, "RayLib-3D");
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 0.0f, 10.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 20.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
	SetCamera(camera);
    SetTargetFPS(60);
    Model model = LoadModel("SpaceShuttle.obj");
    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
    Vector3 ShuttlePosition, ptemp;
    double t;
    // ToggleFullscreen();
    while (!WindowShouldClose()) {
        t = GetTime();
        ptemp = shuffle.Simulate(10);
        ShuttlePosition.x = ptemp.x;
        ShuttlePosition.y = ptemp.z;
        ShuttlePosition.z = ptemp.y;
        cout << camera.target.x << " ";
        cout << camera.target.y << " ";
        cout << camera.target.z << "\r";
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
    cout << "\nfinished." << endl;
    return 0;
}