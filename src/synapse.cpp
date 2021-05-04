#include "synapse.h"
#include <iostream>
#include <QString>
#include <QtNetwork>
#include <QtWidgets>
#include <QtNetwork>
#include "encodeDecodeMsg.h"
Synapse::Synapse(int *N,int idGlobalSynapse, QString ipmS, QString ipmT, quint16 port, int type, double *Iinc, double w, QString fx_numberTxt,QString fx_unitMeasureTxt, bool *startICalculate, bool *startVCalculate, QTimer *timer, QMutex *mutexNeuron, long long int *muestra, QTextStream *out, bool *spkOnOff)
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

   groupAddress4=QHostAddress(ipmT);
   udpSocket4.bind(QHostAddress(ipmT), port_target, QUdpSocket::ShareAddress);

   udpSocket4.joinMulticastGroup(groupAddress4);
   connect(&udpSocket4, SIGNAL(readyRead()),this, SLOT(processPendingDatagrams()));
}

void Synapse::processPendingDatagrams(){

    QByteArray datagram;

    while (udpSocket4.hasPendingDatagrams()) {
        datagram.resize(int(udpSocket4.pendingDatagramSize()));
        udpSocket4.readDatagram(datagram.data(), datagram.size());
        data=datagram.constData();
        if (data=="SPIKE") {
            mutexNeuron->lock();
           (*Iinc)+=(*N)*(w); //Se incrementa Iexc ó Iinh es un puntero
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

