#include "neuronscheme.h"
#include "ui_neuronscheme.h"
#include "model_adexlif.h"
#include "encodeDecodeMsg.h"
#include "messages.h"
#include "parameters.h"
#include "node.h"
#include "removeitem.h"
#include "typedata.h"
#include "newneuronGraphic.h"
#include "newgeneratorGraphic.h"
#include "newsynapseGraphic.h"
#include "newspinnaker.h"

#include <iostream>
#include <QGraphicsView>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QDomDocument>

extern typeFxI fxI;

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

    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(0, 0, 961, 570); // Window size

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    ui->graphicsView->scale(qreal(0.8), qreal(0.8));


    // We listen to the PROMISCUOUS_MOTHER_PORT any Neuron
    groupAddress=QHostAddress(IPM_MOTHER);
    udpSocket.bind(QHostAddress::AnyIPv4, GRAPHICS_PORT, QUdpSocket::ShareAddress);
    udpSocket.joinMulticastGroup(groupAddress);
    connect(&udpSocket, SIGNAL(readyRead()),this, SLOT(processPendingDatagrams()));

    // The window is positioned in the center of the screen
    this->move(QApplication::desktop()->availableGeometry().center() - this->rect().center());
}

NeuronScheme::~NeuronScheme()
{
    delete scene;
    delete ui;
}

/*#if QT_CONFIG(wheelevent)
void NeuronScheme::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, -event->angleDelta().y() / 240.0));
}
#endif*/

void NeuronScheme::scaleView(qreal scaleFactor)
{
    qreal factor = ui->graphicsView->transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    ui->graphicsView->scale(scaleFactor, scaleFactor);
}

void NeuronScheme::emptyVectors()
{
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

            if (found)
            {
                scene->removeItem(vectorGraphicsNodes.at(idx));// Is graphically removed
                if ((QDialog*) vectorGraphicsNodes.at(idx)->FormDialog)
                    if (((QDialog*) vectorGraphicsNodes.at(idx)->FormDialog)->isEnabled())
                        ((QDialog*) vectorGraphicsNodes.at(idx)->FormDialog)->close();
                vectorGraphicsNodes.removeAt(idx); // Is remved from the vector
            }       
        }
        else if (msg.operation==CREATE_SYNAPSE_INTO_GRAPHIC)
        {
            QString source=msg.field1;
            QString target=msg.field2;
            int port = msg.field3.toInt();
            int typeSynapse=msg.field4.toInt();
            double w=msg.field5.toDouble();
            QString fx_numberTxt=msg.field6;
            QString fx_unitMeasureTxt =msg.field7;
            int typeNeuronTarget=msg.field8.toInt();
            QString idGlobalSynapse=msg.field9;

            JunctionsDraw *e=new JunctionsDraw(findNode(source),findNode(target),typeNeuronTarget,w,fx_unitMeasureTxt,typeSynapse,idGlobalSynapse);
            scene->addItem(e);
            vectorGraphicsJunction.push_back(e);
            sceneBeSaved=true;
            Node::TypeDataSynapsys synap;
            synap.fx_numberTxt=fx_numberTxt;
            synap.fx_unitMeasureTxt=fx_unitMeasureTxt;
            synap.w=w;
            synap.idGlobal=idGlobalSynapse;
            synap.ipmTarget=target;
            synap.ipmSource=source;
            synap.port=(quint16) port;
            synap.type=typeSynapse;

            Node *node1=findNode(target);
            node1->synapsys.push_back(synap);
            if (node1->FormDialog!=nullptr)
                ((ModelAdExLIF *) node1->FormDialog)->showSynapsys(); // TODO: Change

            // It is NOT necessary to add synapses, because only Neurons are of interest
        }
        else if (msg.operation==REMOVE_SYNAPSE_FROM_MOTHER_TO_GRAPHICS)
        {
            // Synapse is removed from three sites:
            // 1- localNeuron
            // 2- VectorGraphicsNodes
            // 3- VectorGraphicsJunction
            //-----------------------
            int idGlobalSynapse_tmp=msg.field2.toInt();
            QString ipSource_tmp=msg.field1;
            QString ipTarget_tmp=msg.field3;

            // 1- localNeuron
            bool found=false;
            int idx_n=0;
            int idx_s=0;
            while ((!found) && (idx_n<localNeurons.size())){
                idx_s=0;
                while ((!found) && (idx_s<localNeurons.at(idx_n)->Vsynapse.size())) {
                  if (localNeurons.at(idx_n)->Vsynapse.at(idx_s)->idGlobalSynapse==idGlobalSynapse_tmp)
                     found=true;
                  else
                      idx_s++;
               }
               if (!found)
                  idx_n++;
            }
            if (found) {
                if (localNeurons.at(idx_n)->Vsynapse.at(idx_s)) // The synapse may have been removed from Mothers
                   localNeurons.at(idx_n)->Vsynapse.at(idx_s)->deleteSynapse();
                localNeurons.at(idx_n)->Vsynapse.remove(idx_s);
            }

            // 2- VectorGraphicsNodes
            found=false;
            idx_n=0;
            while ((!found) && (idx_n<vectorGraphicsNodes.size())) {
               idx_s=0;
               while ((!found) && (idx_s<vectorGraphicsNodes.at(idx_n)->synapsys.size())) {
                  if (vectorGraphicsNodes.at(idx_n)->synapsys.at(idx_s).idGlobal==QString::number(idGlobalSynapse_tmp))
                      found=true;
                  else
                      idx_s++;
                }
               if (!found)
                   idx_n++;
            }
            if (found)
                vectorGraphicsNodes.at(idx_n)->synapsys.remove(idx_s);

            // 3- VectorGraphicsJunction
            found=false;
            int idx=0;
            while ((!found) && (idx<vectorGraphicsJunction.size())) {
               if (vectorGraphicsJunction.at(idx)->idGlobalSynapse==QString::number(idGlobalSynapse_tmp))
                  found=true;
               else
                   idx++;
            }
            if (found) {
                scene->removeItem(vectorGraphicsJunction.at(idx));// Is graphically removed
                vectorGraphicsJunction.remove(idx); // Is remved from the vector
            }
        }
        else if (msg.operation==LOADSCENE_FROM_MOTHER_TO_GRAPHIC)
        {
            loadSceneFromMsg(msg.field1);
        }
    }
}

