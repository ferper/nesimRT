#ifndef NEURON_H
#define NEURON_H

#include <QDialog>
#include <QHostAddress>
#include <QUdpSocket>
#include <QVector>
#include "synapse.h"
#include "parameters.h"
#include "Widget.h"
#include "config.h"
#define SPIKE "SPIKE"

class QLabel;
class Neuron : public QDialog
{
    Q_OBJECT

public:

    explicit Neuron(QWidget *parent = nullptr, int NumberNeuronsGroup=1, QString label="", float posX=350.0, float posY=350.0, QString ip="", int idGlobalNeuron=1, int typeNeuron=0, int localRemote=(int)LOCAL_NEURON, Parameters *p=nullptr, double w_default=1, QString fx="1e-09");
    ~Neuron();

    Parameters *p;
    QTimer *timer;
    QTimer *timer_RequestIP;


    QString MAC;

    QString id;
    QString label;
    int NumberNeuronsGroup; // If N==1 the neuron es alone. N>1 it's a group
    QString ipmSource;      // Neuron multicast IP. So that others can subscribe to it
    float posX;
    float posY;
    double we; // Current value for synapsys
    double wi; // Current value for synapsys
    QString fx_numberTxt; // fx's value Text
    QString fx_unitMeasureTxt; // fx's unit's name
    bool isBuilded; // The Neuron is builded. It's operative
    Widget *monitor;
    bool dataIsAvailable;
    long long int muestra;
    int typeNeuron; // 0= none, 1= Mother, 2= Generator, 3= Normal
    void * FormDialog;
    QVector <Synapse *> Vsynapse;
    int localRemote; // Neuron created locally or remotely
    QMutex mutexNeuron;
    bool spkOnOff_exc;
    bool spkOnOff_inh;
    bool enableDataGeneralMonitor;



public slots:
    virtual void paintLocalMonitor();
    virtual void sendDataToGeneralMonitor(bool spiking);
    int get_NSynapses();
    virtual void calculateValues();
    void quit();
    void set_At(double value);


private slots:
    void sendRequestIP_DHCP();
    void processPendingDatagrams();
    void killme();

protected:
    double Iexc_prior;
    double Iinh_prior;
    bool Iexc_enabled;
    bool Iinh_enabled;
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    double H;
    quint16 GeneralMonitorPort; // Port to send the calculated data to the General Monitor
    QUdpSocket udpSocket4_senderMonitor; // To public data calculated to General Monitor
    void generateSpike();

private:
    void killme(bool showMessageBox);
    void liveNeuron();
    void sendMsg(QString msg, quint16 port);
    QString getLocalMAC();
    bool V_enabled;
    quint16 Motherneuron_port; // Listening port of the Mother Neuron
    quint16 SourcePort; // Port for connection with other neurons

    qlonglong nSpikes; // Number of the spikes produced by the neuron

    QLabel *statusLabel = nullptr;
    QUdpSocket udpSocket4_MotherNeuron; // From mother neuron
    QHostAddress groupAddress4_to_MotherNeuron; // The mother neuron is listened to

    QUdpSocket udpSocket4_sender; // To public a SPIKE
    QHostAddress groupAddress4_to_Public; // Other neurons listen to me

    //QUdpSocket udpSocket4_Promiscuous; // To receive datas in promiscuous mode from a Mother Neuron's port. Ejemplo: Ip from DHCP

    QFile file;
    QTextStream *out;
};

#endif
