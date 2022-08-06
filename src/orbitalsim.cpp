#include "orbitalsim.hpp"

Orbital_Simulator::Orbital_Simulator() {
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
        double ans = u[0]*u[0] + u[1]*u[1] + u[2]*u[2];
        ans = sqrt(ans);
        return -5000.0 / (ans*ans*ans);
    });
    intrx->Set_InitialValue(15);
    intry->Set_InitialValue(0);
    intvx->Set_InitialValue(0);
    intvy->Set_InitialValue(20);
    sim1.Set_SimStep(0.01);
    sim1.Initialize();
    _simstep = sim1.Get_SimStep();
}

void Orbital_Simulator::Simulate(double time) {
    int stepcnt = (int)(time/_simstep/1000.0+0.5);
    for (int i=0; i<stepcnt; ++i)
        sim1.Simulate_OneStep();
}

Vector3 Orbital_Simulator::Get_R() {
    return Vector3{
        (float)(intrx->Get_OutValue()),
        (float)(intry->Get_OutValue()),
        (float)(intrz->Get_OutValue())};
}

Vector3 Orbital_Simulator::Get_V() {
    return Vector3{
        (float)(intvx->Get_OutValue()),
        (float)(intvy->Get_OutValue()),
        (float)(intvz->Get_OutValue())};
}

void Orbital_Simulator::Reset(void){
    sim1.Simulation_Reset();
}