void NeuronScheme::loadSceneFromMsg(QString filename)
{
    QXmlStreamReader xml;

    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Neuronal scheme"),
                             tr("File %1:\n%2 could not be read.")
                             .arg(QDir::toNativeSeparators(fileNameToSave),
                             file.errorString()));
        return;
    }

    xml.setDevice(&file);

    // Delete all graphic items
    QList<QGraphicsItem*> allGraphicsItems = scene->items();
    for(int i = 0; i < allGraphicsItems.size(); i++)
    {
        QGraphicsItem *graphicItem = allGraphicsItems[i];
        if (!graphicItem->toGraphicsObject())
        {
            scene->removeItem(graphicItem);
            delete graphicItem;
            scene->update();
        }
    }

    QDomDocument XML;
    XML.setContent(&file);
    file.close();
    QDomElement root=XML.documentElement();
    std::cout<<"ROOT: "<<root.tagName().toStdString()<<std::endl;
    QString version=root.attribute("version","No assigned");
    std::cout<<"VERSION: "<<version.toStdString()<<std::endl;

    QDomElement Component=root.firstChild().toElement();
    while(!Component.isNull())
    {
        // Load a neuron
        if (Component.tagName()=="Node") {
            QDomElement Child=Component.firstChild().toElement();
            QString ip;
            QString id;
            QString label;
            int typeNeuron=TYPENEURON_NORMAL;
            float posX = 0;
            float posY = 0;
            int amountOfNeurons=1;
            double scale;
            int frecuency=1000; //For the Generator case

            //Neuronal Model
            int neuronModel=0;
            double V;
            double Iexc;
            double Iinh;
            double tauExc;
            double tauInh;
            double tauV;
            double R;
            double Vr;
            double Vrh;
            double Vth;
            double At;

            // Read each child of the component node
            while (!Child.isNull())
            {
                // Read Name and value
                if (Child.tagName()=="ip") ip=Child.firstChild().toText().data();
                if (Child.tagName()=="typeNeuron") typeNeuron=Child.firstChild().toText().data().toInt();
                if (Child.tagName()=="id") id=Child.firstChild().toText().data();
                if (Child.tagName()=="label") label=Child.firstChild().toText().data();
                if (Child.tagName()=="posX") posX=Child.firstChild().toText().data().toFloat();
                if (Child.tagName()=="posY") posY=Child.firstChild().toText().data().toFloat();
                if (Child.tagName()=="scale") scale=Child.firstChild().toText().data().toDouble();
                if (Child.tagName()=="frecuency") frecuency=Child.firstChild().toText().data().toInt();
                if (Child.tagName()=="amountOfNeurons") amountOfNeurons=Child.firstChild().toText().data().toInt();

                //Mathematical model parameters
                // TODO: Add new parameters
                // Load mathematical model and parameters
                if (Child.tagName()=="Model") neuronModel=Child.firstChild().toText().data().toInt();
                if (Child.tagName()=="V") V=Child.firstChild().toText().data().toDouble();
                if (Child.tagName()=="Iexc") Iexc=Child.firstChild().toText().data().toDouble();
                if (Child.tagName()=="Iinh") Iinh=Child.firstChild().toText().data().toDouble();
                if (Child.tagName()=="tauExc") tauExc=Child.firstChild().toText().data().toDouble();
                if (Child.tagName()=="tauInh") tauInh=Child.firstChild().toText().data().toDouble();
                if (Child.tagName()=="tauV") tauV=Child.firstChild().toText().data().toDouble();
                if (Child.tagName()=="R") R=Child.firstChild().toText().data().toDouble();
                if (Child.tagName()=="Vr") Vr=Child.firstChild().toText().data().toDouble();
                if (Child.tagName()=="Vrh") Vrh=Child.firstChild().toText().data().toDouble();
                if (Child.tagName()=="Vth") Vth=Child.firstChild().toText().data().toDouble();
                if (Child.tagName()=="At") At=Child.firstChild().toText().data().toDouble();
                // Next child
                Child = Child.nextSibling().toElement();

            }

            QStringList strlist;
            strlist=ip.split(".");
            if (typeNeuron==TYPENEURON_NORMAL)
            {
                DHCP_NORMAL_A= strlist.at(0).toInt();
                DHCP_NORMAL_B= strlist.at(1).toInt();
                DHCP_NORMAL_C= strlist.at(2).toInt();
                DHCP_NORMAL_D= strlist.at(3).toInt();
                restaurateIPSimulated(QString::number(TYPENEURON_NORMAL));
            }
            else if (typeNeuron==TYPENEURON_GENERATOR)
            {
                DHCP_GENERATOR_A= strlist.at(0).toInt();
                DHCP_GENERATOR_B= strlist.at(1).toInt();
                DHCP_GENERATOR_C= strlist.at(2).toInt();
                DHCP_GENERATOR_D= strlist.at(3).toInt();
                restaurateIPSimulated(QString::number(TYPENEURON_GENERATOR));
            }

            idGlobalNeuron= id.toInt();
            // TODO: Add new parameters
            Parameters *p1= new Parameters(neuronModel,V,Iexc,Iinh,tauExc,tauInh,tauV,R,Vr,Vrh,Vth,At);
            if (typeNeuron==TYPENEURON_GENERATOR)
            {
                Neuron *n = new Neuron(nullptr,amountOfNeurons, label, posX, posY, ip, idGlobalNeuron, typeNeuron,LOCAL_NEURON, p1,1,"1E-9");
                SpikeGenerator *spikeGenerator = new SpikeGenerator(nullptr,frecuency,ip);
                n->FormDialog=spikeGenerator;
                localNeurons.append(n);
            }
            else if(neuronModel==0)
            {
                neuron_adexlif *n = new neuron_adexlif(nullptr,amountOfNeurons ,label,posX,posY, ip,idGlobalNeuron, typeNeuron,LOCAL_NEURON, p1,1,"1E-9",V,Iexc,Iinh,Vr);
                localNeurons.append(dynamic_cast<Neuron*>(n));
            }
            else if(neuronModel==1)
            {
                neuron_cubalif *n = new neuron_cubalif(nullptr,amountOfNeurons ,label,posX,posY, ip,idGlobalNeuron, typeNeuron,LOCAL_NEURON, p1,1,"1E-9",V,Iexc,Iinh,Vr);
                localNeurons.append(dynamic_cast<Neuron*>(n));
            }
            else if(neuronModel==2)
            {
                neuron_izhikevich *n = new neuron_izhikevich(nullptr,amountOfNeurons ,label,posX,posY, ip,idGlobalNeuron, typeNeuron,LOCAL_NEURON, p1,1,"1E-9",V,Iexc,Iinh,Vr);
                localNeurons.append(dynamic_cast<Neuron*>(n));
            }
        }
        else if (Component.tagName()=="Synapse")
        {
            QDomElement Child=Component.firstChild().toElement();

            QString source="";
            QString target="";
            int port=0;
            double w=0;
            QString fx_unitMeasureTxt="nA";
            QString fx_numberTxt="1E-9";
            int typeTargetNeuron=TYPENEURON_NONE;
            int typeSynapse=TYPE_SYP_EXCITATION;
            int idGlobalSynapse_tmp=0;
            while (!Child.isNull())
            {
                if (Child.tagName()=="source") source=Child.firstChild().toText().data();
                if (Child.tagName()=="target") target=Child.firstChild().toText().data();
                if (Child.tagName()=="typeTargetNeuron") typeTargetNeuron=Child.firstChild().toText().data().toInt();
                if (Child.tagName()=="w") w=Child.firstChild().toText().data().toDouble();
                if (Child.tagName()=="fx") fx_unitMeasureTxt=Child.firstChild().toText().data();
                if (Child.tagName()=="typeSynapse") typeSynapse=Child.firstChild().toText().data().toInt();
                if (Child.tagName()=="idGlobalSynapse") idGlobalSynapse_tmp=Child.firstChild().toText().data().toInt();
                // Next child
                Child = Child.nextSibling().toElement();
            }
            // The general value of idGlobalSynapse is updated

            idGlobalSynapse=idGlobalSynapse_tmp;
            if (typeTargetNeuron==TYPENEURON_NORMAL)
                port=INTERNEURON_PORT;
            else if (typeTargetNeuron==TYPENEURON_GENERATOR)
                port=GENERATOR_PORT;

            QString msg=QString("#")+sep_operation+CREATE_SYNAPSE_INTO_MOTHER+sep_operation+source+separator;
            fx_numberTxt=fxI.find(fx_unitMeasureTxt).value();
            msg+=target+separator+QString::number(port)+separator+ QString::number(typeSynapse)+separator+QString::number(w)+separator+fx_numberTxt+separator+fx_unitMeasureTxt+separator;
            msg+=QString::number(typeTargetNeuron)+separator+QString::number(idGlobalSynapse)+separator;
            sendMsg(msg,IPM_NEURON_PROMISCUOUS,NEURON_PROMISCUOS_PORT);

        }
        // Next component
        Component = Component.nextSibling().toElement();
    }

    file.close();
}

