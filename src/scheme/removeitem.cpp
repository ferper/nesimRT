#include "removeitem.h"
#include "ui_removeitem.h"
#include <QList>
#include "encodeDecodeMsg.h"
#include <QMessageBox>
#include <iostream>
#include "publicmsg.h"
#include <QGraphicsDropShadowEffect>

Removeitem::Removeitem(QWidget *parent, QList <Node*> *vectorGraphicsNodes) :
    QWidget(parent),
    ui(new Ui::Removeitem)
{
    ui->setupUi(this);

    QGraphicsScene *sceneTmp = new QGraphicsScene();
    QPixmap p0(":graphics/neuron.png");
    p0=p0.scaledToWidth(ui->graphicsView->width()-5);
    p0=p0.scaledToHeight(ui->graphicsView->height()-5);

    sceneTmp->addPixmap(p0);
    ui->graphicsView->setScene(sceneTmp);

    sceneTmp = new QGraphicsScene();
    QPixmap p1(":graphics/equation.png");

    sceneTmp->addPixmap(p1);
    ui->graphicsView_2->setScene(sceneTmp);

    this->vectorGraphicsNodes=vectorGraphicsNodes;
    fillAllNeurons();
    if (vectorGraphicsNodes->size())
       fill_LocalSynapsys(vectorGraphicsNodes->at(0)->id);
    else
        fill_LocalSynapsys("");
    fill_GlobalSynapsys();

    ui->label_25->setText(MODEL_ADEXLIF);

    QPixmap pixmap_RemoveNeuron(":graphics/RemoveNeuron.png");
    QIcon ButtonIconRN(pixmap_RemoveNeuron);
    ui->pushButton->setIcon(ButtonIconRN);
    QSize sRN(ui->pushButton->rect().size().width()-10,ui->pushButton->rect().size().height()-10);
    ui->pushButton->setIconSize(sRN);
    ui->pushButton->setFixedSize(ui->pushButton->rect().size());
    ui->pushButton->setFlat(true);

    QPixmap pixmap_RemoveLocalNeuron(":graphics/RemoveLocalSynapse.png");
    QIcon ButtonIconRLN(pixmap_RemoveLocalNeuron);
    ui->pushButton_2->setIcon(ButtonIconRLN);
    QSize sRLN(ui->pushButton_2->rect().size().width()-10,ui->pushButton_2->rect().size().height()-10);
    ui->pushButton_2->setIconSize(sRLN);
    ui->pushButton_2->setFixedSize(ui->pushButton_2->rect().size());
    ui->pushButton_2->setFlat(true);

    QPixmap pixmap_RemoveGlobalNeuron(":graphics/RemoveGlobalSynapse.png");
    QIcon ButtonIconRGN(pixmap_RemoveGlobalNeuron);
    ui->pushButton_3->setIcon(ButtonIconRGN);
    QSize sRGN(ui->pushButton_3->rect().size().width()-10,ui->pushButton_3->rect().size().height()-10);
    ui->pushButton_3->setIconSize(sRLN);
    ui->pushButton_3->setFixedSize(ui->pushButton_3->rect().size());
    ui->pushButton_3->setFlat(true);

    QPixmap pixmap_Exit(":graphics/exit.png");
    QIcon ButtonIconExit(pixmap_Exit);
    ui->pushButton_8->setIcon(ButtonIconExit);
    QSize sExit(ui->pushButton_8->rect().size().width()-10,ui->pushButton_8->rect().size().height()-10);
    ui->pushButton_8->setIconSize(sExit);
    ui->pushButton_8->setFixedSize(ui->pushButton_8->rect().size());
    ui->pushButton_8->setFlat(true);

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

    ui->pushButton->setGraphicsEffect(effect0);
    ui->pushButton_2->setGraphicsEffect(effect1);
    ui->pushButton_3->setGraphicsEffect(effect2);
    ui->pushButton_8->setGraphicsEffect(effect3);

    QPalette p = ui->tableWidget_Neurons->palette();
    p.setColor(QPalette::Base, QColor("#bbc9b6"));
    ui->tableWidget_Neurons->setPalette(p);

    p.setColor(QPalette::Base, QColor("#e0dca2"));
    ui->tableWidget_LocalSynapsys->setPalette(p);

    p.setColor(QPalette::Base, QColor("#f5c1ec"));
    ui->tableWidget_GlobalSynapsys->setPalette(p);

    ui->tableWidget_Neurons->setStyleSheet("QTableWidget::item {""border: 1px solid black;""}");
    ui->tableWidget_LocalSynapsys->setStyleSheet("QTableWidget::item {""border: 1px solid black;""}");
    ui->tableWidget_GlobalSynapsys->setStyleSheet("QTableWidget::item {""border: 1px solid black;""}");

    connect(ui->lineEdit_Neuron, SIGNAL(textChanged(const QString &)), this, SLOT(neuronFilterSearch()));
    connect(ui->lineEdit_Local, SIGNAL(textChanged(const QString &)), this, SLOT(LocalSynapsysFilterSearch()));
    connect(ui->lineEdit_Global, SIGNAL(textChanged(const QString &)), this, SLOT(GlobalSynapsysFilterSearch()));

    // When messages are sent for deletion, the changes are refreshed on the screen
    connect(&timer_NeuronsRefresh, SIGNAL(timeout()), this, SLOT(neuronFilterSearch()));
    connect(&timer_LocalSynapsysRefresh, SIGNAL(timeout()), this, SLOT(LocalSynapsysFilterSearch()));
    connect(&timer_GlobalSynapsysRefresh, SIGNAL(timeout()), this, SLOT(GlobalSynapsysFilterSearch()));

}

