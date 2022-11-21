#ifndef ORBITALSIM_H
#define ORBITALSIM_H

#include <list>
#include <cmath>
#include "simucpp.hpp"
#include "raylib.h"

using namespace simucpp;

struct OrbitPointParam {
    int _PointNum=256;
    int _PointInterval=4;
    int _cnt=0;
};


class Orbital_Object {
public:
    Orbital_Object(int id=0);
    /* 3D模型部分 */
    void Load_Model(const char* objfile, const char* pngfile);
    void Update_Model();
    void Set_Scale(float scale);
    void Set_ID(int id);
    void Set_DrawOrbit(bool draw=true);
    void Set_RotationAxis(Vector3 rotationAxis);
    void Set_RotationAngle(float rotationAngle);

    /* 仿真器部分 */
    void Simulate(int time);
    void Set_initRV(Vector3 vecR, Vector3 vecV);
    void Set_initR(Vector3 vec);
    void Set_initV(Vector3 vec);
    Vector3 Get_R();
    Vector3 Get_V();
    void Reset(void);

private:
    /* 3D模型部分 */
    Model _model;
    Vector3 _scale;
    std::list<Vector3> _Points;
    OrbitPointParam _oparam;
    int _id=0;
    bool _draw=true;
    float _rotAngle;
    Vector3 _rotAxis;

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
