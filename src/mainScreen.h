#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include "WidgetGeneralMonitor.h"
#include "spikeGenerator.h"
#include "neuron.h"
#include "motherNeuron.h"
#include <QVector>
#include "typedata.h"
#include "scheme/neuronscheme.h"
#define VERSION 4.3

namespace Ui {
class MainWindow;
}

class MainScreen : public QMainWindow
{    Q_OBJECT

public:
    explicit MainScreen(QWidget *parent = nullptr);

    int generalClock;
    ~MainScreen();

    NeuronScheme *schemeNeuron;
    MotherNeuron *motherNeuron;//= MotherNeuron(nullptr,IPM_MOTHER,schemeNeuron);
    QGraphicsScene *scene;
    SpikeGenerator *spikeGenerator;

    QUdpSocket udpSocketX;
    QHostAddress groupX;
    QHostAddress  groupAddressJ;
    QUdpSocket    udpSocketJ;



private slots:

    void on_bt_neural_scheme_clicked();

    void on_bt_general_monitor_clicked();

    void on_bt_about_clicked();

    void on_bt_exit_clicked();

 private:
    Ui::MainWindow *ui;

 protected:
     void showEvent(QShowEvent *) override;
     void closeEvent(QCloseEvent *event) override;
     void keyPressEvent(QKeyEvent *event) override;

};

#endif // MAINWINDOW_H
