#ifndef WIDGET_GMONITOR_H
#define WIDGET_GMONITOR_H

#include <QTimer>
#include <QWidget>
#include <QHostAddress>
#include <QUdpSocket>
#include "TickerGraph.h"
#include "neuron.h"
#include <QVector>
#include "motherNeuron.h"
namespace Ui {
    class Form;
}

class WidgetGeneralMonitor : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetGeneralMonitor(QWidget *parent = nullptr, typeDataNeurons *Qneurons=nullptr);
    ~WidgetGeneralMonitor() override;

     double Iexc;
     double Iinh;
     double V1;
     double V2;
     QString ipmSource;
     int *muestra;
     bool *dataIsAvailable;
     typeDataNeurons *Qneurons;
     QUdpSocket *udpSocket4_GeneralMonitor;
     QHostAddress *groupAddress4_GeneralMonitor;

     QVector <QString> paramsMonitor = {"Spike","Iexc","Iinh","V"};

protected:
     bool event(QEvent *event) override; //To process onShow event window
     void keyPressEvent(QKeyEvent *event) override;

public slots:
     void paintGraphic();

private slots:

     void processDataGeneralMonitor();
     void onShow(); //Function to be called from event function
     void on_pushButton_4_clicked();
     void on_pushButton_3_clicked();
     void on_pushButton_21_clicked();
     void on_pushButton_22_clicked();
     void on_spinSizeX_valueChanged(int arg1);
     void on_spinSizeY_valueChanged(int arg1);
     void on_pushButton_Exit_clicked();
     void on_pushButton_autoRange_clicked();
     void on_pushButton_Start_clicked();
     void on_pushButton_Capture_clicked();

private:
    int fr; //frecuencia de muestreo
    Ui:: Form *ui;
    TickerGraph * graph;
    QTimer timer;
    bool isDrawing;
    int rightMargen;
    int bottomMargen;
    int windowWidth;
    int windowHeight;
    int posCanvasX;
    int posCanvasY;

    QMutex mutexMonitor;
    QMutex mutexReceived;

    void initializeColorSeries();
    void showLegend();
    void setupGraphColor();
    QMap <QString,QPair<QColor,QColor>> colorsSerie;  //Contains the colors serie of the neurons

    typedef QPair <double *, QString> typeSerie;

    QMap <QString,QPair <QString,int>> signalsSelected; // Signals selected to Monitor
    QMap <QString, double *> signalsReceived;
    QMap <QString, double *> signalsMonitor; //Signals to Monitor

    //To data cache
    QVector <double*> values;
    QMap <QString, double* >::Iterator itData;
};

#endif // WIDGET_GMONITOR_H