Node* NeuronScheme::findNode(QString key){
    bool founded=false;
    int idx=0;
    int total=vectorGraphicsNodes.size();
    while ((!founded) && (idx<total)) {
        if (vectorGraphicsNodes.at(idx)->ip==key)
            founded=true;
        else
            idx++;
    }
    if (founded)
        return vectorGraphicsNodes.at(idx);

    return (Node*) nullptr;
}

void NeuronScheme::on_btExport_clicked()
{
    NewSpiNNaker *SpiNNaker = new NewSpiNNaker(nullptr,&vectorGraphicsNodes,&vectorGraphicsJunction);
    SpiNNaker->setWindowModality(Qt::ApplicationModal);
    SpiNNaker->show();
}

void NeuronScheme::on_btExit_clicked()
{
    close();
}

void NeuronScheme::on_btLoad_clicked()
{
    QXmlStreamReader xml;

    QFileDialog fileDialog(this, tr("Load neuron model"), QDir::currentPath(),tr("Neuronal Files (*.xNS *.xml)"));
    fileDialog.setOption(QFileDialog::DontUseNativeDialog,true);// QFileDialog::DontUseNativeDialog,true); //Very important for refresh
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    if (QDialog::Accepted != fileDialog.exec())
        return ;

    fileDialog.window()->setFocus();
    QString tmpFileName=fileDialog.selectedFiles().at(0);
    if (tmpFileName.isEmpty())
       return;

    fileNameToSave=tmpFileName;
    removeALLScenary();

    QString msg=QString("#")+sep_operation+REMOVE_ALL_NEURON_FROM_GRAPHICS_TO_MOTHER_LOADSCENE+sep_operation;
    msg+=tmpFileName+separator;
    sendMsg(msg,IPM_NEURON_PROMISCUOUS,NEURON_PROMISCUOS_PORT);

    fileDialog.close();
}

