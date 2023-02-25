#include "neuronscheme.h"
#include "ui_neuronscheme.h"
#include "model_adexlif.h"
#include "encodeDecodeMsg.h"
#include "messages.h"
#include "parameters.h"
#include "newneuronGraphic.h"
#include "node.h"
#include "removeitem.h"

#include <QMessageBox>
#include <QDesktopWidget>

NeuronScheme::NeuronScheme(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NeuronScheme)
{
    ui->setupUi(this);

    fileNameToSave="";
    scene = new QGraphicsScene(ui->graphicsView);

    DHCP_NORMAL_A=sim_DHCP_NORMAL_A=QString(IPM_BASE_NEURON).mid(0,3).toInt();

    DHCP_GENERATOR_A=sim_DHCP_GENERATOR_A=QString(IPM_GENERATOR).mid(0,3).toInt();

    DHCP_NORMAL_B=DHCP_NORMAL_C=DHCP_NORMAL_D=0;
    sim_DHCP_NORMAL_B=sim_DHCP_NORMAL_C=sim_DHCP_NORMAL_D=0;

    DHCP_GENERATOR_B=DHCP_GENERATOR_C=DHCP_GENERATOR_D=0;
    sim_DHCP_GENERATOR_B=sim_DHCP_GENERATOR_C=sim_DHCP_GENERATOR_D=0;

    idGlobalSynapse=0;
    idGlobalNeuron=0;

    // We listen to the PROMISCUOUS_MOTHER_PORT any Neuron
    groupAddress=QHostAddress(IPM_MOTHER);
    udpSocket.bind(QHostAddress::AnyIPv4, GRAPHICS_PORT, QUdpSocket::ShareAddress);
    udpSocket.joinMulticastGroup(groupAddress);
    connect(&udpSocket, SIGNAL(readyRead()),this, SLOT(processPendingDatagrams()));

    // The window is positioned in the center of the screen
    this->move(QApplication::desktop()->availableGeometry().center() - this->rect().center());

    // TODO: Remove?
    /*timerEmpty = new QTimer(this);
    timerEmpty->setInterval(1000);
    timerEmpty->stop();
    connect(timerEmpty, SIGNAL(timeout()), this, SLOT(emptyVectors()));*/
}

NeuronScheme::~NeuronScheme()
{
    delete scene;
    delete ui;
}

void NeuronScheme::emptyVectors()
{
    //timerEmpty->stop();
    emptyVectorGraphicsEdges();
    emptyVectorGraphicsNodes();
}

void NeuronScheme::emptyVectorGraphicsEdges()
{
    while (!vectorGraphicsJunction.empty())
    {
        vectorGraphicsJunction.erase(vectorGraphicsJunction.begin());
    }
    vectorGraphicsJunction.clear();
}

void NeuronScheme::emptyVectorGraphicsNodes() {
    while (!vectorGraphicsNodes.empty())
    {
        vectorGraphicsNodes.erase(vectorGraphicsNodes.begin());
    }
    vectorGraphicsNodes.clear();

}

void NeuronScheme::emptyLocalNeurons()
{
    while (!localNeurons.empty()) {
        // Is removed removes the pointers to the FormDialogs of each neuron
        if (localNeurons.at(0)->localRemote==REMOTE_NEURON)
        {
            if (localNeurons.at(0)->typeNeuron==TYPENEURON_NORMAL)
            {
                delete ((ModelAdExLIF*) localNeurons[0]->FormDialog);
            }
            else if (localNeurons.at(0)->typeNeuron==TYPENEURON_GENERATOR)
            {
                // This case is not contemplated. We put it in case in the future you want to have a REMOTE generator
            }
        }
        else
        { //LOCAL_NEURON
            if  ((localNeurons.at(0)->typeNeuron==TYPENEURON_NORMAL) ||
                (localNeurons.at(0)->typeNeuron==TYPENEURON_GENERATOR))
            {
                    // No need to remove synapses from localNeuron as they are not created
            }
        }

        localNeurons.erase(localNeurons.begin());
    }
    localNeurons.clear();
}