// To strecht the images into the container
void Removeitem::showEvent(QShowEvent *) {
    ui->graphicsView->fitInView(ui->graphicsView->sceneRect(),Qt::KeepAspectRatio);
    ui->graphicsView_2->fitInView(ui->graphicsView_2->sceneRect(),Qt::KeepAspectRatio);
    on_tableWidget_Neurons_cellClicked(0,0);
}


void Removeitem::keyPressEvent(QKeyEvent *event){
    switch(event->key()) {
       case Qt::Key_Escape: // if ESCAPE key is pressed, the window is closed
          close();
          break;
    }
}

Node* Removeitem::findNode(QString key, QString value){
    bool founded=false;
    int idx=0;
    int total=vectorGraphicsNodes->size();
    while ((!founded) && (idx<total)) {
        if (key=="ip") {
            if (vectorGraphicsNodes->at(idx)->ip==value)
                founded=true;
            else
                idx++;
        }
        else if (key=="label") {
            if (vectorGraphicsNodes->at(idx)->label==value)
                founded=true;
            else
                idx++;

        }
        else if (key=="id") {
            if (vectorGraphicsNodes->at(idx)->id==value)
                founded=true;
            else
                idx++;
        }
    }
    if (founded)
        return vectorGraphicsNodes->at(idx);
    return (Node*) nullptr;
}

