#include "model3d.hpp"

Model3D::Model3D(int id): _id(id) {
    _scale = (Vector3) { 1.0f, 1.0f, 1.0f};
    _rotAxis = (Vector3) { 0.0f, 0.0f, 1.0f};
    _rotAngle = 0.0f;
    Reset_Model();
}
void Model3D::Load_Model(const char* objfile, const char* pngfile) {
    _model = LoadModel(objfile);
    Texture2D img = LoadTexture(pngfile);
    _model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = img;
}
void Model3D::Update_Model(bool pushPoint) {
    DrawModelEx(_model, _pos, _rotAxis, _rotAngle, _scale, WHITE);
    if (!_draw) return;
    Vector3 posstart, posend;
    std::list<Vector3>::iterator it = _Points.begin();
    posstart = *it;
    it++;
    for (; it != _Points.end(); ++it) {
        posend = *it;
        DrawLine3D(posstart, posend, _oparam._color);
        posstart = posend;
    }
    if (!pushPoint) return;
    _oparam._cnt++;
    if (_oparam._cnt<_oparam._PointInterval) return;
    _oparam._cnt = 0;
    _Points.push_back(_pos);
    _Points.pop_front();
}
void Model3D::Reset_Model() {
    _Points.clear();
    for (int i = 0; i < _oparam._PointNum; i++)
        _Points.push_back(_initpos);
}
void Model3D::Set_Scale(float scale) {
    _scale = (Vector3) { scale, scale, scale };
}
