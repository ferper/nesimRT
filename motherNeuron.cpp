#include "motherNeuron.h"
#include "config.h"
#include "encodeDecodeMsg.h"
#include <QString>
#include <iostream>
#include "scheme/node.h"
#include "scheme/model_adexlif.h"

MotherNeuron::MotherNeuron(QWidget *parent, QString ipm,  MainGraphics *schemeNeuron)
    : QDialog(parent)
{
    this->schemeNeuron=schemeNeuron;

    groupAddress4=QHostAddress(ipm);

    auto ttlLabel = new QLabel(tr("TTL for IPv4 multicast datagrams:"));
    auto ttlSpinBox = new QSpinBox;
    ttlSpinBox->setRange(0, 255);

    auto ttlLayout = new QHBoxLayout;
    ttlLayout->addWidget(ttlLabel);
    ttlLayout->addWidget(ttlSpinBox);

    startButton = new QPushButton(tr("&Start"));
    auto quitButton = new QPushButton(tr("&Quit"));

    auto buttonBox = new QDialogButtonBox;
    buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);
    timer.setTimerType(Qt::PreciseTimer);
    connect(startButton, &QPushButton::clicked, this, &MotherNeuron::startSending);
    connect(quitButton, &QPushButton::clicked, this, &MotherNeuron::close);
    connect(&timer, &QTimer::timeout, this, &MotherNeuron::sendDatagram);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel);
    mainLayout->addLayout(ttlLayout);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Mother Neuron"));
    ttlSpinBox->setValue(1);


    //Escuachamos en el puerto IPM_NEURON_PROMISCUOUS cualquier neurona
    groupAddress4_allNeurons=QHostAddress(IPM_NEURON_PROMISCUOUS);
    udpSocket4_allNeurons.bind(QHostAddress::AnyIPv4, NEURON_PROMISCUOS_PORT, QUdpSocket::ShareAddress);
    udpSocket4_allNeurons.joinMulticastGroup(groupAddress4_allNeurons);
    connect(&udpSocket4_allNeurons, SIGNAL(readyRead()),this, SLOT(processPendingDatagramsPromiscuous()));
}

//Puede enviarse a todas las neuronas o al Graphic
void MotherNeuron::publicAMessage(QString msg, QString group, quint16 port){
   QHostAddress groupAddress=QHostAddress(group);
   QByteArray datagram = msg.toStdString().c_str();
   udpSocket4.writeDatagram(datagram, groupAddress, port);
}

