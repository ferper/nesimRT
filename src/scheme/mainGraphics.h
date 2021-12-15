#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QPushButton>
#include "junctionsDraw.h"
#include "neuron.h"


class Node;

class MainGraphics : public QGraphicsView
{
   Q_OBJECT

public:
    MainGraphics(QWidget *parent = nullptr);
    void itemMoved();
    // Add a new neuron into the structure data
    void addNeuron(int typeNeuron, QString ip, QString name, QString label, float posX, float posY, double scale,double V,
                   double Iexc, double Iinh, double tauExc, double tauInh, double tauV,
                   double R, double Vr, double Vrh, double Vth, double At, int amountOfNeurons);
    void addNeuron(int typeNeuron, QString ip, QString id, QString label, float posX, float posY, double scale,int amountOfNeurons, Parameters *p);


    bool existIp(QString ip);

    //void updateDataNeuronsNeuron(QString ip, QString id, int N);
    //void updateDataNeuronSypnapse(QString idGlobal, QString ipmSourceNeuron, QString ipmTarget, int type, quint16 port, double w, double fx_numberTxt);

    QString generateIPReal(QString type);
    QString generateIPSimulated(QString type);
    void restaurateIPSimulated(QString type); //Al cancelar de newNeuron o newGenerator

    bool sceneBeSaved=false;
    QString fileNameToSave="";
    QGraphicsScene *scene;
    QString mathematicalModel;
    QList <Node*> vectorGraphicsNodes;
    QVector <JunctionsDraw*> vectorGraphicsJunction;
    QList <Neuron *> localNeurons; // Neurons created locally, to display the monitors
    int generateIdGlobalNeuron();
    int idGlobalNeuron; // It is passed to Neuron and it is passed through messages to newNeuronGraphics and newGenerator

public slots:
    void newGenerator();
    void newSpiNNaker();
    void newSynapse();
    void newNeuron();
    void newScenary();
    void newRemove();
    void saveSceneAs();
    void saveCurrentScene();
    void loadScene();
    void loadSceneFromMsg(QString filename);

    void shuffle();
    void zoomIn();
    void zoomOut();
    void processPendingDatagrams();
    int getNumberItemOf(int typeOfItem);
    void removeALLScenary();


private slots:
    void emptyVectors();
protected:
    void keyPressEvent(QKeyEvent *event) override;

#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *event) override;

#endif
    void drawBackground(QPainter *painter, const QRectF &rect) override;

    void scaleView(qreal scaleFactor);

private:
    int timerId;
    // Counters to build a multicast IP (MDHCP)
    int DHCP_NORMAL_A,DHCP_NORMAL_B,DHCP_NORMAL_C,DHCP_NORMAL_D;
    int DHCP_GENERATOR_A,DHCP_GENERATOR_B,DHCP_GENERATOR_C,DHCP_GENERATOR_D;

    int sim_DHCP_NORMAL_A,sim_DHCP_NORMAL_B,sim_DHCP_NORMAL_C,sim_DHCP_NORMAL_D;
    int sim_DHCP_GENERATOR_A,sim_DHCP_GENERATOR_B,sim_DHCP_GENERATOR_C,sim_DHCP_GENERATOR_D;


    Node* findNode(QString key);
    void saveScene(QString filename);
    void sendMsg(QString message, QString ipmTarget, int port);

    QUdpSocket udpSocket; // From to send a message
    QHostAddress groupAddress; // I'm listening
    int idGlobalSynapse; // It is passed to newsynapseGraphic by reference and there it is modified as many times as synapses are created

    QTimer *timerEmpty;
    void emptyLocalNeurons();
    void emptyVectorGraphicsEdges();
    void emptyVectorGraphicsNodes();
};


#endif // GRAPHWIDGET_H