void NeuronScheme::on_btNew_clicked()
{
    QMessageBox::StandardButton reply;
    QString msg="Would you like to save the current scenario?";

    if (sceneBeSaved)
        reply=QMessageBox::question(this, "Warning",msg,QMessageBox::Cancel|QMessageBox::Yes|QMessageBox::No);
    if (reply==QMessageBox::Cancel)
        return;

    if (reply==QMessageBox::Yes) {
        if (fileNameToSave.length())
            saveScene(fileNameToSave);
        else
            saveSceneAs();
        sceneBeSaved=false;
    }
    else if (reply==QMessageBox::No)
        fileNameToSave="";

    sceneBeSaved=false;

    DHCP_NORMAL_A=QString(IPM_BASE_NEURON).mid(0,3).toInt();
    DHCP_GENERATOR_A=QString(IPM_GENERATOR).mid(0,3).toInt();
    DHCP_NORMAL_B=0;
    DHCP_NORMAL_C=0;
    DHCP_NORMAL_D=0;

    DHCP_GENERATOR_B=0;
    DHCP_GENERATOR_C=0;
    DHCP_GENERATOR_D=0;

    restaurateIPSimulated(QString::number(TYPENEURON_NORMAL));
    restaurateIPSimulated(QString::number(TYPENEURON_GENERATOR));

    removeALLScenary();
}

