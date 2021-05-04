#include "spikeGenerator.h"
#include <chrono>
#include "config.h"

SpikeGenerator::SpikeGenerator(QWidget *parent, int frecuency_ms, QString ip)
    : QDialog(parent)

{

    setWindowFlags(Qt::Tool);

    QString msg = QString("Ready to send SPIKES");
    statusLabel = new QLabel(msg);

    this->groupAddress4=QHostAddress(ip);
    auto frecuencyLabel = new QLabel(tr("Frecuency(ms):"));
    auto spinFrecuency = new QSpinBox;

    spinFrecuency->setRange(0, 10000);
    auto ttlLayout = new QHBoxLayout;
    ttlLayout->addWidget(frecuencyLabel);
    ttlLayout->addWidget(spinFrecuency);

    startButton = new QPushButton(tr("&Start"));
    startButton->setGeometry(0,0,100,50);
    QPixmap pixmap_Start("scheme/graphics/play.png");
    QIcon ButtonIconStart(pixmap_Start);
    startButton->setIcon(ButtonIconStart);
    QSize s1(startButton->rect().size().width()-10,startButton->rect().size().height()-10);
    startButton->setIconSize(s1);
    startButton->setFixedSize(startButton->rect().size());
    startButton->setFlat(true);


    stopButton = new QPushButton(tr("S&top"));
    stopButton->setGeometry(0,0,100,50);
    QPixmap pixmap_Stop("scheme/graphics/stop.png");
    QIcon ButtonIconStop(pixmap_Stop);
    stopButton->setIcon(ButtonIconStop);
    QSize s2(stopButton->rect().size().width()-10,stopButton->rect().size().height()-10);
    stopButton->setIconSize(s1);
    stopButton->setFixedSize(stopButton->rect().size());
    stopButton->setFlat(true);

    auto quitButton = new QPushButton(tr("&Quit"));

    quitButton->setGeometry(0,0,100,50);
    QPixmap pixmap_Quit("scheme/graphics/exit.png");
    QIcon ButtonIconQuit(pixmap_Quit);
    quitButton->setIcon(ButtonIconQuit);
    QSize s3(quitButton->rect().size().width()-10,quitButton->rect().size().height()-10);
    quitButton->setIconSize(s3);
    quitButton->setFixedSize(quitButton->rect().size());
    quitButton->setFlat(true);


    QGraphicsDropShadowEffect* effect0 = new QGraphicsDropShadowEffect();
    QGraphicsDropShadowEffect* effect1 = new QGraphicsDropShadowEffect();
    QGraphicsDropShadowEffect* effect2 = new QGraphicsDropShadowEffect();

    effect0->setBlurRadius(100);
    effect0->setOffset(3,3);
    effect1->setBlurRadius(100);
    effect1->setOffset(3,3);
    effect2->setBlurRadius(100);
    effect2->setOffset(3,3);

    startButton->setGraphicsEffect(effect0);
    stopButton->setGraphicsEffect(effect1);
    quitButton->setGraphicsEffect(effect2);

    auto buttonBox = new QDialogButtonBox;
    buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(stopButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);


    timer.setTimerType(Qt::PreciseTimer);
    connect(spinFrecuency, QOverload<int>::of(&QSpinBox::valueChanged), this, &SpikeGenerator::frecuencyChanged);

    connect(startButton, &QPushButton::clicked, this, &SpikeGenerator::startSending);
    connect(stopButton, &QPushButton::clicked, this, &SpikeGenerator::stopSending);
    connect(quitButton, &QPushButton::clicked, this, &SpikeGenerator::close);
    connect(&timer, SIGNAL(timeout()), this, SLOT(sendDatagram()));

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel);
    mainLayout->addLayout(ttlLayout);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Generator: ")+ip);
    this->frecuency_ms=1000;
    spinFrecuency->setValue(frecuency_ms);
    stopButton->setEnabled(false);
    isSending=false;
}

void SpikeGenerator::frecuencyChanged(int fr)
{
    frecuency_ms=fr;
    if (this->isVisible()) {
        if (isSending)
            timer.start(frecuency_ms);
    }
}

void SpikeGenerator::startSending()
{
    statusLabel->setText("Sending SPIKES");
    startButton->setEnabled(false);
    stopButton->setEnabled(true);
    isSending=true;
    timer.start(frecuency_ms);
}

void SpikeGenerator::stopSending()
{

    statusLabel->setText("Ready to send SPIKES");
    stopButton->setEnabled(false);
    startButton->setEnabled(true);
    isSending=false;
    timer.stop();
}

void SpikeGenerator::set_timerStop(){
    timer.stop();
    messageSpikeSended=1;
}

void SpikeGenerator::set_PushButton(bool state){
   startButton->setEnabled(state);
}
void SpikeGenerator::set_resetMsg(){
   messageSpikeSended=1;
}

void SpikeGenerator::sendDatagram()
{
    QByteArray datagram="SPIKE";
    udpSocket4.writeDatagram(datagram, groupAddress4, GENERATOR_PORT);
       ++messageSpikeSended;
}