void Removeitem::fill_GlobalSynapsys(QString key, QString value) {

   Node *n;

   ui->tableWidget_GlobalSynapsys->clearContents();
   ui->tableWidget_GlobalSynapsys->setColumnCount(6);
   ui->tableWidget_GlobalSynapsys->setRowCount(0);
   QStringList labels;
   labels << tr("Source") << tr("Target") << tr("Type") << tr("Value") << tr("Amount") << tr("idGlobalSynapse");
   ui->tableWidget_GlobalSynapsys->setColumnWidth(0,100);
   ui->tableWidget_GlobalSynapsys->setColumnWidth(1,80);
   ui->tableWidget_GlobalSynapsys->setColumnWidth(2,30);
   ui->tableWidget_GlobalSynapsys->setColumnWidth(3,80);
   ui->tableWidget_GlobalSynapsys->setColumnWidth(4,60);
   ui->tableWidget_GlobalSynapsys->setColumnWidth(5,0); // It is 0, not to show it
   ui->tableWidget_GlobalSynapsys->setHorizontalHeaderLabels(labels);

   ui->tableWidget_GlobalSynapsys->setSelectionBehavior(QAbstractItemView::SelectRows);
   ui->tableWidget_GlobalSynapsys->setSelectionMode(QAbstractItemView::SingleSelection);
   ui->tableWidget_GlobalSynapsys->setShowGrid(true);
   ui->tableWidget_GlobalSynapsys->setEditTriggers(QAbstractItemView::NoEditTriggers);

   QString test0="";
   QString test1="";
   if (key.length()) {
       if (key=="source")
           test0=value.toUpper();
       else
           test1=value.toUpper();
   }

   for (int i=0; i<vectorGraphicsNodes->size();i++ ) {
            n=vectorGraphicsNodes->at(i);
            if (n->label.toUpper().contains(test0)) {
               for (int j=0; j<n->synapsys.size();j++) {
                    if (findNode("ip",n->synapsys.at(j).ipmTarget)->label.toUpper().contains(test1)) {
                        QTableWidgetItem *tSource = new QTableWidgetItem();
                        QTableWidgetItem *tTarget = new QTableWidgetItem();
                        QTableWidgetItem *tType = new QTableWidgetItem();
                        QTableWidgetItem *tValue = new QTableWidgetItem();
                        QTableWidgetItem *tAmount = new QTableWidgetItem();
                        QTableWidgetItem *tidGlobalSynapse = new QTableWidgetItem();

                        tTarget->setText(n->label);
                        tSource->setText(findNode("ip",n->synapsys.at(j).ipmSource)->label);


                        if (n->synapsys.at(j).type==TYPE_SYP_EXCITATION) {
                           tType->setText("Exc");

                           //ui->tableWidget_GlobalSynapsys->setStyleSheet("QTableView {selection-background-color: #000000; selection-color: #00FF00;}");
                        }
                        else {
                           tType->setText("Inh");

                           tSource->setForeground(QColor::fromRgb(255,0,0));
                           tTarget->setForeground(tSource->foreground());
                           tType->setForeground(tSource->foreground());
                           tValue->setForeground(tSource->foreground());
                           tAmount->setForeground(tSource->foreground());
                           tidGlobalSynapse->setForeground(tSource->foreground());
                        }
                        tValue->setText(QString::number(n->synapsys.at(j).w)+" "+n->synapsys.at(j).fx_unitMeasureTxt);
                        tAmount->setText(QString::number(n->amountOfNeurons));
                        tidGlobalSynapse->setText(n->synapsys.at(j).idGlobal);

                        int row = ui->tableWidget_GlobalSynapsys->rowCount();

                        ui->tableWidget_GlobalSynapsys->insertRow(row);
                        ui->tableWidget_GlobalSynapsys->setItem(row, 0,tSource);
                        ui->tableWidget_GlobalSynapsys->setItem(row, 1,tTarget);
                        ui->tableWidget_GlobalSynapsys->setItem(row, 2,tType);
                        ui->tableWidget_GlobalSynapsys->setItem(row, 3,tValue);
                        ui->tableWidget_GlobalSynapsys->setItem(row, 4,tAmount);
                        ui->tableWidget_GlobalSynapsys->setItem(row, 5,tidGlobalSynapse);

                    }
                 }
            }

        }
        ui->tableWidget_GlobalSynapsys->selectRow(0);
}