void NeuronScheme::on_btSave_clicked()
{
    if (fileNameToSave.length())
        saveScene(fileNameToSave);
    else
        saveSceneAs();
}

void NeuronScheme::saveSceneAs()
{
    QString fileName;

    if (!fileName.length())
    {
        QFileDialog fileDialog(this, tr("Save neuron model"), QDir::currentPath(),tr("Neuronal Files (*.xNS *.xml)"));
        fileDialog.setOption(QFileDialog::DontUseNativeDialog,true);// Very important for refresh
        fileDialog.setAcceptMode(QFileDialog::AcceptSave);
        if (QDialog::Accepted != fileDialog.exec())
            return ;

        fileDialog.window()->setFocus();
        fileName=fileDialog.selectedFiles().at(0);
    }

    if (fileName.isEmpty())
        return;

    fileNameToSave=fileName;
    saveScene(fileNameToSave);
}

void NeuronScheme::on_btSaveAs_clicked()
{
    saveSceneAs();
}

void NeuronScheme::saveScene(QString fileName) {

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
       QMessageBox::warning(this, tr("Neuronal scheme"),
                                tr("Error writting file %1:\n%2.")
                                .arg(QDir::toNativeSeparators(fileName),
                                 file.errorString()));
       return;
    }

    QXmlStreamWriter xml;
    xml.setAutoFormatting(true);
    xml.setDevice(&file);
    xml.writeStartDocument();
    xml.writeDTD(QStringLiteral("<!DOCTYPE xbel>"));
    xml.writeStartElement(QStringLiteral("xNeuronalScheme"));
    // TODO: DElete mathematical model in general XML
    //xml.writeAttribute( QStringLiteral("MODEL"), mathematicalModel.toStdString().c_str()); //QStringLiteral(QString::strcat(mathematicalModel)));
    xml.writeAttribute( QStringLiteral("version"), QStringLiteral("1.0"));
    xml.writeTextElement("Nodes","NEURONAL NODES");
    for (int i=0; i<vectorGraphicsNodes.size();i++) {
        xml.writeStartElement("Node");
        xml.writeTextElement("ip", vectorGraphicsNodes.at(i)->ip);
        xml.writeTextElement("id", vectorGraphicsNodes.at(i)->id);
        xml.writeTextElement("label", vectorGraphicsNodes.at(i)->label);
        xml.writeTextElement("posX", QString::number(vectorGraphicsNodes.at(i)->pos().x())); // >posGraph_X));
        xml.writeTextElement("posY", QString::number(vectorGraphicsNodes.at(i)->pos().y()));// >posGraph_Y));//posGraph_Y));
        xml.writeTextElement("scale", QString::number(vectorGraphicsNodes.at(i)->scale));
        xml.writeTextElement("typeNeuron", QString::number(vectorGraphicsNodes.at(i)->typeNode));
        xml.writeTextElement("frecuency", QString::number(vectorGraphicsNodes.at(i)->frecuency));
        xml.writeTextElement("amountOfNeurons", QString::number(vectorGraphicsNodes.at(i)->amountOfNeurons));

        // Neuron parameters
        // TODO: Add new parameters
        xml.writeTextElement("Model", QString::number((int)vectorGraphicsNodes.at(i)->parameters->neuronModel));
        xml.writeTextElement("V", QString::number((double)vectorGraphicsNodes.at(i)->parameters->V));
        xml.writeTextElement("Iexc", QString::number((double)vectorGraphicsNodes.at(i)->parameters->Iexc));
        xml.writeTextElement("Iinh", QString::number((double)vectorGraphicsNodes.at(i)->parameters->Iinh));
        xml.writeTextElement("tauExc", QString::number(vectorGraphicsNodes.at(i)->parameters->tau_e));
        xml.writeTextElement("tauInh", QString::number(vectorGraphicsNodes.at(i)->parameters->tau_i));
        xml.writeTextElement("tauV", QString::number(vectorGraphicsNodes.at(i)->parameters->tau_v));
        xml.writeTextElement("R", QString::number(vectorGraphicsNodes.at(i)->parameters->R));
        xml.writeTextElement("Vr", QString::number(vectorGraphicsNodes.at(i)->parameters->Vr));
        xml.writeTextElement("Vrh", QString::number(vectorGraphicsNodes.at(i)->parameters->Vrh));
        xml.writeTextElement("Vth", QString::number(vectorGraphicsNodes.at(i)->parameters->Vth));
        xml.writeTextElement("At", QString::number(vectorGraphicsNodes.at(i)->parameters->At));

        xml.writeEndElement();
    }

    // TODO: Add STDP
    xml.writeTextElement("Synapsys","NEURONAL SYNAPSES");
    for (int i=0; i<vectorGraphicsJunction.size();i++) {
        xml.writeStartElement("Synapse");
        xml.writeTextElement("source", vectorGraphicsJunction.at(i)->sourceNode()->ip);
        xml.writeTextElement("target", vectorGraphicsJunction.at(i)->targetNode()->ip);
        xml.writeTextElement("typeTargetNeuron", QString::number(vectorGraphicsJunction.at(i)->typeTargetNeuron));
        xml.writeTextElement("w", QString::number(vectorGraphicsJunction.at(i)->w));
        xml.writeTextElement("fx", vectorGraphicsJunction.at(i)->fx);
        xml.writeTextElement("typeSynapse", QString::number(vectorGraphicsJunction.at(i)->typeSynapse));
        xml.writeTextElement("idGlobalSynapse", vectorGraphicsJunction.at(i)->idGlobalSynapse);
        xml.writeEndElement();
    }
    xml.writeEndDocument();
    file.close();
    sceneBeSaved=false;

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

void NeuronScheme::on_btNewGenerator_clicked()
{
    float posX= QRandomGenerator::global()->bounded(800)+250;
    float posY= QRandomGenerator::global()->bounded(450)+100;

    NewGeneratorGraphics *newGenerator = new NewGeneratorGraphics(nullptr,this,
                                                                  posX,posY,
                                                                  &idGlobalNeuron,
                                                                  &vectorGraphicsNodes,
                                                                  &localNeurons,
                                                                  &sceneBeSaved);
    newGenerator->setWindowModality(Qt::ApplicationModal);
    newGenerator->show();
}


void NeuronScheme::on_btNewSynapse_clicked()
{
    if (vectorGraphicsNodes.size()) {
        NewSynapseGraphic *newSynapse= new NewSynapseGraphic(nullptr,&idGlobalSynapse, scene, &vectorGraphicsNodes, &vectorGraphicsJunction, &sceneBeSaved);
        newSynapse->setWindowModality(Qt::ApplicationModal);
        newSynapse->show();
    }
    else
        QMessageBox::information(this, "Warning","No neuron has been created yet.");
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

