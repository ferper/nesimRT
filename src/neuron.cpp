#include <QtWidgets>
#include <QtNetwork>

#include "neuron.h"
#include "synapse.h"
#include "parameters.h"
#include "Widget.h"

#include <iostream>
#include <limits>
#include <iomanip>
#include <math.h>
#include "config.h"
#include "encodeDecodeMsg.h"
#include "spikeGenerator.h"
#include "scheme/model_adexlif.h"
using namespace std;

Neuron::Neuron(QWidget *parent, int NumberNeuronsGroup, QString label, float posX, float posY, QString ip, int idGlobalNeuron, int typeNeuron, int localRemote, Parameters *p,double w_default, QString fx_numberTxt)
    : QDialog(parent)
{

    // The neuron is operative, has an IP
    this->ipmSource=ip; // It is assigned in the processPendingDatagrams method
    this->id=QString::number(idGlobalNeuron);
    isBuilded=(ipmSource.length()>0);
    this->label=label;
    this->posX=posX;
    this->posY=posY;

    this->MAC=getLocalMAC();
    this->Motherneuron_port=MOTHER_PORT;
    //this->ipmMother=ipmMother; // Mother Neuron multicast ip
    this->monitor=nullptr;
    this->GeneralMonitorPort=MONITOR_PORT;
    this->typeNeuron=typeNeuron;
    if (typeNeuron==int(TYPENEURON_NORMAL))
        this->SourcePort= int(INTERNEURON_PORT);
    else if (typeNeuron==int(TYPENEURON_GENERATOR))
        this->SourcePort= int(GENERATOR_PORT);

    this->nSpikes=0; // Number of the spikes produced by the neuron
    this->Iexc_prior=0;
    this->Iinh_prior=0;
    this->V_prior=0;
    this->p=p;
    this->IexcCurrent=0;
    this->IinhCurrent=0;
    this->VCurrent=0;
    this->muestra=0;
    this->we=w_default;
    this->fx_numberTxt=fx_numberTxt;
    this->Iinh_enabled=false;
    this->Iexc_enabled=false;
    this->V_enabled=false;
    this->H=0.01;
    this->dataIsAvailable=false;
    this->NumberNeuronsGroup=NumberNeuronsGroup;
    this->enableDataGeneralMonitor=false;
    this->FormDialog = (QDialog*) this; // By default, all Neurons are TYPENODE_NORMAL

    if (enableDataGeneralMonitor)
        statusLabel = new QLabel(tr("Neuron ready - General Monitor: ON" ));
    else
        statusLabel = new QLabel(tr("Neuron ready - General Monitor: OFF" ));
    setWindowFlags(Qt::Tool);
    setGeometry(0,0,170,90);

    auto btnLocalMonitor = new QPushButton(tr("  Local\n  Monitor"));
    btnLocalMonitor->setGeometry(0,0,150,50);
    QPixmap pixmap_Local(":graphics/localMonitor.jpg");
    QIcon ButtonIconLocal(pixmap_Local);
    btnLocalMonitor->setIcon(ButtonIconLocal);
    QSize s1(btnLocalMonitor->rect().size().width()-10,btnLocalMonitor->rect().size().height()-10);
    btnLocalMonitor->setIconSize(s1);
    btnLocalMonitor->setFixedSize(btnLocalMonitor->rect().size());
    btnLocalMonitor->setFlat(true);

    auto btnGeneralMonitor = new QPushButton(tr("  General\n  Monitor"));
    btnGeneralMonitor->setGeometry(0,0,150,50);
    QPixmap pixmap_General(":graphics/generalMonitor.jpg");
    QIcon ButtonIconGeneral(pixmap_General);
    btnGeneralMonitor->setIcon(ButtonIconGeneral);
    QSize s2(btnGeneralMonitor->rect().size().width()-10,btnGeneralMonitor->rect().size().height()-10);
    btnGeneralMonitor->setIconSize(s2);
    btnGeneralMonitor->setFixedSize(btnGeneralMonitor->rect().size());
    btnGeneralMonitor->setFlat(true);

    auto btnKillme = new QPushButton(tr("  kill me"));
    btnKillme->setGeometry(0,0,150,50);
    QPixmap pixmap_Killme(":graphics/cancel.png");
    QIcon ButtonIconKillme(pixmap_Killme);
    btnKillme->setIcon(ButtonIconKillme);
    QSize s3(btnKillme->rect().size().width()-10,btnKillme->rect().size().height()-10);
    btnKillme->setIconSize(s2);
    btnKillme->setFixedSize(btnKillme->rect().size());
    btnKillme->setFlat(true);

    auto quitButton = new QPushButton(tr("&Quit"));
    quitButton->setGeometry(0,0,150,50);
    QPixmap pixmap_Quit(":graphics/exit.png");
    QIcon ButtonIconQuit(pixmap_Quit);
    quitButton->setIcon(ButtonIconQuit);
    QSize s4(quitButton->rect().size().width()-10,quitButton->rect().size().height()-10);
    quitButton->setIconSize(s4);
    quitButton->setFixedSize(quitButton->rect().size());
    quitButton->setFlat(true);

    QGraphicsDropShadowEffect* effect0 = new QGraphicsDropShadowEffect();
    QGraphicsDropShadowEffect* effect1 = new QGraphicsDropShadowEffect();
    QGraphicsDropShadowEffect* effect2 = new QGraphicsDropShadowEffect();
    QGraphicsDropShadowEffect* effect3 = new QGraphicsDropShadowEffect();

    effect0->setBlurRadius(100);
    effect0->setOffset(3,3);
    effect1->setBlurRadius(100);
    effect1->setOffset(3,3);
    effect2->setBlurRadius(100);
    effect2->setOffset(3,3);
    effect3->setBlurRadius(100);
    effect3->setOffset(3,3);

    btnLocalMonitor->setGraphicsEffect(effect0);
    btnGeneralMonitor->setGraphicsEffect(effect1);
    quitButton->setGraphicsEffect(effect2);
    btnKillme->setGraphicsEffect(effect3);

    auto buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(btnLocalMonitor);
    buttonLayout->addWidget(btnGeneralMonitor);
    buttonLayout->addWidget(btnKillme);
    buttonLayout->addWidget(quitButton);
    buttonLayout->addStretch(1);


    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Neuron:")+ipmSource);


    groupAddress4_to_MotherNeuron=QHostAddress(IPM_MOTHER);
    udpSocket4_MotherNeuron.bind(QHostAddress::AnyIPv4, MOTHER_PORT, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    udpSocket4_MotherNeuron.joinMulticastGroup(groupAddress4_to_MotherNeuron);

    connect(&udpSocket4_MotherNeuron, SIGNAL(readyRead()),this, SLOT(processPendingDatagrams()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(quit()));
    connect(btnLocalMonitor, SIGNAL(clicked()), this, SLOT(paintLocalMonitor()));
    connect(btnKillme, SIGNAL(clicked()), this, SLOT(killme()));
    connect(btnGeneralMonitor,&QPushButton::clicked,[=]() {
        if (isBuilded) {
            enableDataGeneralMonitor=!enableDataGeneralMonitor;
            if (enableDataGeneralMonitor)
               statusLabel->setText("Neuron ready - General Monitor: ON");
            else
               statusLabel->setText("Neuron ready - General Monitor: OFF");
            EncodeDecodeMsg msg;

            QByteArray datagram = msg.encondeMsg(ENABLE_SEND_GENERAL_MONITOR,ipmSource,QString::number(enableDataGeneralMonitor)).toStdString().c_str();
            //cout<<"salida:::  "<<datagram.toStdString()<<endl;

            QUdpSocket udpSocket;
            QHostAddress groupAddress;
            groupAddress=QHostAddress(IPM_NEURON_PROMISCUOUS);
            udpSocket.setSocketOption(QAbstractSocket::MulticastTtlOption, TTL);
            udpSocket.writeDatagram(datagram, groupAddress, NEURON_PROMISCUOS_PORT);
        }
    });

    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    timer->setInterval(1);
    timer->stop();
    connect(timer, SIGNAL(timeout()), this, SLOT(calculateValues()));

    timer_RequestIP = new QTimer(this);
    if (!isBuilded) {
        // Request an IP by message sending my MAC throught a port
        timer_RequestIP->setTimerType(Qt::PreciseTimer);
        timer_RequestIP->setInterval(2000); // Every two seconds a request of IP it's send
        connect(timer_RequestIP, SIGNAL(timeout()), this, SLOT(sendRequestIP_DHCP()));
        timer_RequestIP->start();
    }
    else {
        this->liveNeuron();
    }
    this->localRemote= localRemote;
    file.setFileName("basura10_ExInNx"+label+".csv");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Warning","It was not possible to create the data extraction file.");
    }
    out= new QTextStream(&file);
    linea=0;
    spkOnOff_exc=false;
    spkOnOff_inh=false;
}

void Neuron::quit() {
   close();
}

void Neuron::keyPressEvent(QKeyEvent *event){
    switch(event->key()) {
        case Qt::Key_Escape: // If ESCAPE key is pressed, the window is closed
          close();
        break;
    }
}

void Neuron::closeEvent(QCloseEvent *event){

    if (localRemote==REMOTE_NEURON) {
        QMessageBox::StandardButton reply;
        QString msg="Are you sure you want to quit?\nAll the synapses will be removed.";
        reply=QMessageBox::question(this, "Warning",msg,QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        if (reply==QMessageBox::Yes) {
            killme(false);
            event->accept();
            udpSocket4_MotherNeuron.close();
            QApplication::closeAllWindows();
            QApplication::exit();
        }
        else
            event->ignore();
    }
    else {
        if (monitor)
            monitor->close();
        event->accept();
    }
}

void Neuron::killme(){
    killme(true);
}

void Neuron::killme(bool showMessageBox) {

    QMessageBox::StandardButton reply=QMessageBox::Yes;
    if (showMessageBox) {
        QString msg="Are you sure you want to remove this neuron?\nAll the synapses associated will be removed.";
        reply=QMessageBox::question(this, "Warning",msg,QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    }
    if ((reply==QMessageBox::Yes) || (!showMessageBox)) {

        EncodeDecodeMsg msg1;
        QString m= msg1.encondeMsg(REMOVE_NEURON_FROM_GRAPHIC_TO_MOTHER, this->ipmSource,this->id).toStdString().c_str();
        sendMsg(m ,NEURON_PROMISCUOS_PORT);

        m= msg1.encondeMsg(REMOVE_ALL_SYNAPSYS_THE_OTHER_TOME_FROM_NEURON_TO_MOTHER, this->ipmSource,this->id).toStdString().c_str();
        sendMsg(m ,NEURON_PROMISCUOS_PORT);

        for (int i=0; i<Vsynapse.size();i++) {
            QString m=msg1.encondeMsg(REMOVE_SYNAPSE_FROM_GRAPHIC_TO_MOTHER, QString::number(Vsynapse.at(i)->idGlobalSynapse),ipmSource,Vsynapse.at(i)->ipmT).toStdString().c_str();
            sendMsg(m ,NEURON_PROMISCUOS_PORT);
        }
        if (localRemote==REMOTE_NEURON)
            QApplication::exit();
        else {
            close();
        }
    }
}

void Neuron::sendRequestIP_DHCP(){

   if (!isBuilded) { // The Neuron hasn't got one correct IP
        QUdpSocket udpSocket;
        QHostAddress groupAddress;
        groupAddress=QHostAddress(IPM_NEURON_PROMISCUOUS );
        udpSocket.setSocketOption(QAbstractSocket::MulticastTtlOption, TTL);
        EncodeDecodeMsg msg;
        QByteArray datagram = msg.encondeMsg( REQ_IP_DHCP, MAC, QString::number(TYPENEURON_NORMAL) ).toStdString().c_str();
        udpSocket.writeDatagram(datagram, groupAddress, NEURON_PROMISCUOS_PORT );
    }
}

QString Neuron::getLocalMAC() {

    return  QString::number(QRandomGenerator::global()->generate64());
}

/**************************************************************
        When the Neuron has got all necesary data for to live
***************************************************************/
void Neuron::liveNeuron(){ // The neuron is completly functional

    this->isBuilded=true;
    timer_RequestIP->stop();

    groupAddress4_to_Public=QHostAddress(IPM_NEURON_PROMISCUOUS);
    // Force binding to their respective families
    udpSocket4_sender.setSocketOption(QAbstractSocket::MulticastTtlOption, TTL);
    udpSocket4_senderMonitor.setSocketOption(QAbstractSocket::MulticastTtlOption, TTL); // To public data calculated to General Monitor

    if (!this->label.length())
       label="N-"+ipmSource;
    QString msg=QString("#")+sep_operation+CREATE_NEURON_FROM_NEURON_TO_MOTHER+sep_operation+id+separator+label+separator;
    msg+=QString::number(posX)+separator+QString::number(posY)+separator;
    msg+=QString::number(NumberNeuronsGroup)+separator+ipmSource+separator+QString::number(typeNeuron)+separator;
    msg+=QString::number(localRemote)+separator+QString::number(we)+separator+fx_numberTxt+separator;
    msg+=QString::number(double(p->V))+separator+QString::number(double(p->Iexc))+separator+QString::number(double (p->Iinh))+separator;

    msg+=QString::number(p->tau_e)+separator+QString::number(p->tau_i)+separator+QString::number(p->tau_v)+separator;
    msg+=QString::number(p->R)+separator+QString::number(p->Vr)+separator+QString::number(p->Vrh)+separator+QString::number(p->Vth)+separator;
    msg+=QString::number(p->At)+separator;//+QString::number(w)+separator+ fx_numberTxt+separator;

    sendMsg(msg,NEURON_PROMISCUOS_PORT);

    QString cadena="Neuron: " + ipmSource;
    setWindowTitle(tr(cadena.toStdString().c_str()));
    if (monitor!=nullptr)
        monitor->setWindowTitle(tr(cadena.toStdString().c_str()));
}

void Neuron::sendMsg(QString msg,quint16 port){

    QUdpSocket udpSocket;
    QHostAddress groupAddress;
    groupAddress=QHostAddress(IPM_NEURON_PROMISCUOUS);
    udpSocket.setSocketOption(QAbstractSocket::MulticastTtlOption, TTL);
    QByteArray datagram = msg.toStdString().c_str();
    udpSocket.writeDatagram(datagram, groupAddress, port);
}

void Neuron::sendDataToGeneralMonitor(bool spiking){

    QString spike="0";
    if (spiking)
        spike="100000000";
    QString cadena=ipmSource+";"+spike+";"+QString::number((double)IexcCurrent*10000000000)+";"+QString::number((double)IinhCurrent*10000000000)+";"+QString::number((double)VCurrent*5000)+";";
    QByteArray datagram = cadena.toStdString().c_str();
    udpSocket4_senderMonitor.writeDatagram(datagram, groupAddress4_to_Public, GeneralMonitorPort);
}
void Neuron::generateSpike(){
    nSpikes++;
    QByteArray datagram = "SPIKE";
    udpSocket4_sender.writeDatagram(datagram, groupAddress4_to_Public, SourcePort);

}

void Neuron::paintLocalMonitor(){
    if (!monitor)
        monitor= new Widget(nullptr,&ipmSource,&label, &dataIsAvailable,&muestra,&IexcCurrent,&IinhCurrent,&VCurrent,p,&Vsynapse,&NumberNeuronsGroup);
    monitor->show();
}


int Neuron::get_NSynapses(){

    //cout<<Vsynapse.size()<<endl;
    //cout<<"Current Iexc value:"<<p->Iexc<<endl;
    return Vsynapse.size();
}
void Neuron::set_At(double value) {
    p->At=value;

}
Neuron::~Neuron(){
    delete out;
    for (int i=0; i<Vsynapse.size();i++)
       delete Vsynapse[i];
    delete monitor;
}

void Neuron::calculateIinh() {
   double tmp_Iinh=0;
   if (Iinh_enabled) {
      tmp_Iinh=p->Iinh;
      Iinh_prior=p->Iinh;
      p->Iinh=Iinh_prior+H*(-Iinh_prior/p->tau_i);
   }
}

void Neuron::calculateIexc(){
   if (Iexc_enabled) {
      Iexc_prior=p->Iexc;
      p->Iexc=Iexc_prior+H*(-Iexc_prior/p->tau_e);
   }
}
void Neuron::calculateV() {
       V_prior=p->V;
       p->V=V_prior+H*((1/p->tau_v)*(-(V_prior-p->Vr)+p->At*exp((V_prior-p->Vrh)/p->At)+p->R*(Iexc_prior-Iinh_prior)));
}

void Neuron::calculateValues(){
    bool spiking=false;
    dataIsAvailable=false;
    mutexNeuron.lock();
    for (int i=0; i<10; i++) {
        calculateV();
        if (p->V>p->Vth) { //SPIKE Generator
            p->V=p->Vr;
            spiking = true;
            generateSpike();
        }
        linea++;

        // Initial values for Iexc, Iinh y V
        IexcCurrent=p->Iexc;
        IinhCurrent=p->Iinh;
        VCurrent=p->V;
        calculateIexc();
        calculateIinh();
    }
    mutexNeuron.unlock();
    dataIsAvailable=true;
    if (enableDataGeneralMonitor)
        sendDataToGeneralMonitor(spiking);
}
double Neuron::get_IexcCurrent(){
    return IexcCurrent;
}
double Neuron::get_IinhCurrent(){
    return IinhCurrent;
}
double Neuron::get_VCurrent(){
   return VCurrent;
}

void Neuron::processPendingDatagrams()
{
    QByteArray datagram;

    QString data="";

    while (udpSocket4_MotherNeuron.hasPendingDatagrams()) {
        datagram.resize(int(udpSocket4_MotherNeuron.pendingDatagramSize()));
        udpSocket4_MotherNeuron.readDatagram(datagram.data(), datagram.size());
        data=datagram.constData();
        EncodeDecodeMsg msg;
        msg.decodeMsg(QString(data));

        if (msg.operation==CREATE_SYNAPSE_INTO_NEURON) {
           if (ipmSource==msg.field2) { // The IP of the recipient matches that of the process
               this->p->Iexc=0;
               this->Iexc_prior=0;
               this->p->Iinh=0;
               this->Iinh_prior=0;

               double w1;
               Synapse *s1;
               w1=1.13/1000000000;
               quint16 port_target=quint16(msg.field3.toInt());
               int type =msg.field4.toInt();
               QString fx_numberTxt=msg.field6;
               QString fx_unitMeasureTxt =msg.field7;
               int idGlobalSynapse=msg.field9.toInt();
               if (type==TYPE_SYP_EXCITATION)
                   s1 = new Synapse(&NumberNeuronsGroup,idGlobalSynapse,msg.field1,msg.field2,port_target,type,&p->Iexc,we,fx_numberTxt,fx_unitMeasureTxt,&Iexc_enabled,&V_enabled,timer,&mutexNeuron,&muestra,out,&spkOnOff_exc);
               else
                   s1 = new Synapse(&NumberNeuronsGroup,idGlobalSynapse,msg.field1,msg.field2,port_target,type,&p->Iinh,wi,fx_numberTxt,fx_unitMeasureTxt,&Iinh_enabled,&V_enabled,timer,&mutexNeuron,&muestra,out,&spkOnOff_inh);
               //cout<<"-I am neuron "<<ipmSource.toStdString()<< " and I have created a synapse with "<<msg.field1.toStdString()<<" type: "<<type<<endl;
               s1->timer->start();

               Vsynapse.push_back(s1); // The memory is freed and the synapse will be erased
           }
           if (monitor!=nullptr)
                monitor->showSynapsys();
        }
        else if (msg.operation==UPDATE_VALUES_SYNAPSE_FROM_MOTHER_TO_NEURON) {
           if (ipmSource==msg.field1) { // The IP of the recipient matches that of the process
               int id=msg.field2.toInt();
               double w=msg.field3.toDouble();
               QString fx_numberTxt=msg.field4;
               QString fx_unitMeasureTxt=msg.field5;
               int typeSynapse=msg.field6.toInt();
               bool encontrado=false;
               int idx=0;
               while ((!encontrado) && (idx<Vsynapse.size())) {
                  if (Vsynapse.at(idx)->idGlobalSynapse == id)
                      encontrado=true;
                  else
                      idx++;
               }

               if (encontrado) {
                   Vsynapse.at(idx)->w=w*fx_numberTxt.toDouble();
                   Vsynapse.at(idx)->fx_numberTxt=fx_numberTxt;
                   Vsynapse.at(idx)->fx_unitMeasureTxt=fx_unitMeasureTxt;
                   Vsynapse.at(idx)->type=typeSynapse;
                   //Vsynapse.at(idx)->Iinc = &p->Iinh;&p->Iexcep
                   if (monitor) // If it is visible from the widget, refresh is done in table
                       monitor->showSynapsys();
               }
           }
        }
        else if (msg.operation==UPDATE_MODEL_VALUES_PARAMETERS_FROM_MOTHER_TO_NEURON) {
           if (ipmSource==msg.field1) { // The IP of the recipient matches that of the process
              p->V=msg.field3.toDouble();
              p->Iexc=msg.field4.toDouble();
              p->Iinh=msg.field5.toDouble();
              p->tau_e=msg.field6.toDouble();
              p->tau_i=msg.field7.toDouble();
              p->tau_v=msg.field8.toDouble();
              p->R=msg.field9.toDouble();
              p->Vr=msg.field10.toDouble();
              p->Vrh=msg.field11.toDouble();
              p->Vth=msg.field12.toDouble();
              p->At=msg.field13.toDouble();
              this->NumberNeuronsGroup= msg.field14.toInt();
              this->label=  msg.field15;
              //y Actualizar los controles graficos.
              if (monitor)
                 monitor->showParameters();
           }
        }
        else if (msg.operation==REMOVE_SYNAPSE_FROM_MOTHER_TO_NEURON) {
            if (ipmSource==msg.field1) { // The IP of the recipient matches that of the process
                bool found=false;
                int idx=0;
                int idGlobal_tmp=msg.field2.toInt();
                QString ipTarget_tmp=msg.field3;
                while ((!found) && (idx<Vsynapse.size())) {
                   if (Vsynapse.at(idx)->idGlobalSynapse==idGlobal_tmp)
                      found=true;
                    else
                       idx++;
                }
                if (found) {
                    if (Vsynapse.size()==1) // If the neuron has only one synapse, when it is removed, the calculating motor will stop working.
                        this->dataIsAvailable=false;
                    Vsynapse.at(idx)->timer->stop();
                    *(Vsynapse.at(idx)->startICalculate)=false;
                    *(Vsynapse.at(idx)->startVCalculate)=false;
                    Vsynapse.at(idx)->udpSocket4.leaveMulticastGroup(QHostAddress(Vsynapse.at(idx)->ipmS));
                    Vsynapse.at(idx)->deleteSynapse();
                    Vsynapse.remove(idx);
                }
            }
       }
       else if (msg.operation==REMOVE_ALL_NEURON_FROM_MOTHER_TO_NEURON_LOADSCENE) { //ALL_NEURON
            //ALL neurons process this message; Local and Remote
            //The message is for ALL NEURON. There isn't ip neuron filter.
            //Remove all synapsys and me (i'am a neuron)

            QVector <Synapse*>::iterator it=this->Vsynapse.begin();
            while (it!=Vsynapse.end()) {
                it= Vsynapse.erase(it);
            }
            this->timer->stop();
            if (monitor)
                monitor->close();
            delete this;
            if (localRemote==REMOTE_NEURON) { // Only when it is a remotely created Neuron
               QApplication::closeAllWindows();
               QApplication::exit();
            }
       }
       else if (msg.operation==REMOVE_ALL_NEURON_FROM_MOTHER_TO_NEURON) { //ALL_NEURON
            //ALL neurons process this message; Local and Remote
            //The message is for ALL NEURON. There isn't ip neuron filter.
            //Remove all synapsys and me (i'am a neuron)

            QVector <Synapse*>::iterator it=this->Vsynapse.begin();
            while (it!=Vsynapse.end()) {
                it= Vsynapse.erase(it);
            }
            this->timer->stop();
            if (monitor)
                monitor->close();
            if (this->localRemote==LOCAL_NEURON)
                close();

            if (localRemote==REMOTE_NEURON) { // Only when it is a remotely created neuron
               QApplication::closeAllWindows();
               QApplication::exit();
            }
            delete this;
        }
        else if (msg.operation==REMOVE_NEURON_FROM_MOTHER_TO_NEURON) {
            if ((ipmSource==msg.field1) && (id==msg.field2)) { // The IP of the recipient matches that of the process
                QVector <Synapse*>::iterator it=this->Vsynapse.begin();
                while (it!=Vsynapse.end()) {
                    it= Vsynapse.erase(it);
                }
                this->timer->stop();
                if (monitor)
                    monitor->close();
                if (this->typeNeuron==TYPENEURON_GENERATOR) {
                    ((SpikeGenerator *)FormDialog)->close();
                    ((SpikeGenerator *)FormDialog)->set_timerStop();
                }
                if (this->localRemote==LOCAL_NEURON)
                    close();
                if (localRemote==REMOTE_NEURON) { // Only when it is a remotely created neuron
                   QApplication::closeAllWindows();
                   QApplication::exit();
                }
                delete this;
             }
        }
        else if (msg.operation==ACK_REQ_IP_DHCP) { //Assign the ip to the Neuron
            if (!isBuilded) {
                if (MAC==msg.field1) { // The IP of the recipient matches that of the process
                    timer_RequestIP->stop();
                    this->isBuilded=true;
                    MAC=QString(MAC)+QString("AX"); //TODO: It is a mark to remove this and put a random mark so 
                                                    // that within a device there can be several Neurons
                    this->ipmSource=msg.field2;
                    this->id=msg.field3;
                    this->label=msg.field4;
                    this->liveNeuron(); // A Neuron is brought to life
                }
            }
        }
    }
}

