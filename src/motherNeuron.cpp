#include "motherNeuron.h"
#include "config.h"
#include "encodeDecodeMsg.h"
#include <QString>
#include <iostream>
#include "scheme/node.h"
#include "scheme/model_adexlif.h"

MotherNeuron::MotherNeuron(QWidget *parent, NeuronScheme *schemeNeuron)
    : QDialog(parent)
{
    this->schemeNeuron=schemeNeuron;

    udpSocket4.bind(QHostAddress(QHostAddress::AnyIPv4), 0);
    udpSocket4.setSocketOption(QAbstractSocket::MulticastTtlOption, TTL);

    // Listening to the IPM_NEURON_PROMISCUOUS to any Neuron
    groupAddress4_allNeurons=QHostAddress(IPM_NEURON_PROMISCUOUS);
    udpSocket4_allNeurons.bind(QHostAddress::AnyIPv4, NEURON_PROMISCUOS_PORT, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    udpSocket4_allNeurons.joinMulticastGroup(groupAddress4_allNeurons);
    connect(&udpSocket4_allNeurons, SIGNAL(readyRead()),this, SLOT(processPendingDatagramsPromiscuous()));
}

// Can be sent to all Neuron or to the Graphic part
void MotherNeuron::publicAMessage(QString msg, QString group, quint16 port){
   QHostAddress groupAddress=QHostAddress(group);
   QByteArray datagram = msg.toStdString().c_str();
   udpSocket4.writeDatagram(datagram, groupAddress, port);
}

// Here, any datagram from any Neuron is received and even from the Graphic part
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
        if (msg.operation==ENABLE_SEND_GENERAL_MONITOR){  // From a Neuron the sending General Monitor is activated
            typeDataNeurons::Iterator it;
            it=dataNeurons.find(msg.field1);
            if (it!=dataNeurons.end())
                it.value().sending=(msg.field2==QString::number(true));
        }
        else if (msg.operation==CREATE_NEURON_FROM_NEURON_TO_MOTHER) { // A Neuron is created: Analytically and Graphically
            createNeuron(&msg);
        }
        else if (msg.operation==UPDATE_VALUES_SYNAPSE_FROM_NEURONWIDGET_TO_MOTHER) { // The W value for the synapse is updated
            // The analytical part of MOTHER is updated
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
                // The message UPDATE_VALUES_SYNAPSE_FROM_MOTHER_TO_NEURON is sent to the Neuron, to update his synapsys Vsynapse vector
                // that by modifying, is also reflected in Widget.cpp, because it is passed by reference

                EncodeDecodeMsg msg1;
                QString m=msg1.encondeMsg(UPDATE_VALUES_SYNAPSE_FROM_MOTHER_TO_NEURON,msg.field1,idGlobalSynapse,QString::number(w),fx_numberTxt, fx_unitMeasureTxt, QString::number(typeSynapse));
                publicAMessage(m, IPM_MOTHER, MOTHER_PORT );

                // So that the Neurons that had been created were, and that only have a Graphical representation, the new value is updated
                // The Graphical Neuron is located

                bool foundNode=false;
                int idxNode=0;
                while ((!foundNode) && (idxNode<schemeNeuron->vectorGraphicsNodes.size())) {
                    if (schemeNeuron->vectorGraphicsNodes.at(idxNode)->ipmSource==msg.field1)
                        foundNode=true;
                    else
                        idxNode++;
                }
                // The synapse of the Graphic Neuron is located
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
                    // Updates synapse values in Graphic label
                    schemeNeuron->vectorGraphicsJunction[idGlobalSynapse.toInt()-1]->w=w;
                    schemeNeuron->vectorGraphicsJunction[idGlobalSynapse.toInt()-1]->fx=fx_unitMeasureTxt;
                    schemeNeuron->vectorGraphicsJunction[idGlobalSynapse.toInt()-1]->typeSynapse=typeSynapse;

                    // the new values are refreshed in real time
                    schemeNeuron->vectorGraphicsJunction[idGlobalSynapse.toInt()-1]->adjust();
                    schemeNeuron->vectorGraphicsNodes[idxNode]->synapsys[idxSynapse].w=w;//*fx_numberTxt.toDouble();
                    schemeNeuron->vectorGraphicsNodes[idxNode]->synapsys[idxSynapse].fx_numberTxt=fx_numberTxt;
                    schemeNeuron->vectorGraphicsNodes[idxNode]->synapsys[idxSynapse].fx_unitMeasureTxt=fx_unitMeasureTxt;
                    schemeNeuron->vectorGraphicsNodes[idxNode]->synapsys[idxSynapse].type=typeSynapse;

                    // If the From of the Neuron Node is visible, the changes are updated
                    if (schemeNeuron->vectorGraphicsNodes[idxNode]->FormDialog!=nullptr)
                       ((ModelAdExLIF*) schemeNeuron->vectorGraphicsNodes[idxNode]->FormDialog)->showSynapsys();
                }
            }
        }
        else if (msg.operation==UPDATE_NUMBER_GROUP_NEURON) { // The value of the number of neurons in a group is updated
            typeDataNeurons::Iterator it;
            it=dataNeurons.find(msg.field1);
            if (it!=dataNeurons.end()) {
                int N=msg.field2.toInt();
                it.value().N=N;
            }
        }

        else if (msg.operation==REMOVE_SYNAPSE_MOTHER) { // A synapse sent from a Neuron is removed
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

            // Synapse is analytically recorded
            addSynapse(idGlobalSynapse,source,target,typeSynapse,port,w, fx_numberTxt);

            // Message for the neuron
            // #:CREATESNEURON:impS;impT;port;type;Wvalue;valueFxI;fx;typeNeuron;idGlobalSynapse
            QString msg1=QString("#")+sep_operation+CREATE_SYNAPSE_INTO_NEURON+sep_operation+source+separator;
            msg1+=target+separator+QString::number(port)+separator+QString::number(typeSynapse)+separator+QString::number(w)+separator+fx_numberTxt;
            msg1+=separator+ fx_unitMeasureTxt + separator+QString::number(typeNeuronTarget)+separator+idGlobalSynapse+separator;
            publicAMessage(msg1,IPM_MOTHER,MOTHER_PORT);


            // Message for the Graphics
            // #:CREATESGRAPHIC:impS;impT;port;type;Wvalue;valueFxI;fx;typeNeuron;idGlobalSynapse
            msg1=QString("#")+sep_operation+CREATE_SYNAPSE_INTO_GRAPHIC+sep_operation+source+separator;
            msg1+=target+separator+QString::number(port)+separator+QString::number(typeSynapse)+separator+QString::number(w)+separator+fx_numberTxt;
            msg1+=separator+ fx_unitMeasureTxt + separator+QString::number(typeNeuronTarget)+separator+idGlobalSynapse+separator;
            publicAMessage(msg1,IPM_MOTHER,GRAPHICS_PORT);
        }
        else if (msg.operation==UPDATE_MODEL_VALUES_PARAMETERS_FROM_WIDGET_TO_MOTHER) {
            // It is found out if it is local Neuron or it is created externally
            bool isALocalNeuron=false;
            // For Neurons that were created outside and that only have graphical representation, they have to update the new value 
            // The Graphical Neurons is located
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

                    // If the From of the Neuron Node is visible, the changes are updated
                    if (schemeNeuron->vectorGraphicsNodes[idxNode]->FormDialog!=nullptr)
                      ((ModelAdExLIF*) schemeNeuron->vectorGraphicsNodes[idxNode]->FormDialog)->showParameters();
                }
            }
        }
        else if (msg.operation==UPDATE_MODEL_VALUES_PARAMETERS_FROM_MODEL_TO_MOTHER) {

            //QUdpSocket udpSocket4_MotherNeuron; // From mother neuron
            QHostAddress groupAddress4_to_MotherNeuron; // The Neuron Mother is listening 

            EncodeDecodeMsg msgtmp;
            QByteArray datagram = msgtmp.encondeMsg(UPDATE_MODEL_VALUES_PARAMETERS_FROM_MOTHER_TO_NEURON,msg.field1,msg.field2,msg.field3,msg.field4,msg.field5,msg.field6,msg.field7,msg.field8,msg.field9,msg.field10,msg.field11,msg.field12,msg.field13, msg.field14,msg.field15).toStdString().c_str();
            groupAddress4_to_MotherNeuron=QHostAddress(IPM_MOTHER);
            udpSocket4.bind(QHostAddress(QHostAddress::AnyIPv4), 0);
            udpSocket4.writeDatagram(datagram, groupAddress4_to_MotherNeuron, MOTHER_PORT);
        }
        else if (msg.operation==REMOVE_SYNAPSE_FROM_GRAPHIC_TO_MOTHER) {
            // 1 - Run kill code on MOTHER
            // 2 - Send message to Neurona
            // 3 - Send message to Graphic


            // 1 - Run kill code on MOTHER
            // The analytic synapse beloging to MOTHER is eliminated
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
            // 2 - Send message to Neurona
            // Synapsis delete message is sent to Neuron
            QString ipSource_tmp=msg.field2;
            QString ipTarget_tmp=msg.field3;
            EncodeDecodeMsg msg1;
            QString m0=msg1.encondeMsg(REMOVE_SYNAPSE_FROM_MOTHER_TO_NEURON,ipSource_tmp,idGlobal, ipTarget_tmp);
            publicAMessage(m0, IPM_MOTHER,MOTHER_PORT );

            // 3 - Send message to Graphic
            // Synapsis delete message is sent Graphics
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
            // The message is sent to all Neurons so that even the generators die
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
            // The message is sent to all Neurons so that even the generators die
            // This message is received by all Neurons
            EncodeDecodeMsg msg4;
            QString m4=msg4.encondeMsg(REMOVE_ALL_NEURON_FROM_MOTHER_TO_NEURON,"");
            publicAMessage(m4, IPM_MOTHER,MOTHER_PORT );
        }
        else if (msg.operation==REMOVE_NEURON_FROM_GRAPHIC_TO_MOTHER){
            // NOTE: synapses are already deleted.
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
           int n=0; // All Neurons and all synapses are traversed
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
    double w=msg->field9.toDouble();
    double fx=msg->field10.toDouble();
    double V=msg->field11.toDouble();
    double Iexc=msg->field12.toDouble();
    double Iinh=msg->field13.toDouble();
    int neuronModel=msg->field14.toInt();
    double tauExc=msg->field15.toDouble();
    double tauInh=msg->field16.toDouble();
    double tauV=msg->field17.toDouble();
    double R=msg->field18.toDouble();
    double Vr=msg->field19.toDouble();
    double Vrh=msg->field20.toDouble();
    double Vth=msg->field21.toDouble();
    double At=msg->field22.toDouble();

    QString msg1=QString("#")+sep_operation+CREATE_GRAPHIC_NEURON_FROM_MOTHER_TO_GRAPHICS+sep_operation+ip+separator;
    msg1+=id+separator+label+separator+QString::number(posX)+separator+QString::number(posY)+separator+QString::number(typeNeuron)+separator+QString::number(amountOfNeurons)+separator;
    msg1+=QString::number(localRemote)+separator+QString::number(w)+separator+QString::number(fx)+separator;
    msg1+=QString::number(V)+separator+QString::number(Iexc)+separator+QString::number(Iinh)+separator+QString::number(neuronModel)+separator+QString::number(tauExc)+separator;
    msg1+=QString::number(tauInh)+separator+QString::number(tauV)+separator+QString::number(R)+separator+QString::number(Vr)+separator;
    msg1+=QString::number(Vrh)+separator+QString::number(Vth)+separator+QString::number(At)+separator;
    publicAMessage(msg1, IPM_MOTHER, (quint16) GRAPHICS_PORT);

    // Register the Neuron in MotherNeuron's TAD ANALITIC.
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
