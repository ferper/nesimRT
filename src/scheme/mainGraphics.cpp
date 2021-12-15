#include "mainGraphics.h"
#include "junctionsDraw.h"
#include "node.h"
#include <math.h>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QPushButton>
#include <iostream>
#include <QVector>
#include <QDialog>
#include <QMessageBox>
#include "spikeGenerator.h"
#include "publicmsg.h"
#include <QSettings>
#include <QXmlStreamWriter>
#include <QtXml>
#include <QFileDialog>
#include "newneuronGraphic.h"
#include "newsynapseGraphic.h"
#include "newgeneratorGraphic.h"
#include "removeitem.h"
#include <QMetaObject>
#include "model_adexlif.h"
#include <scheme/newspinnaker.h>
extern typeFxI fxI;

MainGraphics::MainGraphics(QWidget *parent )
    : QGraphicsView(parent), timerId(0)

{



    fileNameToSave="";
    scene = new QGraphicsScene(this);

    DHCP_NORMAL_A=sim_DHCP_NORMAL_A=QString(IPM_BASE_NEURON).mid(0,3).toInt();

    DHCP_GENERATOR_A=sim_DHCP_GENERATOR_A=QString(IPM_GENERATOR).mid(0,3).toInt();

    DHCP_NORMAL_B=DHCP_NORMAL_C=DHCP_NORMAL_D=0;
    sim_DHCP_NORMAL_B=sim_DHCP_NORMAL_C=sim_DHCP_NORMAL_D=0;

    DHCP_GENERATOR_B=DHCP_GENERATOR_C=DHCP_GENERATOR_D=0;
    sim_DHCP_GENERATOR_B=sim_DHCP_GENERATOR_C=sim_DHCP_GENERATOR_D=0;

    idGlobalSynapse=0;
    idGlobalNeuron=0;

    QPushButton *btload = new QPushButton();
    btload->setGeometry(QRect(50,0,140,70));
    btload->setText("   &Load");

    QPixmap pixmap_load(":graphics/load.png");
    QIcon ButtonIcon(pixmap_load);
    btload->setIcon(ButtonIcon);

    QSize s0(btload->rect().size().width()-20,btload->rect().size().height()-20);
    btload->setIconSize(s0);
    btload->setFixedSize(btload->rect().size());

    QGraphicsProxyWidget *proxy = scene->addWidget(btload);

    QPushButton *btNewScenary = new QPushButton();
    btNewScenary->setGeometry(QRect(50,70,140,70));
    btNewScenary->setText("    new \nS&cenary");

    QPixmap pixmap_NewScenary(":graphics/newScenary.png");
    QIcon ButtonIcon0(pixmap_NewScenary);
    btNewScenary->setIcon(ButtonIcon0);

    QSize s1(btNewScenary->rect().size().width()-20,btNewScenary->rect().size().height()-20);
    btNewScenary->setIconSize(s1);
    btNewScenary->setFixedSize(btNewScenary->rect().size());
    proxy = scene->addWidget(btNewScenary);

    QPushButton *btCurrentSave = new QPushButton();
    btCurrentSave->setGeometry(QRect(50,140,140,70));
    btCurrentSave->setText("   &Save");

    QPixmap pixmap_save(":graphics/save.jpeg");
    QIcon ButtonIcon1(pixmap_save);
    btCurrentSave->setIcon(ButtonIcon1);

    QSize s2(btCurrentSave->rect().size().width()-20,btCurrentSave->rect().size().height()-20);
    btCurrentSave->setIconSize(s2);
    btCurrentSave->setFixedSize(btCurrentSave->rect().size());
    proxy = scene->addWidget(btCurrentSave);

    QPushButton *btSaveAs = new QPushButton();
    btSaveAs->setGeometry(QRect(50,210,140,70));
    btSaveAs->setText("  S&ave \n    as...");

    QPixmap pixmap_saveAs(":graphics/saveas.jpeg");
    QIcon ButtonIcon2(pixmap_saveAs);
    btSaveAs->setIcon(ButtonIcon2);

    QSize s3(btSaveAs->rect().size().width()-20,btSaveAs->rect().size().height()-20);
    btSaveAs->setIconSize(s3);
    btSaveAs->setFixedSize(btSaveAs->rect().size());
    proxy = scene->addWidget(btSaveAs);


    QPushButton *btNewNeuron = new QPushButton();
    btNewNeuron->setGeometry(QRect(50,280,140,70));
    btNewNeuron->setText("     new \n  &Neuron");

    QPixmap pixmap_NewNeuron(":graphics/newNeuron.jpg");
    QIcon ButtonIcon3(pixmap_NewNeuron);
    btNewNeuron->setIcon(ButtonIcon3);

    QSize s4(btNewNeuron->rect().size().width()-40,btNewNeuron->rect().size().height()-20);
    btNewNeuron->setIconSize(s4);
    btNewNeuron->setFixedSize(btNewNeuron->rect().size());
    proxy = scene->addWidget(btNewNeuron);


    QPushButton *btNewGenerator = new QPushButton();
    btNewGenerator->setGeometry(QRect(50,350,140,70));
    btNewGenerator->setText("     new \n&Generator");

    QPixmap pixmap_NewGenerator(":graphics/newGenerator.jpg");
    QIcon ButtonIcon4(pixmap_NewGenerator);
    btNewGenerator->setIcon(ButtonIcon4);

    QSize s5(btNewGenerator->rect().size().width()-80,btNewGenerator->rect().size().height());
    btNewGenerator->setIconSize(s5);
    btNewGenerator->setFixedSize(btNewGenerator->rect().size());
    proxy = scene->addWidget(btNewGenerator);



    QPushButton *btNewSynapse = new QPushButton();
    btNewSynapse->setGeometry(QRect(50,420,140,70));
    btNewSynapse->setText("    new \n S&ynapse");

    QPixmap pixmap_NewSynapse(":graphics/newSynapse.jpg");
    QIcon ButtonIcon5(pixmap_NewSynapse);
    btNewSynapse->setIcon(ButtonIcon5);

    QSize s6(btNewSynapse->rect().size().width()-20,btNewSynapse->rect().size().height()-20);
    btNewSynapse->setIconSize(s6);
    btNewSynapse->setFixedSize(btNewSynapse->rect().size());
    proxy = scene->addWidget(btNewSynapse);


    QPushButton *btRemove = new QPushButton();
    btRemove->setGeometry(QRect(50,490,140,70));
    btRemove->setText("  &Remove \n  Neuron");

    QPixmap pixmap_RemoveNeuron(":graphics/removeNeuron.jpeg");
    QIcon ButtonIcon6(pixmap_RemoveNeuron);
    btRemove->setIcon(ButtonIcon6);

    QSize s7(btRemove->rect().size().width()-20,btRemove->rect().size().height()-20);
    btRemove->setIconSize(s7);
    btRemove->setFixedSize(btRemove->rect().size());
    proxy = scene->addWidget(btRemove);

    QPushButton *btExit = new QPushButton();
    btExit->setGeometry( QRect(1490,560,140,70));
    btExit->setText("   E&xit");
    QPixmap pixmap_Exit(":graphics/exit.png");
    QIcon ButtonIcon7(pixmap_Exit);
    btExit->setIcon(ButtonIcon7);
    QSize s8(btExit->rect().size().width()-20,btExit->rect().size().height()-20);
    btExit->setIconSize(s8);
    btExit->setFixedSize(btExit->rect().size());
    proxy = scene->addWidget(btExit);

    QPushButton *btSpiNNaker = new QPushButton();
    btSpiNNaker->setGeometry(QRect(50,560,140,70));
    btSpiNNaker->setText("   SpiNNa&ker");

    QPixmap pixmap_SpiNNaker(":graphics/exit.png");
    QIcon ButtonIcon8(pixmap_SpiNNaker);
    btExit->setIcon(ButtonIcon8);

    QSize s9(btSpiNNaker->rect().size().width()-20,btSpiNNaker->rect().size().height()-20);
    btExit->setIconSize(s9);
    btSpiNNaker->setFixedSize(btSpiNNaker->rect().size());
    proxy = scene->addWidget(btSpiNNaker);

    this->mathematicalModel=MODEL_ADEXLIF;

    QLabel *label0 = new QLabel();
    label0->setText("model - ");
    label0->setGeometry(QRect(650,-40,200,20));
    // Transparent and black
    label0->setStyleSheet("background-color: rgba(0,0,0,0); color: black;");

    QFont font = label0->font();
    font.setBold(true);
    font.setPointSize(18);
    label0->setFont(font);

    QLabel *label1 = new QLabel();
    label1->setText(mathematicalModel);
    label1->setGeometry(QRect(740,-40,200,20));
    label1->setStyleSheet("background-color: rgba(0,0,0,0); color: blue;");
    label1->setFont(font);

    proxy = scene->addWidget(label0);
    proxy = scene->addWidget(label1);

    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(200, 0, 1280, 630); // Window size

    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));

    this->setMinimumSize(1280, 630);
    this->setWindowTitle("Neural Scheme");

    connect(btload, SIGNAL(clicked()), this, SLOT(loadScene()));
    connect(btCurrentSave, SIGNAL(clicked()), this, SLOT(saveCurrentScene()));
    connect(btSaveAs, SIGNAL(clicked()), this, SLOT(saveSceneAs()));
    connect(btNewGenerator , SIGNAL(clicked()), this, SLOT(newGenerator()));
    connect(btNewNeuron, SIGNAL(clicked()), this, SLOT(newNeuron()));
    connect(btNewSynapse, SIGNAL(clicked()), this, SLOT(newSynapse()));
    connect(btNewScenary, SIGNAL(clicked()), this, SLOT(newScenary()));
    connect(btRemove, SIGNAL(clicked()), this, SLOT(newRemove()));
    connect(btSpiNNaker, SIGNAL(clicked()), this, SLOT(newSpiNNaker()));
    connect(btExit, &QPushButton::clicked, [=]() {close();});


    // We listen to the PROMISCUOUS_MOTHER_PORT any Neuron
    groupAddress=QHostAddress(IPM_MOTHER);
    udpSocket.bind(QHostAddress::AnyIPv4, GRAPHICS_PORT, QUdpSocket::ShareAddress);
    udpSocket.joinMulticastGroup(groupAddress);
    connect(&udpSocket, SIGNAL(readyRead()),this, SLOT(processPendingDatagrams()));

    // The window is positioned in the center of the screen
    this->move(QApplication::desktop()->availableGeometry().center() - this->rect().center());

    timerEmpty = new QTimer(this);
    timerEmpty->setInterval(1000);
    timerEmpty->stop();
    connect(timerEmpty, SIGNAL(timeout()), this, SLOT(emptyVectors()));
}

