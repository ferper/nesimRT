#ifndef SPIKE_GENERATOR_H
#define SPIKE_GENERATOR_H

#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>
#include "config.h"
class SpikeGenerator : public QDialog
{
    Q_OBJECT

public:
    explicit SpikeGenerator(QWidget *parent = nullptr, int frecuency_ms=100, QString ip=IPM_GENERATOR);
    void set_timerStop();
    void set_PushButton(bool state);
    void set_resetMsg();


public slots:
    void startSending();

private slots:
    void frecuencyChanged(int fr);
    void stopSending();
    void sendDatagram();


private:
    QLabel *statusLabel = nullptr;
    QPushButton *startButton = nullptr;
    QPushButton *stopButton = nullptr;
    QUdpSocket udpSocket4;
    QTimer timer;
    QHostAddress groupAddress4;
    long int messageSpikeSended = 1;
    int frecuency_ms;
    bool isSending;
};

#endif