//Aquí se recibe cualquier datagrama de cualquier Neurona e incluso de la parte grafica
void MotherNeuron::processPendingDatagramsPromiscuous()
{
    QByteArray datagram;
    QString data="";

    while (udpSocket4_allNeurons.hasPendingDatagrams()) {
        datagram.resize(int(udpSocket4_allNeurons.pendingDatagramSize()));
        udpSocket4_allNeurons.readDatagram(datagram.data(), datagram.size());
        data=datagram.constData();
        EncodeDecodeMsg msg;
        msg.decodeMsg(QString(data));
        if (msg.operation==ENABLE_SEND_GENERAL_MONITOR){  //Desde una Neurona se activa el Sending General Monitor
            typeDataNeurons::Iterator it;
            it=dataNeurons.find(msg.field1);
            if (it!=dataNeurons.end())
                it.value().sending=(msg.field2==QString::number(true));
        }
        else if (msg.operation==CREATE_NEURON_FROM_NEURON_TO_MOTHER) { //Creamos una neurona: Analiticamente y graficamente
            createNeuron(&msg);
        }
        else if (msg.operation==UPDATE_VALUES_SYNAPSE_FROM_NEURONWIDGET_TO_MOTHER) { //Actualizamos el valor de W para la sinapsis
            //Actualizamos la parte analitica de MOTHER
            typeDataNeurons::Iterator it;
            it=dataNeurons.find(msg.field1);
            if (it!=dataNeurons.end()) {
                double w=msg.field3.toDouble();
                QString fx_numberTxt=msg.field4;
                QString fx_unitMeasureTxt=msg.field5;

                int typeSynapse=msg.field6.toInt();
                int idx=0;
                bool encontrado=false;
                QString idGlobalSynapse=msg.field2;
                while ((!encontrado) && idx<it.value().synapsys.size()) {
                    if (it.value().synapsys[idx].idGlobal==msg.field2)
                        encontrado=true;
                    else
                        idx++;
                }
                if (encontrado) {
                   it.value().synapsys[idx].w=w;
                   it.value().synapsys[idx].fx_numberTxt=fx_numberTxt;
                   it.value().synapsys[idx].fx_unitMeasureTxt=fx_unitMeasureTxt;
                   it.value().synapsys[idx].type=typeSynapse;
                }
                //Enviamos a la Neurona el mensaje UPDATE_VALUES_SYNAPSE_FROM_MOTHER_TO_NEURON para actualizar su Vector de synapsys Vsynapse
                //que al modificarse, también se vera reflejado en Widget.cpp ya que es pasado por referencia

                EncodeDecodeMsg msg1;
                QString m=msg1.encondeMsg(UPDATE_VALUES_SYNAPSE_FROM_MOTHER_TO_NEURON,msg.field1,idGlobalSynapse,QString::number(w),fx_numberTxt, fx_unitMeasureTxt, QString::number(typeSynapse));
                publicAMessage(m ,IPM_MOTHER,MOTHER_PORT );

                //Para las neuronas que fueron creadas fuera y que solo tienen representacion grafica, tienen que actualizar el nuevo valor
                //Localizamos la Neurona grafica

                bool foundNode=false;
                int idxNode=0;
                while ((!foundNode) && (idxNode<schemeNeuron->vectorGraphicsNodes.size())) {
                    if (schemeNeuron->vectorGraphicsNodes.at(idxNode)->ipmSource==msg.field1)
                        foundNode=true;
                    else
                        idxNode++;
                }
                //Localizamos la sinapsis de la Neurona grafica
                bool foundSynapse=false;
                int idxSynapse=0;
                if (foundNode) {
                    while ((!foundSynapse) && (idxSynapse<schemeNeuron->vectorGraphicsNodes.at(idxNode)->synapsys.size())) {
                        if (schemeNeuron->vectorGraphicsNodes.at(idxNode)->synapsys.at(idxSynapse).idGlobal==idGlobalSynapse)
                            foundSynapse=true;
                        else
                            idxSynapse++;
                    }
                }
                if (foundSynapse) {
                    //Actualizamos los valores de la sinapsis en la etiqueta grafica
                    schemeNeuron->vectorGraphicsJunction[idGlobalSynapse.toInt()-1]->w=w;
                    schemeNeuron->vectorGraphicsJunction[idGlobalSynapse.toInt()-1]->fx=fx_unitMeasureTxt;
                    schemeNeuron->vectorGraphicsJunction[idGlobalSynapse.toInt()-1]->typeSynapse=typeSynapse;
                    //Para que refresque en tiempo real los nuevos valores graficos
                    schemeNeuron->vectorGraphicsJunction[idGlobalSynapse.toInt()-1]->adjust();
                    schemeNeuron->vectorGraphicsNodes[idxNode]->synapsys[idxSynapse].w=w;//*fx_numberTxt.toDouble();
                    schemeNeuron->vectorGraphicsNodes[idxNode]->synapsys[idxSynapse].fx_numberTxt=fx_numberTxt;
                    schemeNeuron->vectorGraphicsNodes[idxNode]->synapsys[idxSynapse].fx_unitMeasureTxt=fx_unitMeasureTxt;
                    schemeNeuron->vectorGraphicsNodes[idxNode]->synapsys[idxSynapse].type=typeSynapse;

                    //Si el Form de la neurona (node) está visible, entonces actualizar los cambios.
                    if (schemeNeuron->vectorGraphicsNodes[idxNode]->FormDialog!=nullptr)
                       ((ModelAdExLIF*) schemeNeuron->vectorGraphicsNodes[idxNode]->FormDialog)->showSynapsys();
                }
            }
        }
        else if (msg.operation==UPDATE_NUMBER_GROUP_NEURON) { //Actualizamos el valor del número de neuronas de un grupo
            typeDataNeurons::Iterator it;
            it=dataNeurons.find(msg.field1);
            if (it!=dataNeurons.end()) {
                int N=msg.field2.toInt();
                it.value().N=N;
            }
        }

        else if (msg.operation==REMOVE_SYNAPSE_MOTHER) { //Eliminamos una sinapsis enviado desde una neurona
            typeDataNeurons::Iterator it;
            it=dataNeurons.find(msg.field1);
            if (it!=dataNeurons.end()) {
                QString id=msg.field2;
                bool encontrado=false;
                int i=0;
                while (i<it.value().synapsys.size() && !encontrado) {
                    if (it.value().synapsys.at(i).idGlobal==id)
                        encontrado=true;
                    else
                        i++;
                }
                if (encontrado)
                    it.value().synapsys.remove(i);
            }

        }
        else if (msg.operation==CREATE_SYNAPSE_INTO_MOTHER) {

            QString source=msg.field1;
            QString target=msg.field2;
            int port = msg.field3.toInt();
            int typeSynapse=msg.field4.toInt();
            double w=msg.field5.toDouble();
            QString fx_numberTxt=msg.field6;
            QString fx_unitMeasureTxt=msg.field7;
            int typeNeuronTarget=msg.field8.toInt();
            QString idGlobalSynapse=msg.field9;

            //Registramos analiticamente la sinapsis
            addSynapse(idGlobalSynapse,source,target,typeSynapse,port,w, fx_numberTxt);

             //Mensaje para la neurona
            // #:CREATESNEURON:impS;impT;port;type;Wvalue;valueFxI;fx;typeNeuron;idGlobalSynapse
            QString msg1=QString("#")+sep_operation+CREATE_SYNAPSE_INTO_NEURON+sep_operation+source+separator;
            msg1+=target+separator+QString::number(port)+separator+QString::number(typeSynapse)+separator+QString::number(w)+separator+fx_numberTxt;
            msg1+=separator+ fx_unitMeasureTxt + separator+QString::number(typeNeuronTarget)+separator+idGlobalSynapse+separator;
            publicAMessage(msg1,IPM_MOTHER,MOTHER_PORT);


            //Mensaje para Graphics
            // #:CREATESGRAPHIC:impS;impT;port;type;Wvalue;valueFxI;fx;typeNeuron;idGlobalSynapse
            msg1=QString("#")+sep_operation+CREATE_SYNAPSE_INTO_GRAPHIC+sep_operation+source+separator;
            msg1+=target+separator+QString::number(port)+separator+QString::number(typeSynapse)+separator+QString::number(w)+separator+fx_numberTxt;
            msg1+=separator+ fx_unitMeasureTxt + separator+QString::number(typeNeuronTarget)+separator+idGlobalSynapse+separator;
            publicAMessage(msg1,IPM_MOTHER,GRAPHICS_PORT );
        }
        else if (msg.operation==UPDATE_MODEL_VALUES_PARAMETERS_FROM_WIDGET_TO_MOTHER) {
            //Averiguar si es una neurona local o es creada externamente
            bool isALocalNeuron=false;
            //Para las neuronas que fueron creadas fuera y que solo tienen representacion grafica, tienen que actualizar el nuevo valor
            //Localizamos la Neurona grafica
            if (!isALocalNeuron) {
                bool foundNode=false;
                int idxNode=0;
                while ((!foundNode) && (idxNode<schemeNeuron->vectorGraphicsNodes.size())) {
                    if (schemeNeuron->vectorGraphicsNodes.at(idxNode)->ipmSource==msg.field1)
                        foundNode=true;
                    else
                        idxNode++;
                }
                if (foundNode) {
                    schemeNeuron->vectorGraphicsNodes.at(idxNode)->parameters->V=msg.field3.toDouble();
                    schemeNeuron->vectorGraphicsNodes.at(idxNode)->parameters->Iexc=msg.field4.toDouble();
                    schemeNeuron->vectorGraphicsNodes.at(idxNode)->parameters->Iinh=msg.field5.toDouble();
                    schemeNeuron->vectorGraphicsNodes.at(idxNode)->parameters->tau_e=msg.field6.toDouble();
                    schemeNeuron->vectorGraphicsNodes.at(idxNode)->parameters->tau_i=msg.field7.toDouble();
                    schemeNeuron->vectorGraphicsNodes.at(idxNode)->parameters->tau_v=msg.field8.toDouble();
                    schemeNeuron->vectorGraphicsNodes.at(idxNode)->parameters->R=msg.field9.toDouble();
                    schemeNeuron->vectorGraphicsNodes.at(idxNode)->parameters->Vr=msg.field10.toDouble();
                    schemeNeuron->vectorGraphicsNodes.at(idxNode)->parameters->Vrh=msg.field11.toDouble();
                    schemeNeuron->vectorGraphicsNodes.at(idxNode)->parameters->Vth=msg.field12.toDouble();
                    schemeNeuron->vectorGraphicsNodes.at(idxNode)->parameters->At=msg.field13.toDouble();
                    schemeNeuron->vectorGraphicsNodes.at(idxNode)->amountOfNeurons= msg.field14.toInt();
                    schemeNeuron->vectorGraphicsNodes.at(idxNode)->label= msg.field15;
                    schemeNeuron->vectorGraphicsNodes.at(idxNode)->setLabel(msg.field15);
                    schemeNeuron->vectorGraphicsNodes.at(idxNode)->hide();
                    schemeNeuron->vectorGraphicsNodes.at(idxNode)->show();

                    //Si el Form de la neurona (node) está visible, entonces actualizar los cambios.
                    if (schemeNeuron->vectorGraphicsNodes[idxNode]->FormDialog!=nullptr)
                      ((ModelAdExLIF*) schemeNeuron->vectorGraphicsNodes[idxNode]->FormDialog)->showParameters();
                }
            }
        }
        else if (msg.operation==UPDATE_MODEL_VALUES_PARAMETERS_FROM_MODEL_TO_MOTHER) {

            QUdpSocket udpSocket4_MotherNeuron; //from mother neuron
            QHostAddress groupAddress4_to_MotherNeuron; //I'm listening to the neuron mother

            groupAddress4_to_MotherNeuron=QHostAddress(IPM_MOTHER);
            EncodeDecodeMsg msgtmp;
            QByteArray datagram = msgtmp.encondeMsg(UPDATE_MODEL_VALUES_PARAMETERS_FROM_MOTHER_TO_NEURON,msg.field1,msg.field2,msg.field3,msg.field4,msg.field5,msg.field6,msg.field7,msg.field8,msg.field9,msg.field10,msg.field11,msg.field12,msg.field13, msg.field14,msg.field15).toStdString().c_str();
            udpSocket4_MotherNeuron.writeDatagram(datagram, groupAddress4_to_MotherNeuron, MOTHER_PORT);

        }
        else if (msg.operation==REMOVE_SYNAPSE_FROM_GRAPHIC_TO_MOTHER) {
            // 1 - Ejecutar codigo de elminacion en MOTHER
            // 2 - Enviar mensaje a Neurona
            // 3 - Enviar mensaje a Graphic


            // 1 - Ejecutar codigo de elminacion en MOTHER
            //Eliminamos la sinapsis analitica perteneciente a MOTHER
            typeDataNeurons::iterator it=dataNeurons.begin();
            QString idGlobal= msg.field1;
            bool found=false;
            int idx=0;
            while ((it!=dataNeurons.end()) && (!found)) { //Nodes
                while (idx<it->synapsys.size() && (!found))
                   if (it->synapsys.at(idx).idGlobal==idGlobal)
                       found=true;
                   else
                       idx++;
                if (!found)
                   ++it;
            }
            if (found) {
                it->synapsys.remove(idx);
            }
            // 2 - Enviar mensaje a Neurona
            //Enviamos mensaje de eliminacion de synapsis a la neurona
            QString ipSource_tmp=msg.field2;
            QString ipTarget_tmp=msg.field3;
            EncodeDecodeMsg msg1;
            QString m0=msg1.encondeMsg(REMOVE_SYNAPSE_FROM_MOTHER_TO_NEURON,ipSource_tmp,idGlobal, ipTarget_tmp);
            publicAMessage(m0, IPM_MOTHER,MOTHER_PORT );

            // 3 - Enviar mensaje a Graphic
            //Enviamos mensaje de eliminacion de synapsis a la graphics
            EncodeDecodeMsg msg2;
            QString m1=msg2.encondeMsg(REMOVE_SYNAPSE_FROM_MOTHER_TO_GRAPHICS,ipSource_tmp,idGlobal, ipTarget_tmp);
            publicAMessage(m1, IPM_MOTHER,GRAPHICS_PORT );

        }
        else if (msg.operation==REMOVE_ALL_NEURON_FROM_GRAPHICS_TO_MOTHER_LOADSCENE){
            typeDataNeurons::iterator it=dataNeurons.begin();
            QVector <DataSynapsys>::iterator its;
            while (it!=dataNeurons.end()) {
                its=it.value().synapsys.begin();
                while (it.value().synapsys.size()>0){
                    its=it.value().synapsys.erase(its);
                }
                it=dataNeurons.erase(it);
            }
            dataNeurons.clear();
            // Enviar mensaje a todas las neuronas para que se mueran incluido los generadores :-)
            EncodeDecodeMsg msg2;
            QString m2=msg2.encondeMsg(REMOVE_ALL_NEURON_FROM_MOTHER_TO_NEURON_LOADSCENE);
            publicAMessage(m2, IPM_MOTHER,MOTHER_PORT );

            EncodeDecodeMsg msg3;
            QString m3=msg3.encondeMsg(LOADSCENE_FROM_MOTHER_TO_GRAPHIC,msg.field1);
            publicAMessage(m3, IPM_MOTHER, GRAPHICS_PORT);
        }
        else if (msg.operation==REMOVE_ALL_NEURON_FROM_GRAPHICS_TO_MOTHER){
            typeDataNeurons::iterator it=dataNeurons.begin();
            QVector <DataSynapsys>::iterator its;
            while (it!=dataNeurons.end()) {
                its=it.value().synapsys.begin();
                while (it.value().synapsys.size()>0){
                    its=it.value().synapsys.erase(its);
                }
                it=dataNeurons.erase(it);
            }
            dataNeurons.clear();
            // Enviar mensaje a todas las neuronas para que se mueran incluido los generadores :-)
            //Este mensaje lo reciben todas las neuronas
            EncodeDecodeMsg msg4;
            QString m4=msg4.encondeMsg(REMOVE_ALL_NEURON_FROM_MOTHER_TO_NEURON,"");
            publicAMessage(m4, IPM_MOTHER,MOTHER_PORT );
        }
        else if (msg.operation==REMOVE_NEURON_FROM_GRAPHIC_TO_MOTHER){
            //NOTA: Ya están borradas las sinapsis.
            QString ipSource=msg.field1;
            QString id=msg.field2;
            bool found=false;
            typeDataNeurons::iterator it=dataNeurons.begin();

            while ((!found) && (it!=dataNeurons.end())) {
               if (it.value().id==id)
                   found=true;
               else
                   ++it;
            }
            if (found)
                dataNeurons.erase(it);

            EncodeDecodeMsg msg2;
            QString m2=msg2.encondeMsg(REMOVE_NEURON_FROM_MOTHER_TO_NEURON,ipSource,id);
            publicAMessage(m2, IPM_MOTHER,MOTHER_PORT );

            EncodeDecodeMsg msg3;
            QString m3=msg2.encondeMsg(REMOVE_NEURON_FROM_MOTHER_TO_GRAPHIC,ipSource,id);
            publicAMessage(m3, IPM_MOTHER,GRAPHICS_PORT );
        }
        else if (msg.operation==REMOVE_ALL_SYNAPSYS_THE_OTHER_TOME_FROM_NEURON_TO_MOTHER) {
           QString ipSource=msg.field1;
           QString idneuron=msg.field2;
           QString ipTarget="";
           int n=0; //Recorrer todas las neuronas y todas las sinapsis
           while (n<schemeNeuron->vectorGraphicsNodes.size()) {
                int s=0;
                while (s<schemeNeuron->vectorGraphicsNodes.at(n)->synapsys.size()) {
                    if (schemeNeuron->vectorGraphicsNodes.at(n)->synapsys.at(s).ipmTarget==ipSource) {
                        QString  idGlobalSynapse= schemeNeuron->vectorGraphicsNodes.at(n)->synapsys.at(s).idGlobal;
                        ipTarget= schemeNeuron->vectorGraphicsNodes.at(n)->synapsys.at(s).ipmTarget;
                        QString S=schemeNeuron->vectorGraphicsNodes.at(n)->ipmSource;
                        EncodeDecodeMsg msx;
                        QString msg=msx.encondeMsg(REMOVE_SYNAPSE_FROM_GRAPHIC_TO_MOTHER,idGlobalSynapse,S,ipSource).toStdString().c_str();
                        publicAMessage(msg, IPM_MOTHER, NEURON_PROMISCUOS_PORT);
                    }
                    s++;
                }
                n++;
            }
        }
        else if (msg.operation==REQ_IP_DHCP) {
            QString MAC= msg.field1;
            QString type=msg.field2;
            schemeNeuron->idGlobalNeuron=schemeNeuron->generateIdGlobalNeuron();
            QString ip=schemeNeuron->generateIPReal(type);
            EncodeDecodeMsg ms;
            QStringList strlist;
            strlist=ip.split(".");
            schemeNeuron->sceneBeSaved=true;
            publicAMessage(ms.encondeMsg(ACK_REQ_IP_DHCP,MAC, ip ,QString::number(schemeNeuron->idGlobalNeuron),"E"+strlist.at(3)),IPM_MOTHER,MOTHER_PORT);
        }
    }
}