void MainGraphics::emptyVectors() {
    timerEmpty->stop();
    emptyVectorGraphicsEdges();
    emptyVectorGraphicsNodes();
}

int MainGraphics::getNumberItemOf(int typeOfItem) {
   int total=0;

   for (int idx=0; idx<vectorGraphicsNodes.size();idx++)
      if (vectorGraphicsNodes.at(idx)->typeNode==typeOfItem)
          total++;

   return total+1;

}

bool MainGraphics::existIp(QString ip) {

    bool encontrado=false;
    int idx=0;

    while ((!encontrado) && (idx<vectorGraphicsNodes.size())) {
        if (vectorGraphicsNodes.at(idx)->ipmSource==ip)
            encontrado=true;
        else idx++;
    }
    return encontrado;
}


void MainGraphics::restaurateIPSimulated(QString type) {
    if (type.toInt()==TYPENEURON_NORMAL) {
        sim_DHCP_NORMAL_A=DHCP_NORMAL_A;
        sim_DHCP_NORMAL_B=DHCP_NORMAL_B;
        sim_DHCP_NORMAL_C=DHCP_NORMAL_C;
        sim_DHCP_NORMAL_D=DHCP_NORMAL_D;
    }
    else if (type.toInt()==TYPENEURON_GENERATOR) {
        sim_DHCP_GENERATOR_A=DHCP_GENERATOR_A;
        sim_DHCP_GENERATOR_B=DHCP_GENERATOR_B;
        sim_DHCP_GENERATOR_C=DHCP_GENERATOR_C;
        sim_DHCP_GENERATOR_D=DHCP_GENERATOR_D;
    }
}

