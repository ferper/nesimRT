#include <QList>
#include "newsynapseGraphic.h"
#include "ui_newsynapseform.h"
#include <iostream>
#include "typedata.h"
#include <QRandomGenerator>
extern typeFxI fxI;

NewSynapseGraphic::NewSynapseGraphic(QWidget *parent, int *idGlobalSynapse, QGraphicsScene *scene, QList <Node*> *vectorGraphicsNeuron, QVector <JunctionsDraw*> *vectorGraphicsEdge, bool *sceneBeSaved):
    QWidget(parent),
    ui(new Ui::NewSynapseForm)
{
    if (vectorGraphicsNeuron->size()==0)
        QMessageBox::information(this, "Warning","There is no created neuron.");

    else {
        this->parent=parent;
        this->idGlobalSynapse=idGlobalSynapse;

        ui->setupUi(this);
        this->setWindowTitle("Create Synapse");

        this->vectorGraphicsNeuron=vectorGraphicsNeuron;
        this->vectorGraphicsEdge=vectorGraphicsEdge;
        this->scene=scene;
        this->sceneBeSaved=sceneBeSaved;

        fullTableWidgetdataNeurons(ui->tableWidget_Source);
        fullTableWidgetdataNeurons(ui->tableWidget_Target);
        neuronFilterSearch(0);
        neuronFilterSearch(1);

        typeFxI::const_iterator it=fxI.constBegin();

        for (;it!=fxI.end();++it)
           ui->comboBox_fx->addItem(it.key());

        ui->comboBox_fx->model()->sort(0);
        ui->comboBox_fx->setCurrentIndex(4);

        ui->lineEdit_w->setText("1.0");

        connect(ui->lineEdit_Source, SIGNAL(textChanged(const QString &)), this, SLOT(neuronFilterSearchSource()));
        connect(ui->lineEdit_Target, SIGNAL(textChanged(const QString &)), this, SLOT(neuronFilterSearchTarget()));

        QGraphicsDropShadowEffect* effect0 = new QGraphicsDropShadowEffect();
        QGraphicsDropShadowEffect* effect1 = new QGraphicsDropShadowEffect();
        effect0->setBlurRadius(100); //Adjust accordingly
        effect0->setOffset(3,3); //Adjust accordingly

        effect1->setBlurRadius(100); //Adjust accordingly
        effect1->setOffset(3,3); //Adjust accordingly

        ui->pushButton_Exit->setGraphicsEffect(effect0);
        ui->pushButton_Create->setGraphicsEffect(effect1);


        QPalette p = ui->tableWidget_Source->palette();
        p.setColor(QPalette::Base, QColor("#bbc9b6"));
        ui->tableWidget_Source->setPalette(p);

        p.setColor(QPalette::Base, QColor("#e0dca2"));
        ui->tableWidget_Target->setPalette(p);

        ui->tableWidget_Source->setStyleSheet("QTableWidget::item {""border: 1px solid black;""}");
        ui->tableWidget_Target->setStyleSheet("QTableWidget::item {""border: 1px solid black;""}");
    }
}

void NewSynapseGraphic::keyPressEvent(QKeyEvent *event){
    switch(event->key()) {
       case Qt::Key_Escape: // Pressing ESCAPE closes the window
          close();
          break;
    }
}

