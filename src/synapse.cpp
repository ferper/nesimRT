#include "synapse.h"
#include <iostream>
#include <QString>
#include <QtNetwork>
#include <QtWidgets>
#include <QtNetwork>
#include "encodeDecodeMsg.h"
Synapse::Synapse(int *N,
                 int idGlobalSynapse,
                 QString ipmS, QString ipmT,
                 quint16 port,
                 int type,
                 double *Iinc,
                 double w,
                 QString fx_numberTxt,
                 QString fx_unitMeasureTxt,
                 bool *startICalculate,
                 bool *startVCalculate,
                 QTimer *timer,
                 QMutex *mutexNeuron,
                 long long int *muestra,
                 QTextStream *out,
                 bool *spkOnOff)
{
   this->N=N;
   this->idGlobalSynapse=idGlobalSynapse;
   this->ipmS=ipmS;
   this->ipmT=ipmT;
   this->port_target=port;
   this->type=type;
   this->Iinc=Iinc;
   this->w=w*fx_numberTxt.toDouble();
   this->fx_numberTxt=fx_numberTxt;
   this->fx_unitMeasureTxt=fx_unitMeasureTxt;
   this->startICalculate=startICalculate;
   this->startVCalculate=startVCalculate;
   this->timer=timer;
   this->mutexNeuron=mutexNeuron;
   this->muestra=muestra;
   this->out=out;
   this->spkOnOff=spkOnOff;

   pre_trace=0.0;
   post_trace=0.0;

   pre_spiketime = std::chrono::high_resolution_clock::now();
   post_spiketime = std::chrono::high_resolution_clock::now();

   groupAddress4=QHostAddress(ipmS);
   udpSocket4.bind(QHostAddress::AnyIPv4, port_target, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
   udpSocket4.joinMulticastGroup(groupAddress4);
   connect(&udpSocket4, SIGNAL(readyRead()),this, SLOT(processPendingDatagrams()));
}

// STODO: Implementar un metodo a la sinapsis que sea para indicar que se ha recibido un spike post-synaptic. La neuron que tiene la sinapsis llama
// a este metodo y la sinapsis actualiza el valor de la variable del tiempo (post-sinaptico)
// TODO: Usar mutex para que no se modifique o lea el valor concurrentemente
// Cuando se recibe un spike presinaptico, se incrementa el valor de la sinapsis
// Se guarda el valor de la variable presinaptica
// Se incrementa el valor del peso de la variable (w)
// Cuando se recibe un spike postsinaptico, se guarda el valor de la variable postinaptica
// Se incrementa el valor de la variable w
void Synapse::postSpike(){
    auto actual_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float,std::milli> dt_pre = pre_spiketime - actual_time;

    pre_trace = pre_trace*exp(dt_pre.count()/1000);
    //w += pre_trace*fx_numberTxt.toDouble();

    std::chrono::duration<float,std::milli> dt_post = post_spiketime - actual_time;
    post_trace = post_trace*exp(dt_post.count()/1000);
    post_trace -= 0.05; // TODO: Put as parameter as well as time constant
    post_spiketime = actual_time;
}

void Synapse::processPendingDatagrams(){

    QByteArray datagram;

    while (udpSocket4.hasPendingDatagrams()) {
        datagram.resize(int(udpSocket4.pendingDatagramSize()));
        udpSocket4.readDatagram(datagram.data(), datagram.size());
        data=datagram.constData();
        if (data=="SPIKE") {
            mutexNeuron->lock();
            (*Iinc)+=(*N)*(w); // Iexc or Iinh is incremented in a pointer
            *spkOnOff=true;

            /********* LEARNING ***********/
            auto actual_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float,std::milli> dt_pre = pre_spiketime - actual_time;

            pre_trace = pre_trace*exp(dt_pre.count()/20);
            pre_trace += 0.1; // TODO: Put as parameter as well as time constant

            std::chrono::duration<float,std::milli> dt_post = post_spiketime - actual_time;
            post_trace = post_trace*exp(dt_post.count()/20);

            //w += post_trace*fx_numberTxt.toDouble();
            pre_spiketime = actual_time;
            /******************************/

            mutexNeuron->unlock();
            (*startICalculate)=true;
            (*startVCalculate)=true;
            (*timer).start();
        }
   }
}

void Synapse::deleteSynapse(){
   delete this;
}

