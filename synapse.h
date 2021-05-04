#ifndef SYNAPSE_H
#define SYNAPSE_H

#include <QHostAddress>
#include <QUdpSocket>
#include <QtCore>

class Synapse:public QObject
{
   Q_OBJECT

public:

    explicit Synapse(int *N,int idGlobalSynapse,QString ipmS, QString ipmT, quint16 port, int type, double *Iinc, double w, QString fx_numberTxt, QString fx_unitMeasureTxt, bool *startICalculate, bool *startVCalculate,  QTimer *timer, QMutex *mutexNeuron,long long int *muestra, QTextStream *out, bool *spkOnOff);


    int *N;
    int idGlobalSynapse; //Id global of the all synapsys created on the system

    QString ipmT; //Ip multicast of the Target Neuron
    QString ipmS; //Ip multicast of the Source Neuron
    quint16 port_target; //Neuron's Target port
    int type; //Inhibidora o excitadora
    double *Iinc; //Variable to increment, can be Iexc or Iinh because it's a pointer
    bool *startICalculate; //Flag para empezar a autocalcular Iexc ó Iinh
    bool *startVCalculate;
    QTimer *timer;
    double w; //Current value for synapsys with others neuron

    QString fx_numberTxt; //fx's value Text
    QString fx_unitMeasureTxt; //fx's unit's name
    void deleteSynapse();

    QUdpSocket udpSocket4;
    QHostAddress groupAddress4;
    QMutex *mutexNeuron;

    long long int *muestra;
    QTextStream *out;
    bool *spkOnOff;

private:
    QString data; //Receive data

private slots:
    void processPendingDatagrams();
};

#endif // SYNAPSE_H
