#include "synapse.h"
#include <iostream>
#include <QString>
#include <QtNetwork>
#include <QtWidgets>
#include <QtNetwork>
#include "encodeDecodeMsg.h"
Synapse::Synapse(int *N,int idGlobalSynapse, QString ipmS, QString ipmT, quint16 port, int type, double *Iinc, double w, QString fx_numberTxt,QString fx_unitMeasureTxt, bool *startICalculate, bool *startVCalculate, QTimer *timer,QTimer *pre_siaptico,QTimer *post_sinaptico, QMutex *mutexNeuron, long long int *muestra, QTextStream *out, bool *spkOnOff)
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
   this->pre_sinaptico=pre_sinaptico;
   this->post_sinaptico=post_sinaptico;
   this->mutexNeuron=mutexNeuron;
   this->muestra=muestra;
   this->out=out;
   this->spkOnOff=spkOnOff;

   // TODO: La sinapsis tiene que tener un estado interno de cuando se ha recibido un spike pre-sinaptico
   // TODO: La sinapsis tiene que tener un estado interno de cuando se ha recibido un spike post-sinaptico

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
void Synapse::updateTime(){
    post_sinaptico->start();

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
            //TODO: Leer el valor del spike presinaptico y del postsinaptico
            pre_sinaptico->stop();
            pre_sinaptico->stop();
            //int diferencia =
            // TODO: Actualizar el valor de w de acorde a la formula de STDP
            // TODO: Actualizar el valor del tiempo pre-sinaptico
            pre_sinaptico->start();
            *spkOnOff=true;
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

