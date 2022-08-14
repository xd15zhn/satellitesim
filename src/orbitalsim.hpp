#ifndef ORBITALSIM_H
#define ORBITALSIM_H

#include "simucpp.hpp"
#include "raylib.h"
#include "raymath.h"

using namespace simucpp;

class Orbital_Object
{
public:
    Orbital_Object();
    /* 3D模型部分 */
    void Load_Model(const char* objfile, const char* pngfile);
    void Update_Model(Matrix transform);
    void Set_Scale(double scale);

    /* 仿真器部分 */
    void Simulate(double time);
    void Set_initRV(Vector3 vecR, Vector3 vecV);
    void Set_initR(Vector3 vec);
    void Set_initV(Vector3 vec);
    Vector3 Get_R();
    Vector3 Get_V();
    void Reset(void);

private:
    /* 3D模型部分 */
    Model _model;
    double _scale=1;

    /* 仿真器部分 */
    Simulator _sim1;
    UIntegrator *_intrx=nullptr;
    UIntegrator *_intry=nullptr;
    UIntegrator *_intrz=nullptr;
    UIntegrator *_intvx=nullptr;
    UIntegrator *_intvy=nullptr;
    UIntegrator *_intvz=nullptr;
    UProduct *_prodax=nullptr;
    UProduct *_proday=nullptr;
    UProduct *_prodaz=nullptr;
    UFcnMISO *_misomur=nullptr;
    double _simstep;
};

#endif // ORBITALSIM_H