void Removeitem::fill_LocalSynapsys(QString ipNodeSource, QString key, QString value) {
    Node* node;

    ui->tableWidget_LocalSynapsys->clearContents();
    ui->tableWidget_LocalSynapsys->setColumnCount(6);
    ui->tableWidget_LocalSynapsys->setRowCount(0);
    QStringList labels;
    labels << tr("Label") << tr("Target") << tr("Type") << tr("Value") << tr("Amount") << tr("idGlobalSynapse");
    ui->tableWidget_LocalSynapsys->setColumnWidth(0,100);
    ui->tableWidget_LocalSynapsys->setColumnWidth(1,80);
    ui->tableWidget_LocalSynapsys->setColumnWidth(2,30);
    ui->tableWidget_LocalSynapsys->setColumnWidth(3,80);
    ui->tableWidget_LocalSynapsys->setColumnWidth(4,60);
    ui->tableWidget_LocalSynapsys->setColumnWidth(5,0); // It is 0, not to show it
    ui->tableWidget_LocalSynapsys->setHorizontalHeaderLabels(labels);
    ui->tableWidget_LocalSynapsys->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_LocalSynapsys->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_LocalSynapsys->setShowGrid(true);
    ui->tableWidget_LocalSynapsys->setEditTriggers(QAbstractItemView::NoEditTriggers);

    node=findNode("ip",ipNodeSource);

    QString test0="";
    QString test1="";
    if (key.length()) {
        if (key=="label")
            test0=value.toUpper();
        else
            test1=value.toUpper();
    }

    if (node) {
        for (int i=0; i<node->synapsys.size();i++) {
            if (findNode("ip",node->synapsys.at(i).ipmTarget)->label.toUpper().contains(test0) && (node->synapsys.at(i).ipmTarget.contains(test1))) {
                QTableWidgetItem *tLabel = new QTableWidgetItem();
                QTableWidgetItem *tTarget = new QTableWidgetItem();
                QTableWidgetItem *tType = new QTableWidgetItem();
                QTableWidgetItem *tValue = new QTableWidgetItem();
                QTableWidgetItem *tAmount = new QTableWidgetItem();
                QTableWidgetItem *tidGloablSynapse = new QTableWidgetItem();
                tLabel->setText(findNode("ip",node->synapsys.at(i).ipmTarget)->label);
                tTarget->setText(node->synapsys.at(i).ipmTarget);
                if (node->synapsys.at(i).type==TYPE_SYP_EXCITATION)
                   tType->setText("Exc");
                else {
                   tType->setText("Inh");

                   tLabel->setForeground(QColor::fromRgb(255,0,0));
                   tTarget->setForeground(tLabel->foreground());
                   tType->setForeground(tLabel->foreground());
                   tValue->setForeground(tLabel->foreground());
                   tAmount->setForeground(tLabel->foreground());
                }

                tValue->setText(QString::number(node->synapsys.at(i).w)+" "+node->synapsys.at(i).fx_unitMeasureTxt);
                tAmount->setText(QString::number(node->amountOfNeurons));
                tidGloablSynapse->setText(node->synapsys.at(i).idGlobal);
                int row = ui->tableWidget_LocalSynapsys->rowCount();

                ui->tableWidget_LocalSynapsys->insertRow(row);
                ui->tableWidget_LocalSynapsys->setItem(row, 0,tLabel);
                ui->tableWidget_LocalSynapsys->setItem(row, 1,tTarget);
                ui->tableWidget_LocalSynapsys->setItem(row, 2,tType);
                ui->tableWidget_LocalSynapsys->setItem(row, 3,tValue);
                ui->tableWidget_LocalSynapsys->setItem(row, 4,tAmount);
                ui->tableWidget_LocalSynapsys->setItem(row, 5,tidGloablSynapse);
            }

            ui->tableWidget_LocalSynapsys->selectRow(0);
         }
      }
}


Removeitem::~Removeitem()
{
    delete ui;
}

