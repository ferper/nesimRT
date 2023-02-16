#ifndef NEURON_CUBALIF_H
#define NEURON_CUBALIF_H
#include "../neuron.h"


class neuron_cubalif: public Neuron
{
public:

    neuron_cubalif(QWidget *parent = nullptr, int NumberNeuronsGroup=1, QString label="", float posX=350.0, float posY=350.0, QString ip="", int idGlobalNeuron=1, int typeNeuron=0,  int localRemote=(int)LOCAL_NEURON, Parameters *p=nullptr, double w_default=1, QString fx="1e-09",double V_prior=0,double IexcCurrent=0,double IinhCurrent=0,double VCurrent=0);

//public slots:
    void sendDataToGeneralMonitor(bool spiking);
    void paintLocalMonitor();

private:
    double V_prior;
    void calculateIinh(float dt=1.0) override;
    void calculateIexc(float dt=1.0) override;
    void calculateV(float dt=1.0) override;

};

#endif // NEURON_CUBALIF_H
