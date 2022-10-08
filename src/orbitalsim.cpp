#include "orbitalsim.hpp"

/****************************************************************
// 初始化
****************************************************************/
Orbital_Object::Orbital_Object(int id): _id(id) {
    /* 3D模型部分 */
    _scale = (Vector3) { 1.0f, 1.0f, 1.0f};
    _rotAxis = (Vector3) { 0.0f, 0.0f, 1.0f};
    _rotAngle = 0.0f;
    for (int i = 0; i < _oparam._PointNum; i++)
        _Points.push_back(Vector3{0, 0, 0});

    /* 仿真器部分 */
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
    _sim1.Set_EnableStore(false);
    _sim1.Initialize();
    _simstep = _sim1.Get_SimStep();
}


/****************************************************************
// 3D模型部分
****************************************************************/
void Orbital_Object::Load_Model(const char* objfile, const char* pngfile) {
    _model = LoadModel(objfile);
    Texture2D img = LoadTexture(pngfile);
    _model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = img;
}
void Orbital_Object::Update_Model() {
    Vector3 pos = Get_R();
    DrawModelEx(_model, pos, _rotAxis, _rotAngle, _scale, WHITE);
    if (!_draw) return;
    Vector3 posstart, posend;
    std::list<Vector3>::iterator it = _Points.begin();
    posstart = *it;
    it++;
    for (; it != _Points.end(); ++it) {
        posend = *it;
        DrawLine3D(posstart, posend, RAYWHITE);
        posstart = posend;
    }
    _oparam._cnt++;
    if (_oparam._cnt<_oparam._PointInterval) return;
    _oparam._cnt = 0;
    _Points.push_back(pos);
    _Points.pop_front();
}
void Orbital_Object::Set_Scale(float scale) {
    _scale = (Vector3) { scale, scale, scale };
}
void Orbital_Object::Set_ID(int id) { _id=id; }
void Orbital_Object::Set_DrawOrbit(bool draw) { _draw=draw; }
void Orbital_Object::Set_RotationAxis(Vector3 rotationAxis) { _rotAxis = Vector3Normalize(rotationAxis); }
void Orbital_Object::Set_RotationAngle(float rotationAngle) { _rotAngle = rotationAngle; }


/****************************************************************
// 仿真器部分
****************************************************************/
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
    _Points.clear();
    for (int i = 0; i < _oparam._PointNum; i++)
        _Points.push_back(vec);
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