void Removeitem::fillAllNeurons(QString key, QString value) {

    ui->tableWidget_Neurons->clearContents();
    ui->tableWidget_Neurons->setColumnCount(3);
    ui->tableWidget_Neurons->setRowCount(0);
    QStringList labels;
    labels << tr("Label") << tr("IP Address") <<tr("ID") ;
    ui->tableWidget_Neurons->setColumnWidth(0,100);
    ui->tableWidget_Neurons->setColumnWidth(1,80);
    ui->tableWidget_Neurons->setColumnWidth(2,80); // It is 0, not to show it

    ui->tableWidget_Neurons->setHorizontalHeaderLabels(labels);
    ui->tableWidget_Neurons->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_Neurons->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_Neurons->setShowGrid(true);
    ui->tableWidget_Neurons->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QString test0="";
    QString test1="";
    if (key.length()) {
       if (key=="label")
          test0=value.toUpper();
       else
          test1=value.toUpper();
    }

    for (int i=0; i<vectorGraphicsNodes->size();i++) {
       if ((vectorGraphicsNodes->at(i)->typeNode==TYPENEURON_NORMAL) || (vectorGraphicsNodes->at(i)->typeNode==TYPENEURON_GENERATOR)) {
          if ((vectorGraphicsNodes->at(i)->label.toUpper().contains(test0) && (vectorGraphicsNodes->at(i)->ip.toUpper().contains(test1)))) {
             QTableWidgetItem *tlabel = new QTableWidgetItem();
             QTableWidgetItem *tIp = new QTableWidgetItem();
             QTableWidgetItem *tId = new QTableWidgetItem();

             tlabel->setText(vectorGraphicsNodes->at(i)->label);
             tIp->setText(vectorGraphicsNodes->at(i)->ip);
             tId->setText(vectorGraphicsNodes->at(i)->id);

             int row = ui->tableWidget_Neurons->rowCount();

             ui->tableWidget_Neurons->insertRow(row);
             ui->tableWidget_Neurons->setItem(row, 0,tlabel);
             ui->tableWidget_Neurons->setItem(row, 1,tIp);
             ui->tableWidget_Neurons->setItem(row, 2,tId);
          }
       }
       ui->tableWidget_Neurons->selectRow(0);
    }
}
bool Removeitem::NodeHasSynapsys(Node *node){

    bool exist=false;
    int n=0;
    while ((!exist) && (n<vectorGraphicsNodes->size())) {
        int s=0;
        while ((!exist) && (s<vectorGraphicsNodes->at(n)->synapsys.size())) {
            if (vectorGraphicsNodes->at(n)->synapsys.at(s).ipmTarget==node->ipmSource)
              exist=true;
           else
                s++;
        }
        if (!exist)
            n++;
    }
    return exist;
}