void NeuronScheme::removeALLScenary()
{
    QString idGlobalSynapse="";
    QString ipSource="";
    QString ipTarget="";
    EncodeDecodeMsg msgx;

    for (int n=0; n<vectorGraphicsNodes.size();n++ )
    {
       Node *node = vectorGraphicsNodes.at(n);

        for (int s=0; s<node->synapsys.size(); s++)
        {
           idGlobalSynapse= node->synapsys.at(s).idGlobal;
           ipSource=node->ipmSource;
           ipTarget= node->synapsys.at(s).ipmTarget;
           QString m=msgx.encondeMsg(REMOVE_SYNAPSE_FROM_GRAPHIC_TO_MOTHER,idGlobalSynapse,ipSource,ipTarget).toStdString().c_str();
           sendMsg(m,IPM_NEURON_PROMISCUOUS, NEURON_PROMISCUOS_PORT);
        }
    }
    // It must be checked if there are any synapses that are connected to the neuron, to be deleted as well
    EncodeDecodeMsg msg1;
    for (int n=0; n<vectorGraphicsNodes.size();n++ )
    {
       Node *node = vectorGraphicsNodes.at(n);
       int s=0;
       ipSource=node->ipmSource;
        while (s<node->synapsys.size())
        {
           if (node->synapsys.at(s).ipmTarget==node->ipmSource)
           {
              idGlobalSynapse= node->synapsys.at(s).idGlobal;
              ipTarget= node->synapsys.at(s).ipmTarget;
               QString m= msg1.encondeMsg(REMOVE_SYNAPSE_FROM_GRAPHIC_TO_MOTHER,idGlobalSynapse,ipSource,ipTarget).toStdString().c_str();
               sendMsg(m ,IPM_NEURON_PROMISCUOUS,NEURON_PROMISCUOS_PORT);
            }
            s++;
        }
        //TODO: FutureWork All Neuron deletion messages should be by idLGobalNeuron and not by IP.
        QString m= msg1.encondeMsg(REMOVE_NEURON_FROM_GRAPHIC_TO_MOTHER,node->ipmSource,node->id).toStdString().c_str();
        sendMsg(m ,IPM_NEURON_PROMISCUOUS,NEURON_PROMISCUOS_PORT);
      }
}

void NeuronScheme::sendMsg(QString message, QString ipmTarget, int port) {

    QUdpSocket udpSocket;
    QHostAddress groupAddress;
    groupAddress=QHostAddress(ipmTarget);
    udpSocket.bind(QHostAddress(ipmTarget), port, QUdpSocket::ShareAddress);
    udpSocket.setSocketOption(QAbstractSocket::MulticastTtlOption, TTL);

    QByteArray datagram = message.toStdString().c_str();
    udpSocket.writeDatagram(datagram, groupAddress, port);
}

int NeuronScheme::generateIdGlobalNeuron()
{
    return ++idGlobalNeuron;
}

bool NeuronScheme::existIp(QString ip)
{
    bool encontrado=false;
    int idx=0;

    while ((!encontrado) && (idx<vectorGraphicsNodes.size()))
    {
        if (vectorGraphicsNodes.at(idx)->ipmSource==ip)
            encontrado=true;
        else idx++;
    }
    return encontrado;
}

