#ifndef ORBITALSIM_H
#define ORBITALSIM_H

#include "simucpp.hpp"
#include "raymath.h"

using namespace simucpp;

class Orbital_Simulator
{
public:
    Orbital_Simulator();
    void Simulate(double time);
    void Set_initRV(Vector3 vecR, Vector3 vecV);
    void Set_initR(Vector3 vec);
    void Set_initV(Vector3 vec);
    Vector3 Get_R();
    Vector3 Get_V();
    void Reset(void);
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

#endif // ORBITALSIM_H
