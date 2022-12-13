#include "orbitalsim.hpp"
#include "model3d.hpp"
#include "utils.h"
#include <iostream>

constexpr double omegaEarth = PI/43200.0;  // 地球自转角速度
constexpr float EarthScale = 0.6e-2;
constexpr float StltScale = 1e-5;
constexpr float SunScale = 20;

Vector3 Mat_To_Vector3(Mat m) {
    return (Vector3){(float)m.at(0, 0), (float)m.at(1, 0), (float)m.at(2, 0)};
}

bool Pause() {
    static u8 state = 0x01;
    if (IsKeyDown(KEY_SPACE)) state |= 0x02;
    if (!IsKeyDown(KEY_SPACE) && (state & 0x02)) { state &=~ 0x02; state ^= 0x01; }
#if 1  // 抬起一次空格键暂停，再抬起一次继续
    if (state & 0x01) return true;
    else return false;
#else  // 抬起一次空格键走一步
    if ((state & 0x01) == (state >> 2)) { state ^= 0x04; return false; }
    else return true;
#endif
}

int main(void) {
    vector<RadiusVelocity> stltRV;
    RadiusVelocity earthRV;
    RadiusVelocity CameraPosTgt = {{0, -0.02, 0.02}, {0, 0, 0}};
    stltRV.push_back({{-0.02, 0, 0}, {0, 0.05, 0}});
    stltRV.push_back({{-0.02, 0, 0}, {0, 0.04, 0}});
    stltRV.push_back({{0, 0, 0.02}, {0, 0.04, 0}});
    // stltRV.push_back({{0.006378, 0, 0}, {0, 0.168, 0}});

    /* 初始化仿真器 */
    Orbital_Solver solver;
    solver.Add_Planet(Mat(6, 1, vecdble{
        149.6, 0, 0, 0, 0, 0}));
    cout << solver._orbEarth._position << endl;
    cout << solver._orbEarth._velocity << endl;
    earthRV.r = Mat_To_Vector3(solver._orbEarth._position);
    earthRV.v = Mat_To_Vector3(solver._orbEarth._velocity);
    int stltcnt = stltRV.size();
    for (int i = 0; i < stltcnt; i++) {
        stltRV[i].r = Vector3Add(stltRV[i].r, earthRV.r);
        stltRV[i].v = Vector3Add(stltRV[i].v, earthRV.v);
        solver.Add_Satellite(stltRV[i]);
    }
    solver.Init();

    /*初始化场景*/
    int StepsPerFrame = 1;  // 每帧仿真的步数
    double steptime = 100*StepsPerFrame;  // 每帧仿真对应的实际时间
    Camera camera;
    SetWindowMonitor(1);
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	SetConfigFlags(FLAG_FULLSCREEN_MODE);
    SetTargetFPS(60);
    InitGraph(0, 0, "Universe");
    SetReferenceFrame(earthRV);
	Init_Camera(&camera, CameraPosTgt);
    Init_Skybox();

    /*加载卫星*/
    // Vector3 ans;
    Model3D *satellites = new Model3D[stltcnt];
    for (int i = 0; i < stltcnt; i++) {
        satellites[i].Load_Model("resources/satellite.obj", "resources/satellite.png");
        satellites[i].Set_Scale(StltScale);
        satellites[i]._oparam._PointInterval = 2;
        satellites[i]._oparam._cnt = 1024;
        satellites[i]._initpos = stltRV[i].r;
        satellites[i]._id = i;
        satellites[i].Reset_Model();
    }
    satellites[0]._oparam._color = RED;
    satellites[1]._oparam._color = GREEN;
    satellites[2]._oparam._color = BLUE;
    // satellites[3]._oparam._color = YELLOW;
    /*加载地球*/
    Model3D modelEarth;
    modelEarth.Load_Model("resources/sphere.obj", "resources/earth.png");
    modelEarth.Set_Scale(EarthScale);
    modelEarth._rotAxis = (Vector3){0.3987490689252462, 0, 0.917060074385124};
    modelEarth._initpos = solver.Get_PlanetPosition();
    modelEarth._oparam._PointInterval = 100;
    modelEarth._id = stltcnt;
    modelEarth.Reset_Model();
    /*加载太阳*/
    Model modelSun = LoadModel("resources/sphere.obj");
    Texture2D imgSun = LoadTexture("resources/sun.png");
    modelSun.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = imgSun;

    /*添加全局变量*/
    int cnt = 0;
    float calcfps=0, calccpu=0, calcspd=0;
    int showfps=0, showcpu=0;
    float t = 0;
    float data[5];
    u8 keyState = 0x01;
    cout.precision(8);

    /*场景循环*/
    while (!WindowShouldClose()) {
        modelEarth._pos = solver.Get_PlanetPosition();
        modelEarth._rotAngle = omegaEarth*t;
        for (int i = 0; i < stltcnt; i++) {
            satellites[i]._pos = solver.Get_SatellitePosition(i);
            if (Vector3Length(Vector3Subtract(satellites[i]._pos, modelEarth._pos)) < 0.005) {
                cout << "Collisioned! " << i << endl;
            }
        }
        earthRV.r = Mat_To_Vector3(solver._orbEarth._position);
        earthRV.v = Mat_To_Vector3(solver._orbEarth._velocity);
		Update_Camera(&camera, earthRV);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode3D(camera);
                Update_Skybox();
                DrawModel(modelSun, (Vector3){ 0.0f, 0.0f, 0.0f }, SunScale, WHITE);
                modelEarth.Update_Model(!(keyState & 0x01));
                for (int i = 0; i < stltcnt; i++)
                    satellites[i].Update_Model(!(keyState & 0x01));
            EndMode3D();
            cnt++;
            calcfps += GetFPS();
            calccpu += GetCPUusage();
            calcspd += GetCameraSpeed();
            if (cnt>=30) {
                cnt = 0;
                showfps = (int)(calcfps / 30.0 + 0.5);
                showcpu = (int)(calccpu / 30.0 + 0.5);
                data[0] = StepsPerFrame;
                data[1] = calcspd / 30.0;
                data[2] = camera.position.x;
                data[3] = camera.position.y;
                data[4] = camera.position.z;
                Print(data);
                calcfps = calccpu = calcspd = 0;
            }
            DrawText(TextFormat("FPS: %2i", showfps), 0, 0, 20, LIME);
            DrawText(TextFormat("CPU: %2i %%", showcpu), 0, 30, 20, LIME);
            DrawText(TextFormat("Day: %.1f", t/86400.0), 0, 60, 20, LIME);
        EndDrawing();

        keyState = Pause();
        if (Pause()) keyState |=  0x01;
        else         keyState &=~ 0x01;
        if (IsKeyDown(KEY_EQUAL)) {
            StepsPerFrame++; steptime = 100*StepsPerFrame; }
        if (IsKeyDown(KEY_MINUS)) {
            StepsPerFrame--;
            if (StepsPerFrame<1) StepsPerFrame = 1;
            steptime = 100*StepsPerFrame;
        }
        if (keyState) continue;
        solver.Simulate(StepsPerFrame);
        t += steptime;
    }
    CloseGraph();
    return 0;
}
