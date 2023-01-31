#include <QtWidgets>
#include <QtNetwork>

#include "neuron.h"
#include "synapse.h"
#include "parameters.h"
#include "Widget.h"

#include <iostream>
#include <limits>
#include <iomanip>
#include <math.h>
#include "config.h"
#include "encodeDecodeMsg.h"
#include "spikeGenerator.h"
#include "neuron_cubalif.h"

using namespace std;


neuron_cubalif::neuron_cubalif(QWidget *parent, int NumberNeuronsGroup, QString label, float posX, float posY, QString ip, int idGlobalNeuron, int typeNeuron,int localRemote, Parameters *p, double w_default, QString fx,
        double V_prior,double IexcCurrent,double IinhCurrent,double VCurrent): Neuron(parent,NumberNeuronsGroup,label,posX,posY,ip,idGlobalNeuron,typeNeuron,localRemote,p,w_default,fx)
{

    this->V_prior=0;
    this->IexcCurrent=0;
    this->IinhCurrent=0;
    this->VCurrent=0;

}

void neuron_cubalif::sendDataToGeneralMonitor(bool spiking){

    QString spike="0";
    if (spiking)
        spike="100000000";
    QString cadena=ipmSource+";"+spike+";"+QString::number((double)IexcCurrent*1e9)+";"+QString::number((double)IinhCurrent*1e9)+";"+QString::number((double)VCurrent*1e3)+";";
    QByteArray datagram = cadena.toStdString().c_str();
    QHostAddress groupAddress4=QHostAddress(IPM_NEURON_PROMISCUOUS);
    udpSocket4_senderMonitor.writeDatagram(datagram, groupAddress4, GeneralMonitorPort);
}

void neuron_cubalif::paintLocalMonitor(){
    if (!monitor)
        monitor= new Widget(nullptr,&ipmSource,&label, &dataIsAvailable,&muestra,&IexcCurrent,&IinhCurrent,&VCurrent,p,&Vsynapse,&NumberNeuronsGroup);
    monitor->show();
}

void neuron_cubalif::calculateIinh() {
   if (Iinh_enabled) {
      Iinh_prior=p->Iinh;
      p->Iinh=Iinh_prior+H*(-Iinh_prior/p->tau_i);
   }
}

void neuron_cubalif::calculateIexc(){
   if (Iexc_enabled) {
      Iexc_prior=p->Iexc;
      p->Iexc=Iexc_prior+H*(-Iexc_prior/p->tau_e);
   }
}

void neuron_cubalif::calculateV() {
       V_prior=p->V;
       //p->V=V_prior+H*((1/p->tau_v)*(-(V_prior-p->Vr)+p->At*exp((V_prior-p->Vrh)/p->At)+p->R*(Iexc_prior-Iinh_prior)));
       p->V=V_prior+H*((1/p->tau_v)*(-(V_prior-p->Vr)+p->R*(Iexc_prior-Iinh_prior)));
}
void neuron_cubalif::calculateValues(){
    bool spiking=false;
    dataIsAvailable=false;
    mutexNeuron.lock();
    for (int i=0; i<10; i++) {


        calculateV();
        if (p->V>p->Vth) { //SPIKE Generator
            p->V=p->Vr;
            spiking = true;
            generateSpike();
        }


        // Initial values for Iexc, Iinh y V
        IexcCurrent=p->Iexc;
        IinhCurrent=p->Iinh;
        VCurrent=p->V;
        calculateIexc();
        calculateIinh();
    }
    mutexNeuron.unlock();
    dataIsAvailable=true;
    if (enableDataGeneralMonitor)
        sendDataToGeneralMonitor(spiking);


}


double neuron_cubalif::get_IexcCurrent(){
    return IexcCurrent;
}
double neuron_cubalif::get_IinhCurrent(){
    return IinhCurrent;
}
double neuron_cubalif::get_VCurrent(){
   return VCurrent;
}
