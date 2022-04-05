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

    //TODO: Borrar esto

    std::chrono::high_resolution_clock::time_point start;


    QString MAC;

    QString id;
    QString label;
    float posX;
    float posY;
    double get_IexcCurrent();
    double get_IinhCurrent();
    double get_VCurrent();
    long long int muestra;
    Widget *monitor;
    double IexcCurrent;
    double IinhCurrent;
    double VCurrent;
    bool dataIsAvailable;
    int NumberNeuronsGroup; // If N==1 the neuron es alone. N>1 it's a group
    QString ipmSource;      // Neuron multicast IP. So that others can subscribe to it
    //QString ipmMother;      // Mother Neuron multicast IP
    bool enableDataGeneralMonitor;
    double we; // Current value for synapsys
    double wi; // Current value for synapsys
    QString fx_numberTxt; // fx's value Text
    QString fx_unitMeasureTxt; // fx's unit's name

    int typeNeuron; // 0= none, 1= Mother, 2= Generator, 3= Normal
    void * FormDialog;
    QVector <Synapse *> Vsynapse;
    int localRemote; // Neuron created locally or remotely
    QMutex mutexNeuron;

    int linea;
    bool spkOnOff_exc;
    bool spkOnOff_inh;
public slots:
    void paintLocalMonitor();
    void sendDataToGeneralMonitor(bool spiking);
    int get_NSynapses();
    void calculateValues();
    void quit();
    void set_At(double value);


private slots:
    void sendRequestIP_DHCP();
    void processPendingDatagrams();
    void killme();

protected:
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void killme(bool showMessageBox);
    void liveNeuron();
    void calculateIexc();
    void calculateIinh();
    void calculateV();
    void generateSpike();
    void sendMsg(QString msg, quint16 port);
    QString getLocalMAC();
    double H;
    double Iexc_prior;
    double Iinh_prior;
    double V_prior;
    bool Iexc_enabled;
    bool Iinh_enabled;
    bool V_enabled;
    bool isBuilded; // The Neuron is builded. It's operative

    quint16 Motherneuron_port; // Listening port of the Mother Neuron
    quint16 SourcePort; // Port for connection with other neurons
    quint16 GeneralMonitorPort; // Port to send the calculated data to the General Monitor
    qlonglong nSpikes; // Number of the spikes produced by the neuron

    QLabel *statusLabel = nullptr;
    QUdpSocket udpSocket4_MotherNeuron; // From mother neuron
    QHostAddress groupAddress4_to_MotherNeuron; // The mother neuron is listened to

    QUdpSocket udpSocket4_sender; // To public a SPIKE
    QUdpSocket udpSocket4_senderMonitor; // To public data calculated to General Monitor
    QHostAddress groupAddress4_to_Public; // Other neurons listen to me

    //QUdpSocket udpSocket4_Promiscuous; // To receive datas in promiscuous mode from a Mother Neuron's port. Ejemplo: Ip from DHCP

    QFile file;
    QTextStream *out;
};

#endif
