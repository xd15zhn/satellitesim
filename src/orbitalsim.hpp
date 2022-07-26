#include "simucpp.hpp"
using namespace simucpp;
using namespace zhnmat;

class Orbital_Simulator
{
public:
    Orbital_Simulator() {
        SUIntegrator(intrx, &sim1);
        SUIntegrator(intry, &sim1);
        SUIntegrator(intrz, &sim1);
        SUIntegrator(intvx, &sim1);
        SUIntegrator(intvy, &sim1);
        SUIntegrator(intvz, &sim1);
        SUProduct(prodax, &sim1);
        SUProduct(proday, &sim1);
        SUProduct(prodaz, &sim1);
        SUFcnMISO(misomur, &sim1);
        sim1.connectU(prodax, intvx);
        sim1.connectU(proday, intvy);
        sim1.connectU(prodaz, intvz);
        sim1.connectU(intvx, intrx);
        sim1.connectU(intvy, intry);
        sim1.connectU(intvz, intrz);
        sim1.connectU(intrx, misomur);
        sim1.connectU(intry, misomur);
        sim1.connectU(intrz, misomur);
        sim1.connectU(misomur, prodax);
        sim1.connectU(misomur, proday);
        sim1.connectU(misomur, prodaz);
        sim1.connectU(intrx, prodax);
        sim1.connectU(intry, proday);
        sim1.connectU(intrz, prodaz);
        misomur->Set_Function([](double *u){
            Vector3d r(u[0], u[1], u[2]);
            double ans = r.norm2();
            return -5000.0 / (ans*ans*ans);
        });
        intrx->Set_InitialValue(15);
        intry->Set_InitialValue(0);
        intvx->Set_InitialValue(0);
        intvy->Set_InitialValue(12);
        sim1.Set_SimStep(0.01);
        sim1.Initialize();
        _simstep = sim1.Get_SimStep();
    };
    Vector3d Simulate(double time) {
        int stepcnt = (int)(time/_simstep/1000.0+0.5);
        for (int i=0; i<stepcnt; ++i)
            sim1.Simulate_OneStep();
        float x = intrx->Get_OutValue();
        float y = intry->Get_OutValue();
        float z = intrz->Get_OutValue();
        return Vector3d(x, y, z);
    }
    void Reset(void){
        sim1.Simulation_Reset();
    }
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