QString MainGraphics::generateIPSimulated(QString type) {

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

QString MainGraphics::generateIPReal(QString type) {

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

Node* MainGraphics::findNode(QString key){
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


void MainGraphics::processPendingDatagrams()
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
            Node *node = new Node(this,msg.field4.toFloat(),msg.field5.toFloat(), &localNeurons);
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

            p->V=msg.field11.toDouble();
            p->Iexc=msg.field12.toDouble();
            p->Iinh=msg.field13.toDouble();
            p->tau_e=msg.field14.toDouble();
            p->tau_i=msg.field15.toDouble();
            p->tau_v=msg.field16.toDouble();
            p->R=msg.field17.toDouble();
            p->Vr=msg.field18.toDouble();
            p->Vrh=msg.field19.toDouble();
            p->Vth=msg.field20.toDouble();
            p->At=msg.field21.toDouble();
            node->parameters=p;

            node->FormDialog=nullptr; // The Neuron has been created outside // (QDialog*) this;
            scene->addItem(node);
            vectorGraphicsNodes.push_back(node);
        }
        else if (msg.operation==CREATE_SYNAPSE_INTO_GRAPHIC){

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
            synap.port=(quint16) port;
            synap.type=typeSynapse;

            Node *node1=findNode(target);
            node1->synapsys.push_back(synap);
            if (node1->FormDialog!=nullptr)
                ((ModelAdExLIF *) node1->FormDialog)->showSynapsys();

            // It is NOT necessary to add synapses, because only Neurons are of interest
        }
        else if (msg.operation==REMOVE_NEURON_FROM_MOTHER_TO_GRAPHIC){
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
        else if (msg.operation==REMOVE_SYNAPSE_FROM_MOTHER_TO_GRAPHICS){
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
        else if (msg.operation==LOADSCENE_FROM_MOTHER_TO_GRAPHIC){

           loadSceneFromMsg(msg.field1);
        }
   }
}

void MainGraphics::newGenerator(){
    float posX= QRandomGenerator::global()->bounded(800)+250;
    float posY= QRandomGenerator::global()->bounded(450)+100;

    NewGeneratorGraphics *newGenerator = new NewGeneratorGraphics(nullptr,this,posX,posY,&idGlobalNeuron, &vectorGraphicsNodes,&localNeurons, &sceneBeSaved);
    newGenerator->setWindowModality(Qt::ApplicationModal);
    newGenerator->show();
}
void MainGraphics::newSpiNNaker(){
    NewSpiNNaker *SpiNNaker = new NewSpiNNaker(nullptr,&vectorGraphicsNodes,&vectorGraphicsJunction);
    SpiNNaker->setWindowModality(Qt::ApplicationModal);
    SpiNNaker->show();
}

void MainGraphics::newSynapse() {

    if (vectorGraphicsNodes.size()) {
        NewSynapseGraphic *newSynapse= new NewSynapseGraphic(nullptr,&idGlobalSynapse, scene, &vectorGraphicsNodes, &vectorGraphicsJunction, &sceneBeSaved);
        newSynapse->setWindowModality(Qt::ApplicationModal);
        newSynapse->show();
    }
    else
        QMessageBox::information(this, "Warning","No neuron has been created yet.");

}

void MainGraphics::removeALLScenary() {
    QString idGlobalSynapse="";
    QString ipSource="";
    QString ipTarget="";
    EncodeDecodeMsg msgx;

    for (int n=0; n<vectorGraphicsNodes.size();n++ ) {
       Node *node = vectorGraphicsNodes.at(n);

        for (int s=0; s<node->synapsys.size(); s++) {
           idGlobalSynapse= node->synapsys.at(s).idGlobal;
           ipSource=node->ipmSource;
           ipTarget= node->synapsys.at(s).ipmTarget;
           QString m=msgx.encondeMsg(REMOVE_SYNAPSE_FROM_GRAPHIC_TO_MOTHER,idGlobalSynapse,ipSource,ipTarget).toStdString().c_str();
           sendMsg(m,IPM_NEURON_PROMISCUOUS, NEURON_PROMISCUOS_PORT);
        }
    }
    // It must be checked if there are any synapses that are connected to the neuron, to be deleted as well
    EncodeDecodeMsg msg1;
    for (int n=0; n<vectorGraphicsNodes.size();n++ ) {
       Node *node = vectorGraphicsNodes.at(n);
       int s=0;
       ipSource=node->ipmSource;
        while (s<node->synapsys.size()) {
           if (node->synapsys.at(s).ipmTarget==node->ipmSource){
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

void MainGraphics::newScenary() {

    QMessageBox::StandardButton reply;
    QString msg="Would you like to save the current scenario?";

    if (sceneBeSaved)
        reply=QMessageBox::question(this, "Warning",msg,QMessageBox::Cancel|QMessageBox::Yes|QMessageBox::No);
    if (reply==QMessageBox::Cancel)
        return;

    if (reply==QMessageBox::Yes) {
        saveCurrentScene();
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


int MainGraphics::generateIdGlobalNeuron(){

    return ++idGlobalNeuron;
}


void MainGraphics::newNeuron(){

// CREATE NEURON and do the Form Widget Show
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


    NewNeuronGraphic *newNeuron = new NewNeuronGraphic(nullptr,this,scene,&idGlobalNeuron, &localNeurons, &vectorGraphicsNodes, mathematicalModel,&sceneBeSaved);
    newNeuron->setWindowModality(Qt::ApplicationModal);
    newNeuron->show();
}

void MainGraphics::newRemove() {

    if (vectorGraphicsNodes.size()==0)
        QMessageBox::information(this, "Warning","No neuron has been created yet.");
    else {
       Removeitem *removeitem= new Removeitem(nullptr, &vectorGraphicsNodes);
       removeitem->setWindowModality(Qt::ApplicationModal);
       removeitem->show();
    }
}

//void MainGraphics::updateDataNeuronsNeuron(QString ip, QString id, int N) {
   // It is not necessary to put anything here, since the Neuron constructor sends
   // a message to the Mother Neruon and registers itself
//}

//void MainGraphics::updateDataNeuronSypnapse(QString idGlobal, QString ipmSourceNeuron, QString ipmTarget, int type, quint16 port, double w, double fx){
// Se puede pegar el mismo código de CREATE_SYNAPSYS
// o bien enviar mensaje a motherNeuron para CREATE_SYNAPSYS

//}

void MainGraphics::saveCurrentScene(){
    if (fileNameToSave.length())
        saveScene(fileNameToSave);
    else
        saveSceneAs();
}
void MainGraphics::saveSceneAs(){

    QString fileName;

    if (!fileName.length()) {
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

void MainGraphics::saveScene(QString fileName) {

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
    xml.writeAttribute( QStringLiteral("MODEL"), mathematicalModel.toStdString().c_str()); //QStringLiteral(QString::strcat(mathematicalModel)));
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

        //Mathematical model parameters
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

//TODO: Crear otra funcion, por ejemplo, addRemoteNeuron, para cubrir el caso de añadir una neurona (raspi) remotamente y activa por MDHCP
void MainGraphics::addNeuron(int typeNeuron, QString ip,QString name, QString label, float posX, float posY, double scale,int amountOfNeurons, Parameters *p) {
    addNeuron(typeNeuron,ip,name,label,posX,posY,scale,p->V,p->Iexc,p->Iinh,p->tau_e,p->tau_i,p->tau_v,p->R,p->Vr,p->Vrh,p->Vth,p->At,amountOfNeurons);
}

void MainGraphics::sendMsg(QString message, QString ipmTarget, int port) {

    QUdpSocket udpSocket;
    QHostAddress groupAddress;
    groupAddress=QHostAddress(ipmTarget);
    udpSocket.bind(QHostAddress(ipmTarget), port, QUdpSocket::ShareAddress);
    udpSocket.setSocketOption(QAbstractSocket::MulticastTtlOption, TTL);

    QByteArray datagram = message.toStdString().c_str();
    udpSocket.writeDatagram(datagram, groupAddress, port);
}
void MainGraphics::addNeuron(int typeNeuron, QString ip, QString id, QString label, float posX, float posY, double scale,double V, double Iexc, double Iinh, double tauExc, double tauInh, double tauV, double R, double Vr, double Vrh, double Vth, double At, int amountOfNeurons) {

    double w=1.13;
    double fx=1E9;

    QString msg=QString("#")+sep_operation+CREATE_NEURON_FROM_NEURON_TO_MOTHER+sep_operation+id+separator+label+separator;
    msg+=QString::number(amountOfNeurons)+separator+ip+separator+QString::number(typeNeuron)+separator;
    msg+=QString::number(w)+separator+QString::number(fx)+separator;
    msg+=QString::number(V)+separator+QString::number(Iexc)+separator+QString::number(Iinh)+separator;
    msg+=QString::number(tauExc)+separator+QString::number(tauInh)+separator+QString::number(tauV)+separator;
    msg+=QString::number(R)+separator+QString::number(Vrh)+separator+QString::number(Vth)+separator;
    msg+=QString::number(At)+separator+QString::number(w)+separator+QString::number(fx)+separator;

    sendMsg(msg,IPM_MOTHER,NEURON_PROMISCUOS_PORT);
}


void MainGraphics::emptyLocalNeurons() {

    while (!localNeurons.empty()) {
        //Eliminamos los punteros a los FormDialog de cada neurona
        //Esto no tiene sentido es localNeuron y siempre es LOCAL_NEURON y no REMOTE_NEURON
        if (localNeurons.at(0)->localRemote==REMOTE_NEURON) {
            if (localNeurons.at(0)->typeNeuron==TYPENEURON_NORMAL) {
                delete ((ModelAdExLIF*) localNeurons[0]->FormDialog);
            }
            else if (localNeurons.at(0)->typeNeuron==TYPENEURON_GENERATOR) {
                //Este caso no esta contemplado. Lo ponemos por si en un futuro se
                //quiere tener un generator REMOTO.
            }
        }
        else { //LOCAL_NEURON
            if  ((localNeurons.at(0)->typeNeuron==TYPENEURON_NORMAL) ||
                (localNeurons.at(0)->typeNeuron==TYPENEURON_GENERATOR)) {
                    //No es necesario eliminar las sinapsis de localNeuron, ya que no se crean
            }
        }

        localNeurons.erase(localNeurons.begin());
    }
    localNeurons.clear();
}

void MainGraphics::emptyVectorGraphicsEdges() {

    while (!vectorGraphicsJunction.empty()) {
        vectorGraphicsJunction.erase(vectorGraphicsJunction.begin());
    }
    vectorGraphicsJunction.clear();
}

void MainGraphics::emptyVectorGraphicsNodes() {
    while (!vectorGraphicsNodes.empty()) {
        vectorGraphicsNodes.erase(vectorGraphicsNodes.begin());
    }
    vectorGraphicsNodes.clear();

}
void MainGraphics::loadSceneFromMsg(QString filename) {
    QXmlStreamReader xml;

    QFile file(filename);

        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Neuronal scheme"),
                                 tr("File %1:\n%2 could not be read.")
                                 .arg(QDir::toNativeSeparators(fileNameToSave),
                                      file.errorString()));
            return;
        }



         xml.setDevice(&file);

         QList<QGraphicsItem*> allGraphicsItems = scene->items();

         for(int i = 0; i < allGraphicsItems.size(); i++) {
             QGraphicsItem *graphicItem = allGraphicsItems[i];
             if (!graphicItem->toGraphicsObject()) { //No es un botón
                 scene->removeItem(graphicItem);
             delete graphicItem; //???
                 scene->update();
             }
         }

         QDomDocument XML;
         XML.setContent(&file);
         file.close();
         QDomElement root=XML.documentElement();
         std::cout<<"ROOT: "<<root.tagName().toStdString()<<std::endl;
         mathematicalModel=root.attribute("MODEL","No assigned");
         std::cout<<"MODEL: "<<mathematicalModel.toStdString()<<std::endl;
         QString version=root.attribute("version","No assigned");
         std::cout<<"VERSION: "<<version.toStdString()<<std::endl;

         QDomElement Component=root.firstChild().toElement();

         while(!Component.isNull())
         {
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
                 if (typeNeuron==TYPENEURON_NORMAL) {
                    DHCP_NORMAL_A= strlist.at(0).toInt();
                    DHCP_NORMAL_B= strlist.at(1).toInt();
                    DHCP_NORMAL_C= strlist.at(2).toInt();
                    DHCP_NORMAL_D= strlist.at(3).toInt();
                    restaurateIPSimulated(QString::number(TYPENEURON_NORMAL));
                 }
                 else if (typeNeuron==TYPENEURON_GENERATOR) {
                    DHCP_GENERATOR_A= strlist.at(0).toInt();
                    DHCP_GENERATOR_B= strlist.at(1).toInt();
                    DHCP_GENERATOR_C= strlist.at(2).toInt();
                    DHCP_GENERATOR_D= strlist.at(3).toInt();
                    restaurateIPSimulated(QString::number(TYPENEURON_GENERATOR));
                 }

                 idGlobalNeuron= id.toInt();
                 Parameters *p1= new Parameters(V,Iexc,Iinh,tauExc,tauInh,tauV,R,Vr,Vrh,Vth,At);
                 Neuron *n = new Neuron(nullptr,amountOfNeurons, label, posX, posY, ip, idGlobalNeuron, typeNeuron,LOCAL_NEURON, p1,1,"1E-9");
                 if (typeNeuron==TYPENEURON_GENERATOR) {
                    SpikeGenerator *spikeGenerator = new SpikeGenerator(nullptr,frecuency,ip);
                    n->FormDialog=spikeGenerator;
                 }
                 localNeurons.append(n);

             }
             else if (Component.tagName()=="Synapse") {

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
                 //Actualizamos el valor general de idGlobalSynapse

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

void MainGraphics::loadScene(){

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

void MainGraphics::itemMoved()
{
    if (!timerId)
       timerId = startTimer(1000 / 25);
}

void MainGraphics::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
        close();
        break;
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Space:
    case Qt::Key_Enter:
        shuffle();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}


#if QT_CONFIG(wheelevent)
void MainGraphics::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, -event->delta() / 240.0));
}

#endif

void MainGraphics::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    // Shadow
    QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
        painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
        painter->fillRect(bottomShadow, Qt::darkGray);

    // Fill
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersected(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);

    // Text
    QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
                    sceneRect.width() - 4, sceneRect.height() - 4);
    QString message(tr("Neural Scheme"));

    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(14);
    painter->setFont(font);
    painter->setPen(Qt::lightGray);
    painter->drawText(textRect.translated(2, 2), message);
    painter->setPen(Qt::black);
    painter->drawText(textRect, message);

}

void MainGraphics::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}


void MainGraphics::shuffle()
{
    foreach (QGraphicsItem *item, scene->items()) {
        if (qgraphicsitem_cast<Node *>(item)) {
            int x= QRandomGenerator::global()->bounded(800)+250;
            int y= QRandomGenerator::global()->bounded(450)+100;
            item->setPos(x,y);
        }
    }
}

void MainGraphics::zoomIn()
{
    scaleView(qreal(1.2));
}

void MainGraphics::zoomOut()
{
    scaleView(1 / qreal(1.2));
}
