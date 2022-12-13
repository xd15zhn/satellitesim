#ifndef ORBITALSIM_H
#define ORBITALSIM_H

#include <cmath>
#include "utils.h"
#include "simucpp.hpp"
using namespace simucpp;
using namespace zhnmat;
using namespace std;

constexpr double muSun = 132706538114E-10;
constexpr double muEarth = 398601E-10;


/*********************
记录中心天体μ值和椭圆/双曲线轨道的轨道六要素，抛物线不适用
轨道六要素中的前5个为常数，真近点角记录为初始角度，之后根据经过的时间更新
**********************/
struct Orbital_Elements {
    Orbital_Elements(double mu=1, Mat orbitalElement=Mat(6, 1));
    void Initialize(double mu, Mat orbitalElement);
    void Update_TrueAnomaly(double t);  // 计算经过时间[t]后的真近点角
    Mat Update_Position();  // 根据更新后的真近点角计算位置向量
    Mat Update_Velocity();  // 根据更新后的真近点角计算速度向量
    double _mu;  // 中心天体μ值
    double _kv;  // 用于求速度向量的常数系数
    double _omega;  // 轨道平均角速度
    double _ta;  // 真近点角(true anomaly)
    double _a, _e, _i, _n, _w, _f;  // 轨道六要素._f为初始真近点角
    Mat _RT;  // i,n,w确定的坐标变换矩阵
    Mat _position;  // 位置向量
    Mat _velocity;  // 速度向量
};


/**********************
一个卫星的仿真子系统，需要添加到仿真器中
**********************/
class SubsysSatellite: public PackModule {
public:
    SubsysSatellite(Simulator *sim, std::string name);
    ~SubsysSatellite() {};
    virtual PMatModule Get_InputBus(int n) const;
    void Set_initRV(RadiusVelocity rv);
    Mat Get_Position();
    Mat Get_Velocity();
    MGain *_gain=nullptr;
    MFcnMISO *_miso=nullptr;
    MStateSpace *_mssV=nullptr;
    MStateSpace *_mssR=nullptr;
    std::string _name;
};
typedef SubsysSatellite* PSubsysSatellite;
typedef MConstant* PMConstant;


/**********************
轨道仿真器
**********************/
class Orbital_Solver {
public:
    Orbital_Solver();
    void Init();  // 仿真器初始化
    void Simulate(int cnt);  // 仿真[cnt]步
    void Reset();  // 仿真器复位
    void Add_Planet(Mat orbitalElement);  // 通过轨道六要素添加行星
    void Add_Satellite(RadiusVelocity rv);  // 通过初始位置和速度添加卫星
    Vector3 Get_PlanetPosition(void);  // 获取行星位置
    Vector3 Get_SatellitePosition(int n);  // 获取卫星位置
    Vector3 Get_SatelliteVelocity(int n);  // 获取卫星速度
    std::vector<PSubsysSatellite> _stlts;
    Orbital_Elements _orbEarth;
    MConstant* _mdlEarth;
    Simulator _sim1;
};

#endif // ORBITALSIM_H