void MotherNeuron::createNeuron(EncodeDecodeMsg *msg) {

    QString id=msg->field1;
    QString label=msg->field2;

    float posX=msg->field3.toFloat();
    float posY=msg->field4.toFloat();
    int amountOfNeurons=msg->field5.toInt();
    QString ip =msg->field6;
    int typeNeuron=msg->field7.toInt();
    int localRemote=msg->field8.toInt();
    double w=msg->field9.toDouble();;
    double fx=msg->field10.toDouble();;

    //NOTA->FW: AQUI SE PUEDE PONER UN DETERMINANTE PARA IDENTIFICAR EL MODELO NEURONAL
    double V=msg->field11.toDouble();
    double Iexc=msg->field12.toDouble();
    double Iinh=msg->field13.toDouble();
    double tauExc=msg->field14.toDouble();
    double tauInh=msg->field15.toDouble();;
    double tauV=msg->field16.toDouble();;
    double R=msg->field17.toDouble();
    double Vr=msg->field18.toDouble();;
    double Vrh=msg->field19.toDouble();
    double Vth=msg->field20.toDouble();
    double At=msg->field21.toDouble();;

    QString msg1=QString("#")+sep_operation+CREATE_GRAPHIC_NEURON_FROM_MOTHER_TO_GRAPHICS+sep_operation+ip+separator;
    msg1+=id+separator+label+separator+QString::number(posX)+separator+QString::number(posY)+separator+QString::number(typeNeuron)+separator+QString::number(amountOfNeurons)+separator;
    msg1+=QString::number(localRemote)+separator+QString::number(w)+separator+QString::number(fx)+separator;
    msg1+=QString::number(V)+separator+QString::number(Iexc)+separator+QString::number(Iinh)+separator+QString::number(tauExc)+separator;
    msg1+=QString::number(tauInh)+separator+QString::number(tauV)+separator+QString::number(R)+separator+QString::number(Vr)+separator;
    msg1+=QString::number(Vrh)+separator+QString::number(Vth)+separator+QString::number(At)+separator;
    publicAMessage(msg1, IPM_MOTHER, (quint16) GRAPHICS_PORT);

    //Register the Neuron in MotherNeuron's TAD ANALITIC.
    addNeuron(typeNeuron,ip,id,amountOfNeurons,localRemote);
}