QString NeuronScheme::generateIPReal(QString type)
{
    if (type.toInt()==TYPENEURON_NORMAL) {
        if (DHCP_NORMAL_D<=253)
            DHCP_NORMAL_D++;
        else if (DHCP_NORMAL_C<=253) {
            DHCP_NORMAL_D=1;
            DHCP_NORMAL_C++;
        }
        else if (DHCP_NORMAL_B<=253) {
            DHCP_NORMAL_C=1;
            DHCP_NORMAL_D=1;
            DHCP_NORMAL_B++;
        }
        else if (DHCP_NORMAL_A<=239) {
            DHCP_NORMAL_B=1;
            DHCP_NORMAL_C=1;
            DHCP_NORMAL_D=1;
            DHCP_NORMAL_A++;
        }
        else
            QMessageBox::warning(this, tr("Assigning ip"),tr("There is no ip available for the neuron in the address space."));

        sim_DHCP_NORMAL_A=DHCP_NORMAL_A;
        sim_DHCP_NORMAL_B=DHCP_NORMAL_B;
        sim_DHCP_NORMAL_C=DHCP_NORMAL_C;
        sim_DHCP_NORMAL_D=DHCP_NORMAL_D;

        return QString::number(DHCP_NORMAL_A)+QString(".")+QString::number(DHCP_NORMAL_B)+QString(".")+QString::number(DHCP_NORMAL_C)+QString(".")+QString::number(DHCP_NORMAL_D);
     }
    else if (type.toInt()==TYPENEURON_GENERATOR) {
       if (DHCP_GENERATOR_D<=253)
           DHCP_GENERATOR_D++;
       else if (DHCP_GENERATOR_C<=253) {
           DHCP_GENERATOR_D=1;
           DHCP_GENERATOR_C++;
       }
       else if (DHCP_GENERATOR_B<=253) {
           DHCP_GENERATOR_C=1;
           DHCP_GENERATOR_D=1;
           DHCP_GENERATOR_B++;
        }
        else if (DHCP_GENERATOR_A<=239) {
           DHCP_GENERATOR_B=1;
           DHCP_GENERATOR_C=1;
           DHCP_GENERATOR_D=1;
           DHCP_GENERATOR_A++;
        }
        else
           QMessageBox::warning(this, tr("Assigning ip"),tr("There is no ip available for the generator in the address space."));

        sim_DHCP_GENERATOR_A=DHCP_GENERATOR_A;
        sim_DHCP_GENERATOR_B=DHCP_GENERATOR_B;
        sim_DHCP_GENERATOR_C=DHCP_GENERATOR_C;
        sim_DHCP_GENERATOR_D=DHCP_GENERATOR_D;

       return QString::number(DHCP_GENERATOR_A)+QString(".")+QString::number(DHCP_GENERATOR_B)+QString(".")+QString::number(DHCP_GENERATOR_C)+QString(".")+QString::number(DHCP_GENERATOR_D);
    }
}

QString NeuronScheme::generateIPSimulated(QString type)
{
    if (type.toInt()==TYPENEURON_NORMAL) {
        if (sim_DHCP_NORMAL_D<=253)
            sim_DHCP_NORMAL_D++;
        else if (sim_DHCP_NORMAL_C<=253) {
            sim_DHCP_NORMAL_D=1;
            sim_DHCP_NORMAL_C++;
        }
        else if (sim_DHCP_NORMAL_B<=253) {
            sim_DHCP_NORMAL_C=1;
            sim_DHCP_NORMAL_D=1;
            sim_DHCP_NORMAL_B++;
        }
        else if (sim_DHCP_NORMAL_A<=239) {
            sim_DHCP_NORMAL_B=1;
            sim_DHCP_NORMAL_C=1;
            sim_DHCP_NORMAL_D=1;
            sim_DHCP_NORMAL_A++;
        }
        else
            QMessageBox::warning(this, tr("Assigning ip"),tr("There is no ip available for the neuron in the address space."));

        return QString::number(sim_DHCP_NORMAL_A)+QString(".")+QString::number(sim_DHCP_NORMAL_B)+QString(".")+QString::number(sim_DHCP_NORMAL_C)+QString(".")+QString::number(sim_DHCP_NORMAL_D);
     }
    else if (type.toInt()==TYPENEURON_GENERATOR) {
       if (sim_DHCP_GENERATOR_D<=253)
           sim_DHCP_GENERATOR_D++;
       else if (sim_DHCP_GENERATOR_C<=253) {
           sim_DHCP_GENERATOR_D=1;
           sim_DHCP_GENERATOR_C++;
       }
       else if (sim_DHCP_GENERATOR_B<=253) {
           sim_DHCP_GENERATOR_C=1;
           sim_DHCP_GENERATOR_D=1;
           sim_DHCP_GENERATOR_B++;
        }
        else if (sim_DHCP_GENERATOR_A<=239) {
           sim_DHCP_GENERATOR_B=1;
           sim_DHCP_GENERATOR_C=1;
           sim_DHCP_GENERATOR_D=1;
           sim_DHCP_GENERATOR_A++;
        }
        else
           QMessageBox::warning(this, tr("Assigning ip"),tr("There is no ip available for the generator in the address space."));

        return QString::number(sim_DHCP_GENERATOR_A)+QString(".")+QString::number(sim_DHCP_GENERATOR_B)+QString(".")+QString::number(sim_DHCP_GENERATOR_C)+QString(".")+QString::number(sim_DHCP_GENERATOR_D);
    }
}

