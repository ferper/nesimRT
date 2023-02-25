#ifndef NEURONSCHEME_H
#define NEURONSCHEME_H

#include "junctionsDraw.h"
#include "neuron.h"

#include <QWidget>

class Node;

namespace Ui {
class NeuronScheme;
}

class NeuronScheme : public QWidget
{
    Q_OBJECT

public:
    explicit NeuronScheme(QWidget *parent = nullptr);
    ~NeuronScheme();

    QList <Node*> vectorGraphicsNodes;
    QVector <JunctionsDraw*> vectorGraphicsJunction;
    QList <Neuron *> localNeurons; // Neurons created locally, to display the monitors

    int generateIdGlobalNeuron();
    int idGlobalNeuron; // It is passed to Neuron and it is passed through messages to newNeuronGraphics and newGenerator
    int idGlobalSynapse; // It is passed to newsynapseGraphic by reference and there it is modified as many times as synapses are created

    bool existIp(QString ip);
    QString generateIPReal(QString type);
    QString generateIPSimulated(QString type);
    void restaurateIPSimulated(QString type); //Al cancelar de newNeuron o newGenerator

    bool sceneBeSaved=false;
    QString fileNameToSave="";
    QGraphicsScene *scene;

public slots:
    void removeALLScenary();
    int getNumberItemOf(int typeOfItem);
    void processPendingDatagrams();

private slots:
    void emptyVectors();

    void on_btExport_clicked();

    void on_btExit_clicked();

    void on_btLoad_clicked();

    void on_btNew_clicked();

    void on_btSave_clicked();

    void on_btSaveAs_clicked();

    void on_btNewNeuron_clicked();

    void on_btNewGenerator_clicked();

    void on_btNewSynapse_clicked();

    void on_btRemoveNeuron_clicked();

private:
    Ui::NeuronScheme *ui;

    // Counters to build a multicast IP (MDHCP)
    int DHCP_NORMAL_A,DHCP_NORMAL_B,DHCP_NORMAL_C,DHCP_NORMAL_D;
    int DHCP_GENERATOR_A,DHCP_GENERATOR_B,DHCP_GENERATOR_C,DHCP_GENERATOR_D;

    int sim_DHCP_NORMAL_A,sim_DHCP_NORMAL_B,sim_DHCP_NORMAL_C,sim_DHCP_NORMAL_D;
    int sim_DHCP_GENERATOR_A,sim_DHCP_GENERATOR_B,sim_DHCP_GENERATOR_C,sim_DHCP_GENERATOR_D;

    QUdpSocket udpSocket; // From to send a message
    QHostAddress groupAddress; // I'm listening

    QTimer *timerEmpty;
    void emptyLocalNeurons();
    void emptyVectorGraphicsEdges();
    void emptyVectorGraphicsNodes();

    void sendMsg(QString message, QString ipmTarget, int port);
};

#endif // NEURONSCHEME_H
