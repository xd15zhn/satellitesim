#ifndef MODEL3D
#define MODEL3D
#include <list>
#include <cmath>
#include "raylib.h"

/**********************
画轨道曲线相关参数
**********************/
struct OrbitPointParam {
    int _PointNum=256;  // 画多少个点
    int _PointInterval=1;  // 多少次更新位置后画一个点
    int _cnt=0;  // 更新位置计数器
    Color _color=RAYWHITE;  // 颜色
};

/**********************
一个3D模型，需要设置大小、位置、角度等
**********************/
class Model3D {
public:
    Model3D(int id=0);
    void Load_Model(const char* objfile, const char* pngfile);
    void Update_Model(bool pushPoint=true);
    void Reset_Model();
    void Set_Scale(float scale);
    OrbitPointParam _oparam;  // 画轨道曲线参数
    Model _model;  // 3D模型和贴图等
    Vector3 _scale;  // 3D模型缩放
    int _id=0;  //
    bool _draw=true;  // 是否画轨道曲线
    Vector3 _pos, _initpos;  // 当前位置，起始位置
    float _rotAngle;  // 旋转角
    Vector3 _rotAxis;  // 旋转轴
    std::list<Vector3> _Points;  // 轨道曲线上的点
};

#endif // MODEL3D