void NeuronScheme::restaurateIPSimulated(QString type)
{
    if (type.toInt()==TYPENEURON_NORMAL)
    {
        sim_DHCP_NORMAL_A=DHCP_NORMAL_A;
        sim_DHCP_NORMAL_B=DHCP_NORMAL_B;
        sim_DHCP_NORMAL_C=DHCP_NORMAL_C;
        sim_DHCP_NORMAL_D=DHCP_NORMAL_D;
    }
    else if (type.toInt()==TYPENEURON_GENERATOR)
    {
        sim_DHCP_GENERATOR_A=DHCP_GENERATOR_A;
        sim_DHCP_GENERATOR_B=DHCP_GENERATOR_B;
        sim_DHCP_GENERATOR_C=DHCP_GENERATOR_C;
        sim_DHCP_GENERATOR_D=DHCP_GENERATOR_D;
    }
}

int NeuronScheme::getNumberItemOf(int typeOfItem)
{
   int total=0;

   for (int idx=0; idx<vectorGraphicsNodes.size();idx++)
      if (vectorGraphicsNodes.at(idx)->typeNode==typeOfItem)
          total++;

   return total+1;

}

void NeuronScheme::on_btExport_clicked()
{

}


void NeuronScheme::on_btExit_clicked()
{
    close();
}


void NeuronScheme::on_btLoad_clicked()
{

}


void NeuronScheme::on_btNew_clicked()
{

}


void NeuronScheme::on_btSave_clicked()
{

}


void NeuronScheme::on_btSaveAs_clicked()
{

}


void NeuronScheme::on_btNewNeuron_clicked()
{
    Parameters *p = new Parameters();
    p->V=0;
    p->Iexc=0;
    p->Iinh=0;
    p->tau_e=0.2;
    p->tau_i=0.4;
    p->tau_v=0.5;
    p->R=32000000;
    p->Vr=0;
    p->Vrh=0.005;
    p->Vth=0.015;
    p->At=0.0015;


    NewNeuronGraphic *newNeuron = new NewNeuronGraphic(nullptr,this,scene,&idGlobalNeuron, &localNeurons, &vectorGraphicsNodes, &sceneBeSaved);
    newNeuron->setWindowModality(Qt::ApplicationModal);
    newNeuron->show();
}

