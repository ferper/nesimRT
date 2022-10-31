#ifndef NEURON_ADEXLIF_H
#define NEURON_ADEXLIF_H
#include "../neuron.h"


class neuron_adexlif: public Neuron
{
public:

    neuron_adexlif(QWidget *parent = nullptr, int NumberNeuronsGroup=1, QString label="", float posX=350.0, float posY=350.0, QString ip="", int idGlobalNeuron=1, int typeNeuron=0, int localRemote=(int)LOCAL_NEURON, Parameters *p=nullptr, double w_default=1, QString fx="1e-09",double V_prior=0,double IexcCurrent=0,double IinhCurrent=0,double VCurrent=0);

    double get_IexcCurrent();
    double get_IinhCurrent();
    double get_VCurrent();
    double IexcCurrent;
    double IinhCurrent;
    double VCurrent;

//public slots:
    void sendDataToGeneralMonitor(bool spiking);
    void paintLocalMonitor();
    void calculateValues();


private:

    double V_prior;
    void calculateIinh();
    void calculateIexc();
    void calculateV();

};
#endif // NEURON_ADEXLIF_H