void Removeitem::on_pushButton_clicked()
{
    QMessageBox::StandardButton reply;
    QString msg="Are you sure you want to delete the selected neuron?";
    reply=QMessageBox::question(this, "Warinng",msg,QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    if (reply==QMessageBox::Yes) {
        QTableWidgetItem *item = ui->tableWidget_Neurons->item(ui->tableWidget_Neurons->currentRow(),0);

        if (item) {
            Node *node = findNode("id",ui->tableWidget_Neurons->item(ui->tableWidget_Neurons->currentRow(),2)->text());
            EncodeDecodeMsg msg;
            QString idGlobalSynapse;
            QString ipSource;
            QString ipTarget;

            for (int i=0; i<node->synapsys.size(); i++) {
                idGlobalSynapse= node->synapsys.at(i).idGlobal;
                ipSource=node->ipmSource;
                ipTarget= node->synapsys.at(i).ipmTarget;
                QString m=msg.encondeMsg(REMOVE_SYNAPSE_FROM_GRAPHIC_TO_MOTHER,idGlobalSynapse,ipSource,ipTarget).toStdString().c_str();
                sendMsg(m ,NEURON_PROMISCUOS_PORT);
            }
            // Check if there is any synapse that reaches the neuron, to erase it as well.
            EncodeDecodeMsg msg1;

           if (NodeHasSynapsys(node)){
                QMessageBox::StandardButton reply;
                QString msg="The neuron that you want to delete has synapses associated with other neurons.\n If you want to continue, these will be deleted too. Continue?";
                reply=QMessageBox::question(this, "Atención",msg,QMessageBox::Yes|QMessageBox::No);
                if (reply==QMessageBox::Yes) {
                    int n=0; // Traverse all neurons and all synapses
                    while (n<vectorGraphicsNodes->size()) {
                        int s=0;
                        ipSource=node->ipmSource;
                        while (s<vectorGraphicsNodes->at(n)->synapsys.size()) {
                            if (vectorGraphicsNodes->at(n)->synapsys.at(s).ipmTarget==node->ipmSource){

                                idGlobalSynapse= vectorGraphicsNodes->at(n)->synapsys.at(s).idGlobal;
                                ipTarget= vectorGraphicsNodes->at(n)->synapsys.at(s).ipmTarget;
                                QString m= msg1.encondeMsg(REMOVE_SYNAPSE_FROM_GRAPHIC_TO_MOTHER,idGlobalSynapse,ipSource,ipTarget).toStdString().c_str();
                                sendMsg(m ,NEURON_PROMISCUOS_PORT);
                            }
                            s++;
                        }
                        n++;
                    }
                  //Nota: FutureWork All neuron delete messages should be by idLGobalNeuron and not by ip.
                    QString m= msg1.encondeMsg(REMOVE_NEURON_FROM_GRAPHIC_TO_MOTHER,node->ipmSource,node->id).toStdString().c_str();
                    sendMsg(m ,NEURON_PROMISCUOS_PORT);
                    timer_NeuronsRefresh.start(1000);
                    timer_LocalSynapsysRefresh.start(1000);
                    timer_GlobalSynapsysRefresh.start(1000);
                }
            }
            else {
                QString m= msg1.encondeMsg(REMOVE_NEURON_FROM_GRAPHIC_TO_MOTHER,node->ipmSource,node->id).toStdString().c_str();
                sendMsg(m ,NEURON_PROMISCUOS_PORT);
                timer_NeuronsRefresh.start(1000);
                timer_LocalSynapsysRefresh.start(1000);
                timer_GlobalSynapsysRefresh.start(1000);
            }
        }
        else {
            QMessageBox::information(this, "Warning","No neuron has been created yet.");
        }
    }
}

void Removeitem::on_tableWidget_Neurons_cellClicked(int row, int column)
{
    QTableWidgetItem *itemIp = ui->tableWidget_Neurons->item(row,1);
    fill_LocalSynapsys(itemIp->text());
}

void Removeitem::sendMsg(QString msg, quint16 port) {

    QUdpSocket udpSocket;
    QHostAddress groupAddress;
    groupAddress=QHostAddress(IPM_NEURON_PROMISCUOUS);
    udpSocket.setSocketOption(QAbstractSocket::MulticastTtlOption, TTL);
    QByteArray datagram = msg.toStdString().c_str();
    udpSocket.writeDatagram(datagram, groupAddress, port);
}

void Removeitem::on_pushButton_3_clicked()
{

    QMessageBox::StandardButton reply;
    QString msg="Are you sure you want to delete the selected synapse?";
    reply=QMessageBox::question(this, "Atención",msg,QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    if (reply==QMessageBox::Yes) {
        QTableWidgetItem *item = ui->tableWidget_GlobalSynapsys->item(ui->tableWidget_GlobalSynapsys->currentRow(),5);
        if (item) {
            EncodeDecodeMsg msg;
            QString idGlobalSynapse= ui->tableWidget_GlobalSynapsys->item(ui->tableWidget_GlobalSynapsys->currentRow(),5)->text();
            QString ipSource=findNode("label",ui->tableWidget_GlobalSynapsys->item(ui->tableWidget_GlobalSynapsys->currentRow(),0)->text())->ipmSource;
            QString ipTarget= findNode("label",ui->tableWidget_GlobalSynapsys->item(ui->tableWidget_GlobalSynapsys->currentRow(),1)->text())->ipmSource;

            QString m=msg.encondeMsg(REMOVE_SYNAPSE_FROM_GRAPHIC_TO_MOTHER,idGlobalSynapse,ipSource,ipTarget).toStdString().c_str();

            sendMsg(m ,NEURON_PROMISCUOS_PORT);

            timer_NeuronsRefresh.start(1000);
            timer_LocalSynapsysRefresh.start(1000);
            timer_GlobalSynapsysRefresh.start(1000);
        }
        else {
            QMessageBox::information(this, "Warning","No synapse has been selected.");
        }
    }
}

void Removeitem::on_pushButton_8_clicked()
{
    close();
}


void Removeitem::neuronFilterSearch(){
    if (timer_NeuronsRefresh.isActive())
        timer_NeuronsRefresh.stop();
    QString key="";
    if (ui->comboBox_Neurons->currentIndex()==0)
        key="label";
    else
        key="Ip";

    fillAllNeurons(key,ui->lineEdit_Neuron->text());

    if (ui->tableWidget_Neurons->rowCount()>0)
       fill_LocalSynapsys(ui->tableWidget_Neurons->item(0,1)->text());
    else
       fill_LocalSynapsys("........"); // So that it does not show anything
}

void Removeitem::LocalSynapsysFilterSearch(){

    if (timer_LocalSynapsysRefresh.isActive())
        timer_LocalSynapsysRefresh.stop();

    QString ip="";
    if (ui->tableWidget_Neurons->rowCount()>0)
       ip=ui->tableWidget_Neurons->item(0,1)->text();
    else
       ip=("........"); // So that it does not show anything

    QString key="";
    if (ui->comboBox_Local->currentIndex()==0)
        key="label";
    else
        key="Ip";

    QString text=ui->lineEdit_Local->text();
    fill_LocalSynapsys(ip,key,ui->lineEdit_Local->text());

}
void Removeitem::GlobalSynapsysFilterSearch(){

    if (timer_GlobalSynapsysRefresh.isActive())
        timer_GlobalSynapsysRefresh.stop();

    QString key="source";
    QString value="";
    if (ui->comboBox_Global->currentIndex()==1)
       key="target";

    value=ui->lineEdit_Global->text();
    fill_GlobalSynapsys(key,value);

}

void Removeitem::on_pushButton_2_clicked()
{
    QMessageBox::StandardButton reply;
    QString msg="Are you sure you want to delete the selected synapse?";
    reply=QMessageBox::question(this, "Warning",msg,QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    if (reply==QMessageBox::Yes) {

        QTableWidgetItem *item = ui->tableWidget_LocalSynapsys->item(ui->tableWidget_LocalSynapsys->currentRow(),5);
        if (item) {

            EncodeDecodeMsg msg;
            QString idGlobalSynapse= ui->tableWidget_LocalSynapsys->item(ui->tableWidget_LocalSynapsys->currentRow(),5)->text();
            QString ipSource=ui->tableWidget_Neurons->item(ui->tableWidget_Neurons->currentRow(),1)->text();
            QString ipTarget= ui->tableWidget_LocalSynapsys->item(ui->tableWidget_LocalSynapsys->currentRow(),1)->text();

            QString m=msg.encondeMsg(REMOVE_SYNAPSE_FROM_GRAPHIC_TO_MOTHER,idGlobalSynapse,ipSource,ipTarget).toStdString().c_str();

            sendMsg(m ,NEURON_PROMISCUOS_PORT);

            timer_NeuronsRefresh.start(1000);
            timer_LocalSynapsysRefresh.start(1000);
            timer_GlobalSynapsysRefresh.start(1000);
        }

        else {
            QMessageBox::information(this, "Warning","No synapse has been selected.");
        }
    }
}