void NewSynapseGraphic::fullTableWidgetdataNeurons(QTableWidget *table){

    table->clearContents();
    table->setColumnCount(3);
    table->setRowCount(0);
    QStringList labels;
    labels << tr("IP address") << tr("Identification") << tr("Amount");
    table->setColumnWidth(0,100);
    table->setColumnWidth(1,100);
    table->setColumnWidth(2,70);

    table->setHorizontalHeaderLabels(labels);

    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setShowGrid(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    for (int i=0; i<vectorGraphicsNeuron ->size();i++) {
       QTableWidgetItem *tIp = new QTableWidgetItem();
       QTableWidgetItem *tlabel = new QTableWidgetItem();
       QTableWidgetItem *tAmount = new QTableWidgetItem();
       tIp->setText(vectorGraphicsNeuron->at(i)->ipmSource);
       tlabel->setText(vectorGraphicsNeuron->at(i)->label);
       tAmount->setText(QString::number(vectorGraphicsNeuron->at(i)->amountOfNeurons));
       int row = table->rowCount();

       table->insertRow(row);
       table->setItem(row, 0,tIp);
       table->setItem(row, 1,tlabel);
       table->setItem(row, 2,tAmount);
    }
    table->selectRow(0);
}
NewSynapseGraphic::~NewSynapseGraphic()
{
    delete ui;
}

void NewSynapseGraphic::neuronFilterSearch(int entity) {

    QTableWidget *table;
    int index=0;
    QString patron="";
    QString exclude1="";
    QString exclude2="";

    if (entity==0) { //lineEdit_Source
       table = ui->tableWidget_Source;
       patron= ui->lineEdit_Source->text().toUpper();
       index= ui->comboBox_Source->currentIndex();
       exclude1="MOTHER"; //Nobody can subscribe again to MotherNeuron

    }
    else { //lineEdit_Target
       table = ui->tableWidget_Target;
       patron= ui->lineEdit_Target->text().toUpper();
       index= ui->comboBox_Target->currentIndex();
       exclude1="MOTHER";
       exclude2="GENERATOR"; //A generator can not subscribe to a Neuron
    }

    table->clearContents();
    table->setColumnCount(3);
    table->setRowCount(0);
    QStringList labels;
    labels << tr("IP address") << tr("Identification") << tr("Amount");
    table->setColumnWidth(0,100);
    table->setColumnWidth(1,100);
    table->setColumnWidth(2,70);

    table->setHorizontalHeaderLabels(labels);

    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setShowGrid(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    bool matched=false;
    QString toTest="";
    for (int i=0; i<vectorGraphicsNeuron->size();i++) {
       if (index==0) // Ip Direction
          toTest=vectorGraphicsNeuron->at(i)->ipmSource;
       else if (index==1) //label
          toTest= vectorGraphicsNeuron->at(i)->label;
       else if (index==2) //Amount
          toTest= QString::number(vectorGraphicsNeuron->at(i)->amountOfNeurons);

       if (toTest.toUpper().contains(patron))
       matched=true;

       if (vectorGraphicsNeuron->at(i)->typeNode==TYPENEURON_MOTHER)
            matched=false;
       else if ((vectorGraphicsNeuron->at(i)->typeNode==TYPENEURON_GENERATOR) && (entity==1))
           matched=false;
       if (matched) {
          QTableWidgetItem *tIp = new QTableWidgetItem();
          QTableWidgetItem *tlabel = new QTableWidgetItem();
          QTableWidgetItem *tAmount = new QTableWidgetItem();
          tIp->setText(vectorGraphicsNeuron->at(i)->ipmSource);
          tlabel->setText(vectorGraphicsNeuron->at(i)->label);
          tAmount->setText(QString::number(vectorGraphicsNeuron->at(i)->amountOfNeurons));
          int row = table->rowCount();
          table->insertRow(row);
          table->setItem(row, 0,tIp);
          table->setItem(row, 1,tlabel);
          table->setItem(row, 2,tAmount);
       }
       matched=false;
    }
    table->selectRow(0);
}

void NewSynapseGraphic::neuronFilterSearchSource() {
    neuronFilterSearch(0);
}
void NewSynapseGraphic::neuronFilterSearchTarget() {
    neuronFilterSearch(1);
}

void NewSynapseGraphic::on_comboBox_Source_currentIndexChanged(int index)
{
    neuronFilterSearch(0);
}

void NewSynapseGraphic::on_comboBox_Target_currentIndexChanged(int index)
{
    neuronFilterSearch(1);
}

int NewSynapseGraphic::generate_Globalidsynapse(){
   //TODO: The mutex can be removed, as it is now done from the synapse itself
   mutexIdSynapse.lock();
   (*idGlobalSynapse)++;
   mutexIdSynapse.unlock();
   return *idGlobalSynapse;
}

void NewSynapseGraphic::sendMsg(QString msg, quint16 port) {

    QUdpSocket udpSocket;
    QHostAddress groupAddress;
    groupAddress=QHostAddress(IPM_NEURON_PROMISCUOUS);
    udpSocket.setSocketOption(QAbstractSocket::MulticastTtlOption, TTL);
    QByteArray datagram = msg.toStdString().c_str();
    udpSocket.writeDatagram(datagram, groupAddress, port);
}

void NewSynapseGraphic::on_pushButton_Create_clicked()
{
    QTableWidgetItem *item0 = ui->tableWidget_Source->item(ui->tableWidget_Source->currentRow(),0);
    QTableWidgetItem *item1 = ui->tableWidget_Target->item(ui->tableWidget_Target->currentRow(),0);
    if (item0 && item1) {

        QString ipmSource=ui->tableWidget_Source->item(ui->tableWidget_Source->currentRow(),0)->text();
        QString ipmTarget=ui->tableWidget_Target->item(ui->tableWidget_Target->currentRow(),0)->text();
        QString name=ui->tableWidget_Target->item(ui->tableWidget_Target->currentRow(),1)->text();
        //Source == GENERATOR == Target ==> It is NOT Possible
        if ((ui->tableWidget_Source->item(ui->tableWidget_Source->currentRow(),1)->text().contains("GENERATOR")) &&
                (ui->tableWidget_Target->item(ui->tableWidget_Target->currentRow(),1)->text().contains("GENERATOR"))) {

            QMessageBox::information(this, "Warning","A synapse between two generator neurons cannot be created.");
            return;
        }
        bool found=false;
        int idx=0;
        while ((!found) && (idx<vectorGraphicsNeuron->size())) {
            if (vectorGraphicsNeuron->at(idx)->ipmSource ==ipmSource)
                found=true;
            else
                idx++;
        }

        int typeNeuronSource=TYPENEURON_NORMAL;
        if (found)
           typeNeuronSource=  vectorGraphicsNeuron->at(idx)->typeNode;
        QString fx;
        bool ok;
        quint16 port=INTERNEURON_PORT; //Interneuron port by default;
        int typeSynapse=TYPE_SYP_EXCITATION; //Excitator by default;
        if (typeNeuronSource==TYPENEURON_GENERATOR)
            port=GENERATOR_PORT;
        if (ui->comboBox_type->currentIndex()==1)
            typeSynapse=TYPE_SYP_INHIBITION;
        double w= ui->lineEdit_w->text().toDouble(&ok);
        typeFxI::const_iterator ifx=fxI.cbegin();

        QString tmpFX= ui->comboBox_fx->itemText(ui->comboBox_fx->currentIndex());
        ifx=fxI.find(tmpFX);
        fx=ifx.key();

        if (!ok)
            QMessageBox::information(this, "Warning","The value of the W parameter of the synapse is not valid.");
        else {
            *idGlobalSynapse=generate_Globalidsynapse();

            QString msg=QString("#")+sep_operation+CREATE_SYNAPSE_INTO_MOTHER+sep_operation+ipmSource+separator;
            msg+=ipmTarget+separator+QString::number(port)+separator+QString::number(typeSynapse)+separator+QString::number(w)+separator+ifx.value();
            msg+=separator+ fx + separator+QString::number(typeNeuronSource)+separator+QString::number(*idGlobalSynapse)+separator;

            sendMsg(msg,NEURON_PROMISCUOS_PORT);

            QMessageBox::information(this, "Warning","The synapse has been succesfully created.");
        }
    }
    else {
        if (!item0)
            QMessageBox::information(this, "Warning","There is no source neuron selected.");
        else
            QMessageBox::information(this, "Warning","There is no target neuron selected.");
    }
}

void NewSynapseGraphic::on_pushButton_Exit_clicked()
{
    close();
}
