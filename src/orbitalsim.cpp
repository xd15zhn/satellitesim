#include "orbitalsim.hpp"

Orbital_Object::Orbital_Object() {
    SUIntegrator(_intrx, &_sim1);
    SUIntegrator(_intry, &_sim1);
    SUIntegrator(_intrz, &_sim1);
    SUIntegrator(_intvx, &_sim1);
    SUIntegrator(_intvy, &_sim1);
    SUIntegrator(_intvz, &_sim1);
    SUProduct(_prodax, &_sim1);
    SUProduct(_proday, &_sim1);
    SUProduct(_prodaz, &_sim1);
    SUFcnMISO(_misomur, &_sim1);
    _sim1.connectU(_prodax, _intvx);
    _sim1.connectU(_proday, _intvy);
    _sim1.connectU(_prodaz, _intvz);
    _sim1.connectU(_intvx, _intrx);
    _sim1.connectU(_intvy, _intry);
    _sim1.connectU(_intvz, _intrz);
    _sim1.connectU(_intrx, _misomur);
    _sim1.connectU(_intry, _misomur);
    _sim1.connectU(_intrz, _misomur);
    _sim1.connectU(_misomur, _prodax);
    _sim1.connectU(_misomur, _proday);
    _sim1.connectU(_misomur, _prodaz);
    _sim1.connectU(_intrx, _prodax);
    _sim1.connectU(_intry, _proday);
    _sim1.connectU(_intrz, _prodaz);
    _misomur->Set_Function([](double *u){
        double ans = u[0]*u[0] + u[1]*u[1] + u[2]*u[2];
        ans = sqrt(ans);
        return -5000.0 / (ans*ans*ans);
    });
    _sim1.Set_SimStep(0.01);
    _sim1.Initialize();
    _simstep = _sim1.Get_SimStep();
}

//--------------------------------------------------------------------------------------------
// 3D模型部分
//--------------------------------------------------------------------------------------------
void Orbital_Object::Load_Model(const char* objfile, const char* pngfile) {
    _model = LoadModel(objfile);
    Texture2D img = LoadTexture(pngfile);
    _model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = img;
}
void Orbital_Object::Update_Model(Matrix transform) {
    _model.transform = transform;
    Vector3 pos = Get_R();
    DrawModel(_model, (Vector3){pos.x, pos.z, pos.y}, _scale, WHITE);
}
void Orbital_Object::Set_Scale(double scale) {
    _scale = scale;
}

//--------------------------------------------------------------------------------------------
// 仿真器部分
//--------------------------------------------------------------------------------------------
void Orbital_Object::Simulate(double time) {
    int stepcnt = (int)(time/_simstep/1000.0+0.5);
    for (int i=0; i<stepcnt; ++i)
        _sim1.Simulate_OneStep();
}

void Orbital_Object::Set_initRV(Vector3 vecR, Vector3 vecV) {
    Set_initR(vecR); Set_initV(vecV);
}
void Orbital_Object::Set_initR(Vector3 vec) {
    _intrx->Set_InitialValue(vec.x);
    _intry->Set_InitialValue(vec.y);
    _intrz->Set_InitialValue(vec.z);
}
void Orbital_Object::Set_initV(Vector3 vec) {
    _intvx->Set_InitialValue(vec.x);
    _intvy->Set_InitialValue(vec.y);
    _intvz->Set_InitialValue(vec.z);
}

Vector3 Orbital_Object::Get_R() {
    return Vector3{
        (float)(_intrx->Get_OutValue()),
        (float)(_intry->Get_OutValue()),
        (float)(_intrz->Get_OutValue())};
}
Vector3 Orbital_Object::Get_V() {
    return Vector3{
        (float)(_intvx->Get_OutValue()),
        (float)(_intvy->Get_OutValue()),
        (float)(_intvz->Get_OutValue())};
}

void Orbital_Object::Reset(void) {
    _sim1.Simulation_Reset();
}
