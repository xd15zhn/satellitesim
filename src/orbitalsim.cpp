#include "orbitalsim.hpp"

Orbital_Elements::Orbital_Elements(double mu, Mat orbitalElement) {
    Initialize(mu, orbitalElement);
}
void Orbital_Elements::Initialize(double mu, Mat orbitalElement) {
    _mu = mu;
    _a = orbitalElement.at(0, 0);  // a-半长轴
    _e = orbitalElement.at(1, 0);  // e-偏心率
    _i = orbitalElement.at(2, 0);  // i-轨道倾角
    _n = orbitalElement.at(3, 0);  // Ω-升交点赤经
    _w = orbitalElement.at(4, 0);  // ω-近地点幅角
    _f = orbitalElement.at(5, 0);  // f-真近点角
    if (_e > 1) _a = -_a;
    _kv = sqrt(_mu/(_a*(1-_e*_e)));
    _omega = sqrt(_mu/(_a*_a*_a));
    _RT = Mat(3, 3, vecdble{
        cos(_n)*cos(_w)-sin(_n)*cos(_i)*sin(_w), -cos(_n)*sin(_w)-sin(_n)*cos(_i)*cos(_w),  sin(_n)*sin(_i),
        sin(_n)*cos(_w)+cos(_n)*cos(_i)*sin(_w), -sin(_n)*sin(_w)+cos(_n)*cos(_i)*cos(_w), -cos(_n)*sin(_i),
        sin(_i)*sin(_w),                          sin(_i)*cos(_w),                          cos(_i)
    });
}

void Orbital_Elements::Update_TrueAnomaly(double t) {
    double ma = _omega*t;  // 平近点角(mean anomaly)
    double ta = ma;  // 真近点角(true anomaly)
    ta += (_e+_e-_e*_e*_e*0.25)*sin(ma);
    ta += 1.25*_e*_e*sin(2*ma);
    ta += 13/12*_e*_e*_e*sin(3*ma);
    _ta = ta;
}
Mat Orbital_Elements::Update_Position() {
    double r = _a*(1-_e*_e)/(1+_e*cos(_ta));
    _position = Mat(3, 1, vecdble{r*cos(_ta), r*sin(_ta), 0});
    _position = _RT*_position;
    return _position;
}

Mat Orbital_Elements::Update_Velocity() {
    double r = _a*(1-_e*_e)/(1+_e*cos(_ta));
    _velocity = Mat(3, 1, vecdble{-_kv*sin(_ta), _kv*(_e+cos(_ta)), 0});
    _velocity = _RT*_velocity;
    return _velocity;
}


Mat GravityAcceleration(Mat* u) {
    Mat Rearth = u[0];
    Mat Rstlt = u[1];
    double r[3], rabs, k;
    r[0] = Rstlt.at(0, 0);
    r[1] = Rstlt.at(1, 0);
    r[2] = Rstlt.at(2, 0);
    rabs = r[0]*r[0] + r[1]*r[1] + r[2]*r[2];
    rabs = sqrt(rabs);
    k = -muSun / (rabs*rabs*rabs);
    Mat ans = k * Rstlt;
    r[0] = Rstlt.at(0, 0) - Rearth.at(0, 0);
    r[1] = Rstlt.at(1, 0) - Rearth.at(1, 0);
    r[2] = Rstlt.at(2, 0) - Rearth.at(2, 0);
    rabs = r[0]*r[0] + r[1]*r[1] + r[2]*r[2];
    rabs = sqrt(rabs);
    k = -muEarth / (rabs*rabs*rabs);
    ans += k * (Rstlt - Rearth);
    return ans;
}


/**********************
一个卫星
**********************/
SubsysSatellite::SubsysSatellite(Simulator *sim, std::string name) {
    _name = name;
    _mssV = new MStateSpace(sim, BusSize(3, 1), true, _name+"_mssV");
    _mssR = new MStateSpace(sim, BusSize(3, 1), true, _name+"_mssR");
    _gain = new MGain(sim, eye(3), true, _name+"_gain");
    _miso = new MFcnMISO(sim, BusSize(3, 1), _name+"_miso");
    sim->connectM(_mssV, _mssR);
    sim->connectM(_gain, _miso);
    sim->connectM(_mssR, _miso);
    sim->connectM(_miso, _mssV);
    _miso->Set_Function(GravityAcceleration);
};
PMatModule SubsysSatellite::Get_InputBus(int n) const {
    if (n==0) return _gain;
    return nullptr;
};
void SubsysSatellite::Set_initRV(RadiusVelocity rv) {
    _mssR->Set_InitialValue(Mat(3, 1, vecdble{rv.r.x, rv.r.y, rv.r.z}));
    _mssV->Set_InitialValue(Mat(3, 1, vecdble{rv.v.x, rv.v.y, rv.v.z}));
}
Mat SubsysSatellite::Get_Position() { return _mssR->Get_OutValue(); }
Mat SubsysSatellite::Get_Velocity() { return _mssV->Get_OutValue(); }


/**********************
仿真器
**********************/
Orbital_Solver::Orbital_Solver() {
    _mdlEarth = new MConstant(&_sim1, Mat(3, 1));
    _sim1.Set_SimStep(0.01);
    _sim1.Set_EnableStore(false);
}
void Orbital_Solver::Init() {
    _sim1.Initialize();
    // _sim1.Print_Modules();
}
void Orbital_Solver::Simulate(int cnt) {
    for (int i = 0; i < cnt; i++) {
        _orbEarth.Update_TrueAnomaly(_sim1.Get_t());
        _mdlEarth->Set_OutValue(_orbEarth.Update_Position());
        _sim1.Simulate_OneStep();
    }
}
void Orbital_Solver::Reset() {
    _sim1.Simulation_Reset();
}
void Orbital_Solver::Add_Satellite(RadiusVelocity rv) {
    SubsysSatellite *stlt = new SubsysSatellite(&_sim1, "stlt"+std::to_string(_stlts.size()));
    stlt->Set_initRV(rv);
    _sim1.connectM(_mdlEarth, stlt, 0);
    _stlts.push_back(stlt);
}
void Orbital_Solver::Add_Planet(Mat orbitalElement) {
    _orbEarth.Initialize(muSun, orbitalElement);
    _orbEarth.Update_TrueAnomaly(0);
    _orbEarth.Update_Velocity();
    _mdlEarth->Set_OutValue(_orbEarth.Update_Position());
}
Vector3 Orbital_Solver::Get_PlanetPosition() {
    Vector3 ans;
    ans.x = (float)_orbEarth._position.at(0, 0);
    ans.y = (float)_orbEarth._position.at(1, 0);
    ans.z = (float)_orbEarth._position.at(2, 0);
    return ans;
}
Vector3 Orbital_Solver::Get_SatellitePosition(int n) {
    Mat ans = _stlts[n]->Get_Position();
    return (Vector3){(float)ans.at(0, 0),(float)ans.at(1, 0),(float)ans.at(2, 0)};
}
Vector3 Orbital_Solver::Get_SatelliteVelocity(int n) {
    Mat ans = _stlts[n]->Get_Velocity();
    return (Vector3){(float)ans.at(0, 0),(float)ans.at(1, 0),(float)ans.at(2, 0)};
}
