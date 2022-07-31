#ifndef ORBITALSIM_H
#define ORBITALSIM_H

#include "simucpp.hpp"
using namespace simucpp;
using namespace zhnmat;

class Orbital_Simulator
{
public:
    Orbital_Simulator();
    void Simulate(double time);
    Vector3d Get_R();
    Vector3d Get_V();
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