void MotherNeuron::addNeuron(int typeNeuron, QString ip, QString id, int N, int localRemote){
    DataStructNeuron data;
    data.typeNeuron=typeNeuron;
    data.id=id;
    data.N=N;
    data.sending=false;
    data.localRemote=localRemote;
    dataNeurons.insert(ip,data);
}

void MotherNeuron::addSynapse( QString idGlobal, QString ipmSourceNeuron, QString ipmTarget, int type, quint16 port, double w, QString fx_numberTxt){

    typeDataNeurons::Iterator it=dataNeurons.begin();
    it=dataNeurons.find(ipmSourceNeuron);
    if (it!=dataNeurons.end()) {
        DataSynapsys s;
        s.idGlobal=idGlobal;
        s.ipmTarget=ipmTarget;
        s.port=port;
        s.type=type;
        s.w=w;
        s.fx_numberTxt=fx_numberTxt;
        it.value().synapsys.push_back(s);
    }
}

//Borrar esto
void MotherNeuron::startSending()
{
    startButton->setEnabled(false);
    timer.start(1000);
}

void MotherNeuron::set_timerStop(){
    timer.stop();
}

void MotherNeuron::set_PushButton(bool state){
   startButton->setEnabled(state);
}
void MotherNeuron::set_resetMsg(){
   messageNo=1;
}

//Borrar esto
void MotherNeuron::sendDatagram()
{
    statusLabel->setText(tr("Now sending datagram %1").arg(messageNo));
    QByteArray datagram = "Multicast message " + QByteArray::number(messageNo);
    udpSocket4.writeDatagram(datagram, groupAddress4, 45000);
    ++messageNo;
}

