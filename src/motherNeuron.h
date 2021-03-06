#ifndef MOTHERNEURON_H
#define MOTHERNEURON_H

#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>
#include <QMutex>
#include "encodeDecodeMsg.h"
#include "scheme/mainGraphics.h"
struct DataSynapsys
{
    QString idGlobal;
    int type;
    QString ipmTarget;
    quint16 port;
    double w;
    QString fx_numberTxt;       //fx's value Text
    QString fx_unitMeasureTxt;  //fx's unit's name
};

struct DataStructNeuron
{
    QString id;
    int typeNeuron; //0=none, 1=MotherNeuron, 2=Generator, 3=Normal ===>> To locate the communication port
    int N;
    bool sending;
    int localRemote;
    QVector <DataSynapsys> synapsys;

};
typedef QMap <QString,DataStructNeuron> typeDataNeurons;


class MotherNeuron : public QDialog
{
    Q_OBJECT

public:
    explicit MotherNeuron(QWidget *parent = nullptr, MainGraphics *schemeNeuron=nullptr);
    void addNeuron(int typeNeuron, QString ip,QString id, int N, int localRemote);
    void addSynapse(QString idGlobal, QString ipmSourceNeuron, QString ipmTarget, int type, quint16 port, double w, QString fx_numberTxt);
    typeDataNeurons dataNeurons;

private slots:
    void processPendingDatagramsPromiscuous();

private:
    void sendPromiscuousMsg(QString msg);   // TODO: Delete
    void sendTestMessage();                 // TODO: Delete
    void publicAMessage(QString msg, QString group, quint16 port);
    void createNeuron(EncodeDecodeMsg *msg);

    QLabel *statusLabel = nullptr;


    QUdpSocket udpSocket4_allNeurons;       //Listen to any neuron.
    QHostAddress groupAddress4_allNeurons;  //I'm listening to all neurons on a Port and over an IPmulticast


    QUdpSocket udpSocket4;      //To public With my IPmulticast and a Port
    QHostAddress groupAddress4; //This is my IpMulticast group

    MainGraphics *schemeNeuron; //Pointer to graphic environment

};

#endif