void NeuronScheme::processPendingDatagrams()
{
    QByteArray datagram;
    QString data="";

    while (udpSocket.hasPendingDatagrams()) {
        datagram.resize(int(udpSocket.pendingDatagramSize()));
        udpSocket.readDatagram(datagram.data(), datagram.size());
        data=datagram.constData();
        EncodeDecodeMsg msg;
        msg.decodeMsg(QString(data));
        if (msg.operation==CREATE_GRAPHIC_NEURON_FROM_MOTHER_TO_GRAPHICS){
            Node *node = new Node(ui->graphicsView,msg.field4.toFloat(),msg.field5.toFloat(), &localNeurons);
            node->ip=msg.field1;
            node->ipmSource=node->ip;
            node->id=msg.field2;
            node->label=msg.field3;
            node->posX=msg.field4.toFloat();
            node->posY=msg.field5.toFloat();
            node->typeNode=msg.field6.toInt();

            if (node->typeNode==TYPENEURON_MOTHER)
                node->scale= SIZE0_NODE;
            else if (node->typeNode==TYPENEURON_GENERATOR)
                node->scale= SIZE1_NODE;
            else
                node->scale= SIZE2_NODE;

            node->setPos((qreal) node->posGraph_X, (qreal) node->posGraph_Y);
            node->setScale(node->scale);

            node->amountOfNeurons=msg.field7.toInt();
            node->localRemote=msg.field8.toInt();
            node->w=msg.field9.toDouble();
            node->fx_numberTxt=msg.field10;

            // Parameters of Neuronal Model
            Parameters *p = new Parameters();
            p->neuronModel=msg.field14.toInt();
            if (msg.field14.toInt() == 1){   //"CUBALIF"{
                p->V=msg.field11.toDouble();
                p->Iexc=msg.field12.toDouble();
                p->Iinh=msg.field13.toDouble();
                p->tau_e=msg.field15.toDouble();
                p->tau_i=msg.field16.toDouble();
                p->tau_v=msg.field17.toDouble();
                p->R=msg.field18.toDouble();
                p->Vr=msg.field19.toDouble();
                p->Vrh=msg.field20.toDouble();
                p->Vth=msg.field21.toDouble();
                p->At=msg.field22.toDouble();
                node->parameters=p;
            }else if (msg.field14.toInt() == 2){   //"Izhikevich"{
                p->V=msg.field11.toDouble();
                p->Iexc=msg.field12.toDouble();
                p->Iinh=msg.field13.toDouble();
                p->tau_e=msg.field15.toDouble();
                p->tau_i=msg.field16.toDouble();
                p->tau_v=msg.field17.toDouble();
                p->R=msg.field18.toDouble();
                p->Vr=msg.field19.toDouble();
                p->Vrh=msg.field20.toDouble();
                p->Vth=msg.field21.toDouble();
                p->At=msg.field22.toDouble();
                node->parameters=p;
            }else if (msg.field14.toInt() == 0){    //"Adexlif"{
                p->V=msg.field11.toDouble();
                p->Iexc=msg.field12.toDouble();
                p->Iinh=msg.field13.toDouble();
                p->tau_e=msg.field15.toDouble();
                p->tau_i=msg.field16.toDouble();
                p->tau_v=msg.field17.toDouble();
                p->R=msg.field18.toDouble();
                p->Vr=msg.field19.toDouble();
                p->Vrh=msg.field20.toDouble();
                p->Vth=msg.field21.toDouble();
                p->At=msg.field22.toDouble();
                node->parameters=p;
            }
            node->FormDialog=nullptr; // The Neuron has been created outside // (QDialog*) this;
            scene->addItem(node);
            vectorGraphicsNodes.push_back(node);
        }
        else if (msg.operation==REMOVE_NEURON_FROM_MOTHER_TO_GRAPHIC)
        {
            // Synapses are already being created
            int idx=0;
            bool found=false;
            QString ipmSource=msg.field1;
            QString id=msg.field2;
            while ((!found) && (idx<localNeurons.size())) {
                if (localNeurons.at(idx)->id==id)
                    found=true;
                else
                    idx++;
            }
            if (found)
                localNeurons.removeAt(idx);

            // The neuron must be elimiated from the graphic part. Whether local or remote
            found=false;
            idx=0;
            while ((!found) && (idx<vectorGraphicsNodes.size())) {
                 if (vectorGraphicsNodes.at(idx)->id==id)
                  found=true;
               else
                  idx++;
            }

            if (found) {
                scene->removeItem(vectorGraphicsNodes.at(idx));// Is graphically removed
                if ((QDialog*) vectorGraphicsNodes.at(idx)->FormDialog)
                    if (((QDialog*) vectorGraphicsNodes.at(idx)->FormDialog)->isEnabled())
                        ((QDialog*) vectorGraphicsNodes.at(idx)->FormDialog)->close();
                vectorGraphicsNodes.removeAt(idx); // Is remved from the vector
            }
        }
    }
}

void NeuronScheme::on_btNewGenerator_clicked()
{

}


void NeuronScheme::on_btNewSynapse_clicked()
{

}


void NeuronScheme::on_btRemoveNeuron_clicked()
{
    if (vectorGraphicsNodes.size()==0)
        QMessageBox::information(this, "Warning","No neuron has been created yet.");
    else {
       Removeitem *removeitem= new Removeitem(nullptr, &vectorGraphicsNodes);
       removeitem->setWindowModality(Qt::ApplicationModal);
       removeitem->show();
    }
}

