#include "model_adexlif.h"
#include "ui_model_adexlif.h"
#include "encodeDecodeMsg.h"
#include <QMessageBox>
#include <QDesktopWidget>
#include <QFont>
extern typeFxI fxI;

ModelAdExLIF::ModelAdExLIF(QWidget *parent, Node *node) :
    QWidget(parent),
    ui(new Ui::ModelAdExLIF)
{
    ui->setupUi(this);

    mathematicalModel=MODEL_ADEXLIF;
    if (!node) {
        QMessageBox::critical(this, "Warning","No values have been established for this neuron");
    }
    else {
        this->node=node;
        showParameters();
        showSynapsys();
    }
    QGraphicsScene *sceneTmp = new QGraphicsScene();
    QPixmap p0("scheme/graphics/neuron.png");

    sceneTmp->addPixmap(p0);
    ui->graphicsView->setScene(sceneTmp);

    sceneTmp = new QGraphicsScene();
    QPixmap p1("scheme/graphics/equation.png");
    sceneTmp->addPixmap(p1);

    ui->graphicsView_2->setScene(sceneTmp);
    this->setWindowTitle("Change Neuron");

    QFont font = ui->label_model->font();
    font.setBold(true);
    font.setPointSize(18);
    ui->label_model->setFont(font);
    ui->label_modelContainer->setText(mathematicalModel);
    ui->label_modelContainer->setFont(font);

    fxI.clear();
    fxI.insert("MA","1e06");
    fxI.insert("kA","1e03");
    fxI.insert("A","1");
    fxI.insert("mA","1e-03");
    fxI.insert("uA","1e-06");
    fxI.insert("nA","1e-09");
    fxI.insert("pA","1e-12");

    ui->comboBox->clear();
    typeFxI::const_iterator it=fxI.constBegin();
    while (it!=fxI.constEnd()) {
       ui->comboBox->addItem(it.key());
       ++it;
    }

    connect(ui->tableWidget, SIGNAL(cellClicked (int, int) ),this, SLOT( cellSelected( int, int ) ) );


    QPixmap pixmap_exit("scheme/graphics/exit.png");
    QIcon ButtonIcon2(pixmap_exit);
    ui->pushButton_Exit->setIcon(ButtonIcon2);
    QSize s0(ui->pushButton_Exit->rect().size().width()-10,ui->pushButton_Exit->rect().size().height()-10);
    ui->pushButton_Exit->setIconSize(s0);
    ui->pushButton_Exit->setFixedSize(ui->pushButton_Exit->rect().size());
    ui->pushButton_Exit->setFlat(true);


    QPixmap pixmap_ApplyData("scheme/graphics/applyChangeNeuron.gif");
    QIcon ButtonIcon3(pixmap_ApplyData);
    ui->pushButton_applyData->setIcon(ButtonIcon3);
    QSize s1(ui->pushButton_applyData->rect().size().width()-10,ui->pushButton_applyData->rect().size().height()-10);
    ui->pushButton_applyData->setIconSize(s1);
    ui->pushButton_applyData->setFixedSize(ui->pushButton_applyData->rect().size());
    ui->pushButton_applyData->setFlat(true);

    QPixmap pixmap_ApplySynapse("scheme/graphics/applyChangeSynapse.png");
    QIcon ButtonIcon4(pixmap_ApplySynapse);
    ui->pushButton_ApplySynapse->setIcon(ButtonIcon4);
    QSize s2(ui->pushButton_ApplySynapse->rect().size().width()-10,ui->pushButton_ApplySynapse->rect().size().height()-10);
    ui->pushButton_ApplySynapse->setIconSize(s2);
    ui->pushButton_ApplySynapse->setFixedSize(ui->pushButton_ApplySynapse->rect().size());
    ui->pushButton_ApplySynapse->setFlat(true);


    QPixmap pixmap_Refresh("scheme/graphics/refresh.png");
    QIcon ButtonIcon5(pixmap_Refresh);
    ui->pushButton_Refresh->setIcon(ButtonIcon5);
    QSize s3(ui->pushButton_Refresh->rect().size().width()-10,ui->pushButton_ApplySynapse->rect().size().height()-10);
    ui->pushButton_Refresh->setIconSize(s2);
    ui->pushButton_Refresh->setFixedSize(ui->pushButton_Refresh->rect().size());
    ui->pushButton_Refresh->setFlat(true);

    QPixmap pixmap_Remove("scheme/graphics/removeSynapse.png");
    QIcon ButtonIcon6(pixmap_Remove);
    ui->pushButton_Remove->setIcon(ButtonIcon6);
    QSize s4(ui->pushButton_Remove->rect().size().width()-10,ui->pushButton_Remove->rect().size().height()-10);
    ui->pushButton_Remove->setIconSize(s2);
    ui->pushButton_Remove->setFixedSize(ui->pushButton_Remove->rect().size());
    ui->pushButton_Remove->setFlat(true);

    QGraphicsDropShadowEffect* effect0 = new QGraphicsDropShadowEffect();
    QGraphicsDropShadowEffect* effect1 = new QGraphicsDropShadowEffect();
    QGraphicsDropShadowEffect* effect2 = new QGraphicsDropShadowEffect();
    QGraphicsDropShadowEffect* effect3 = new QGraphicsDropShadowEffect();
    QGraphicsDropShadowEffect* effect4 = new QGraphicsDropShadowEffect();

    effect0->setBlurRadius(100);
    effect0->setOffset(3,3);

    effect1->setBlurRadius(100);
    effect1->setOffset(3,3);

    effect2->setBlurRadius(100);
    effect2->setOffset(3,3);

    effect3->setBlurRadius(100);
    effect3->setOffset(3,3);

    effect4->setBlurRadius(100);
    effect4->setOffset(3,3);

    ui->pushButton_Exit->setGraphicsEffect(effect0);
    ui->pushButton_applyData->setGraphicsEffect(effect1);
    ui->pushButton_ApplySynapse->setGraphicsEffect(effect2);
    ui->pushButton_Remove->setGraphicsEffect(effect3);
    ui->pushButton_Refresh->setGraphicsEffect(effect4);

    QPalette pal = ui->tableWidget->palette();
    pal.setColor(QPalette::Base, QColor("#bbc9b6"));
    ui->tableWidget->setPalette(pal);
    ui->tableWidget->setStyleSheet("QTableWidget::item {""border: 1px solid black;""}");

    this->move(QApplication::desktop()->availableGeometry().center() - this->rect().center());
}


//To strecht the images into the container
void ModelAdExLIF::showEvent(QShowEvent *) {
    ui->graphicsView->fitInView(ui->graphicsView->sceneRect(),Qt::KeepAspectRatio);
    ui->graphicsView_2->fitInView(ui->graphicsView_2->sceneRect(),Qt::KeepAspectRatio);
}

void ModelAdExLIF::keyPressEvent(QKeyEvent *event){
    switch(event->key()) {
       case Qt::Key_Escape: // si se pulsa ESCAPE se cierra la ventana
          close();
          break;
    }
}

void ModelAdExLIF::showParameters() {
    ui->lineEdit_Label->setText(node->label);
    ui->lineEdit_Ip->setText(node->ip);
    ui->lineEdit_V->setText(QString::number((double)node->parameters->V));
    ui->lineEdit_Iexc->setText(QString::number((double)node->parameters->Iexc));
    ui->lineEdit_Iinh->setText(QString::number((double)node->parameters->Iinh));
    ui->lineEdit_tauExc->setText(QString::number(node->parameters->tau_e));
    ui->lineEdit_tauInh->setText(QString::number(node->parameters->tau_i));
    ui->lineEdit_tauV->setText(QString::number(node->parameters->tau_v));
    ui->lineEdit_R->setText(QString::number(node->parameters->R));
    ui->lineEdit_Vr->setText(QString::number(node->parameters->Vr));
    ui->lineEdit_Vrh->setText(QString::number(node->parameters->Vrh));
    ui->lineEdit_Vth->setText(QString::number(node->parameters->Vth));
    ui->lineEdit_At->setText(QString::number(node->parameters->At));
    ui->lineEdit_Amount->setText(QString::number(node->amountOfNeurons));
}

void ModelAdExLIF::showSynapsys(){

    ui->tableWidget->clearContents();
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setRowCount(0);
    QStringList labels;
    labels << tr("id") << tr("Target") << tr("Tipo") << tr("Valor") << tr("Unidad");
    ui->tableWidget->setColumnWidth(0,30);
    ui->tableWidget->setColumnWidth(1,75);
    ui->tableWidget->setColumnWidth(2,30);
    ui->tableWidget->setColumnWidth(3,30);
    ui->tableWidget->setColumnWidth(4,30);

    ui->tableWidget->setHorizontalHeaderLabels(labels);

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);


    for (int i=0; i<node->synapsys.size();i++) {
            QTableWidgetItem *id = new QTableWidgetItem();
            QTableWidgetItem *target = new QTableWidgetItem();
            QTableWidgetItem *type = new QTableWidgetItem;
            QTableWidgetItem *value = new QTableWidgetItem;
            QTableWidgetItem *fx = new QTableWidgetItem;
            id->setText(node->synapsys.at(i).idGlobal);
            target->setText(node->synapsys.at(i).ipmTarget);
            if (node->synapsys.at(i).type==TYPE_SYP_EXCITATION)
                type->setText("Exc");
            else {
                type->setText("Inh");
                id->setForeground(QColor::fromRgb(255,0,0));
                target->setForeground(id->foreground());
                type->setForeground(id->foreground());
                value->setForeground(id->foreground());
                fx->setForeground(id->foreground());
            }

            //Recibimos "nA" o "pA" o "mA"...
            typeFxI::const_iterator ifx=fxI.cbegin();
            ifx=fxI.find(node->synapsys.at(i).fx_unitMeasureTxt);

            value->setText(QString::number(node->synapsys.at(i).w));
            fx->setText(ifx.key());

            int row = ui->tableWidget->rowCount();

            ui->tableWidget->insertRow(row);
            ui->tableWidget->setItem(row, 0,id);
            ui->tableWidget->setItem(row, 1,target);
            ui->tableWidget->setItem(row, 2,type);
            ui->tableWidget->setItem(row, 3,value);
            ui->tableWidget->setItem(row, 4,fx);

/*         TODO: Poner vector que contenga las quivalencias de unidades de medidas fx
                 y monstrar en la tabla las unidades en vez de los valores
                 Modificar los colores y poner el grosor del monitor igual que este, el de la neurona.
*/
        }
    }



void ModelAdExLIF::cellSelected(int row, int col) {
   QTableWidgetItem *itemValue = ui->tableWidget->item(row,3);
   QTableWidgetItem *itemTypeSynapse = ui->tableWidget->item(row,2);
   QTableWidgetItem *itemUnits = ui->tableWidget->item(row,4);
   ui->lineEdit->setText(itemValue->text());
   QString unit= itemUnits->text();
   QString typeSynapse=itemTypeSynapse->text();
   ui->comboBox_2->setCurrentIndex(ui->comboBox_2->findData(typeSynapse, Qt::DisplayRole));
   ui->comboBox->setCurrentIndex(ui->comboBox->findText(unit,Qt::MatchCaseSensitive));//, Qt::DisplayRole));

}

void ModelAdExLIF::show() {
   showSynapsys();
   ui->comboBox->setCurrentIndex(-1);
   ui->comboBox_2->addItem("Exc");
   ui->comboBox_2->addItem("Inh");
   ui->comboBox_2->setCurrentIndex(-1);

   QWidget::show();
}

ModelAdExLIF::~ModelAdExLIF()
{
    delete ui;
}


void ModelAdExLIF::on_pushButton_5_clicked()
{

    bool ok;
    ui->lineEdit->text().toDouble(&ok);
    QTableWidgetItem *itemid = ui->tableWidget->item(ui->tableWidget->currentRow(), 0);
    QTableWidgetItem *target = ui->tableWidget->item(ui->tableWidget->currentRow(), 1);
    QTableWidgetItem *itemTypeSynapse = ui->tableWidget->item(ui->tableWidget->currentRow(), 2);
    QTableWidgetItem *itemValue = ui->tableWidget->item(ui->tableWidget->currentRow(), 3);
    QTableWidgetItem *itemFx = ui->tableWidget->item(ui->tableWidget->currentRow(), 4);

    if (!ok)
        QMessageBox::information(this, "Warning","The value introduced for the synapse is not valid.");
    else if (!itemValue)
        QMessageBox::information(this, "Warning","You must select a synapse.");
    else {

        ui->tableWidget->setItem(ui->tableWidget->currentRow(), 1, itemTypeSynapse);
        itemTypeSynapse->setText(ui->comboBox_2->currentText());
        ui->tableWidget->setItem(ui->tableWidget->currentRow(), 2, itemValue);
        itemValue->setText(ui->lineEdit->text());
        ui->tableWidget->setItem(ui->tableWidget->currentRow(), 3, itemFx);
        itemFx->setText(ui->comboBox->currentText());

        QString id=itemid->text();
        QString fx_numberTxt=fxI.find(itemFx->text()).value();
        QString fx_unitMeasureTxt=fxI.find(itemFx->text()).key();
        double w=itemValue->text().toDouble();
        QString typeSynapse=itemTypeSynapse->text();
        bool encontrado=false;
        int idx=0;
        while ((!encontrado) && (idx<node->synapsys.size())) {
           if (node->synapsys.at(idx).idGlobal == id )
               encontrado=true;
           else
               idx++;
        }

        if (encontrado) {
            int type=TYPE_SYP_EXCITATION;
            if (typeSynapse=="Inh")
                type=TYPE_SYP_INHIBITION;

// LOS DATOS DE Vsynapse SON ACTUALIZADOS POR UN MENSAJE ENTRANTE DESDE MOTHER A NEURON

            QUdpSocket udpSocket4_MotherNeuron; //from mother neuron
            QHostAddress groupAddress4_to_MotherNeuron; //I'm listening to the neuron mother

            EncodeDecodeMsg msg;
            groupAddress4_to_MotherNeuron=QHostAddress(IPM_MOTHER);

            QByteArray datagram = msg.encondeMsg(UPDATE_VALUES_SYNAPSE_FROM_NEURONWIDGET_TO_MOTHER,node->ipmSource,id,QString::number(w),fx_numberTxt,fx_unitMeasureTxt,QString::number(type)).toStdString().c_str();
            udpSocket4_MotherNeuron.writeDatagram(datagram, groupAddress4_to_MotherNeuron, NEURON_PROMISCUOS_PORT);

            int pos=ui->comboBox->findText(fx_unitMeasureTxt,Qt::MatchCaseSensitive);
            ui->comboBox->setCurrentIndex(pos);
        }
    }
}

void ModelAdExLIF::on_pushButton_3_clicked()
{
    showSynapsys();
}

bool ModelAdExLIF::parmatersOK() {
    bool ok=true;
    bool error=false;
    int valueInt;
    double valueDouble;

    if (!ui->lineEdit_Label->text().length()) {
        QMessageBox::information(this, "Warning","A label is needed for identifying the neuron.\nIt will be provided automatically.");
        ui->lineEdit_Label->setText(QString("G"));
        error=true;
    }
    if (!error) {
        valueInt=ui->lineEdit_Amount->text().toInt(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value introduced is not valid.");
            error=true;
        }
    }
    if (!error) {
        valueDouble=ui->lineEdit_Iexc->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value of I Exc introduced is not valid.");
            error=true;
        }
    }

    if (!error) {
        valueDouble=ui->lineEdit_Iinh->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value of I Inh introduced is not valid.");
            error=true;
        }
    }

    if (!error) {
        valueDouble=ui->lineEdit_V->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value of V introduced is not valid.");
            error=true;
        }
    }


    if (!error) {
        valueDouble=ui->lineEdit_tauExc->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value of Tau Exc introduced is not valid.");
            error=true;
        }
    }

    if (!error) {
        valueDouble=ui->lineEdit_tauInh->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value of Tau Inh introduced is not valid.");
            error=true;
        }
    }

    if (!error) {
        valueDouble=ui->lineEdit_tauV->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value of Tau V introduced is not valid.");
            error=true;
        }
    }

    if (!error) {
        valueDouble=ui->lineEdit_R->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value of R introduced is not valid.");
            error=true;
        }
    }

    if (!error) {
        valueDouble=ui->lineEdit_Vr->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value of Vr introduced is not valid.");
            error=true;
        }
    }

    if (!error) {
        valueDouble=ui->lineEdit_Vrh->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value of Vrh introduced is not valid.");
            error=true;
        }
    }

    if (!error) {
        valueDouble=ui->lineEdit_Vth->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value of Vth introduced is not valid.");
            error=true;
        }
    }

    if (!error) {
        valueDouble=ui->lineEdit_At->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value of At introduced is not valid.");
            error=true;
        }
    }

    return !error;

}

void ModelAdExLIF::on_pushButton_4_clicked()
{
    QTableWidgetItem *item = ui->tableWidget->item(ui->tableWidget->currentRow(),0);
    if (item) {
        QString idGlobalSynapse=ui->tableWidget->item(ui->tableWidget->currentRow(),0)->text();
        QString ipTarget=ui->tableWidget->item(ui->tableWidget->currentRow(),1)->text();
        QString ipSource=node->ipmSource;

        ui->tableWidget->removeRow(ui->tableWidget->currentRow());

        EncodeDecodeMsg msg;
        QString m=msg.encondeMsg(REMOVE_SYNAPSE_FROM_GRAPHIC_TO_MOTHER,idGlobalSynapse,ipSource,ipTarget).toStdString().c_str();
        QUdpSocket udpSocket;
        QHostAddress groupAddress;

        groupAddress=QHostAddress(IPM_NEURON_PROMISCUOUS);
        udpSocket.setSocketOption(QAbstractSocket::MulticastTtlOption, TTL);
        QByteArray datagram = m.toStdString().c_str();
        udpSocket.writeDatagram(datagram, groupAddress, NEURON_PROMISCUOS_PORT);
    }
}


void ModelAdExLIF::on_lineEdit_Label_textEdited(const QString &arg1)
{
    ui->lineEdit_Label->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void ModelAdExLIF::on_lineEdit_Label_editingFinished()
{
    ui->lineEdit_Label->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void ModelAdExLIF::on_lineEdit_Amount_textEdited(const QString &arg1)
{
    ui->lineEdit_Amount->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void ModelAdExLIF::on_lineEdit_Amount_editingFinished()
{
    ui->lineEdit_Amount->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void ModelAdExLIF::on_lineEdit_tauExc_textEdited(const QString &arg1)
{
    ui->lineEdit_tauExc->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void ModelAdExLIF::on_lineEdit_tauExc_editingFinished()
{
    ui->lineEdit_tauExc->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void ModelAdExLIF::on_lineEdit_tauInh_textEdited(const QString &arg1)
{
    ui->lineEdit_tauInh->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void ModelAdExLIF::on_lineEdit_tauInh_editingFinished()
{
    ui->lineEdit_tauInh->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void ModelAdExLIF::on_lineEdit_tauV_textEdited(const QString &arg1)
{
    ui->lineEdit_tauV->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void ModelAdExLIF::on_lineEdit_tauV_editingFinished()
{
    ui->lineEdit_tauV->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void ModelAdExLIF::on_lineEdit_R_textEdited(const QString &arg1)
{
    ui->lineEdit_R->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void ModelAdExLIF::on_lineEdit_R_editingFinished()
{
    ui->lineEdit_R->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void ModelAdExLIF::on_lineEdit_Vr_textEdited(const QString &arg1)
{
    ui->lineEdit_Vr->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void ModelAdExLIF::on_lineEdit_Vr_editingFinished()
{
    ui->lineEdit_Vr->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void ModelAdExLIF::on_lineEdit_Vrh_textEdited(const QString &arg1)
{
    ui->lineEdit_Vrh->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void ModelAdExLIF::on_lineEdit_Vrh_editingFinished()
{
    ui->lineEdit_Vrh->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void ModelAdExLIF::on_lineEdit_Vth_textEdited(const QString &arg1)
{
    ui->lineEdit_Vth->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void ModelAdExLIF::on_lineEdit_Vth_editingFinished()
{
    ui->lineEdit_Vth->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void ModelAdExLIF::on_lineEdit_At_textEdited(const QString &arg1)
{
    ui->lineEdit_At->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void ModelAdExLIF::on_lineEdit_At_editingFinished()
{
    ui->lineEdit_At->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void ModelAdExLIF::on_lineEdit_textEdited(const QString &arg1)
{
    ui->lineEdit->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void ModelAdExLIF::on_lineEdit_editingFinished()
{
    ui->lineEdit->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void ModelAdExLIF::on_lineEdit_Amount_returnPressed()
{
    ui->lineEdit_V->setFocus();
}

void ModelAdExLIF::on_lineEdit_tauExc_returnPressed()
{
    ui->lineEdit_tauInh->setFocus();
}

void ModelAdExLIF::on_lineEdit_tauInh_returnPressed()
{
    ui->lineEdit_tauV->setFocus();
}

void ModelAdExLIF::on_lineEdit_tauV_returnPressed()
{
    ui->lineEdit_R->setFocus();
}

void ModelAdExLIF::on_lineEdit_R_returnPressed()
{
    ui->lineEdit_Vr->setFocus();
}

void ModelAdExLIF::on_lineEdit_Vr_returnPressed()
{
    ui->lineEdit_Vrh->setFocus();
}

void ModelAdExLIF::on_lineEdit_Vrh_returnPressed()
{
    ui->lineEdit_Vth->setFocus();
}

void ModelAdExLIF::on_lineEdit_Vth_returnPressed()
{
    ui->lineEdit_At->setFocus();
}

void ModelAdExLIF::on_lineEdit_At_returnPressed()
{
    ui->pushButton_applyData->setFocus();
}

void ModelAdExLIF::on_lineEdit_Label_returnPressed()
{
    ui->lineEdit_Amount->setFocus();
}

void ModelAdExLIF::on_lineEdit_V_textEdited(const QString &arg1)
{
    ui->lineEdit_V->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void ModelAdExLIF::on_lineEdit_V_editingFinished()
{
    ui->lineEdit_V->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void ModelAdExLIF::on_lineEdit_Iexc_textEdited(const QString &arg1)
{
    ui->lineEdit_Iexc->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void ModelAdExLIF::on_lineEdit_Iexc_editingFinished()
{
    ui->lineEdit_Iexc->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void ModelAdExLIF::on_lineEdit_Iinh_textEdited(const QString &arg1)
{
    ui->lineEdit_Iinh->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void ModelAdExLIF::on_lineEdit_Iinh_editingFinished()
{
    ui->lineEdit_Iinh->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void ModelAdExLIF::on_lineEdit_V_returnPressed()
{
    ui->lineEdit_Iexc->setFocus();
}

void ModelAdExLIF::on_lineEdit_Iexc_returnPressed()
{
    ui->lineEdit_Iinh->setFocus();
}

void ModelAdExLIF::on_lineEdit_Iinh_returnPressed()
{
    ui->lineEdit_tauExc->setFocus();
}

void ModelAdExLIF::on_pushButton_2_clicked()
{
    QString model= MODEL_ADEXLIF;
    bool ok=false;
    int valueInt;

    valueInt=ui->lineEdit_Amount->text().toInt(&ok);
    if (!ok)
       QMessageBox::information(this, "Warning","The value introduced is not valid.");
    else {
       node->amountOfNeurons=ui->lineEdit_Amount->text().toInt();

       QUdpSocket udpSocket4_MotherNeuron; //from mother neuron
       QHostAddress groupAddress4_to_MotherNeuron; //I'm listening to the neuron mother

       groupAddress4_to_MotherNeuron=QHostAddress(IPM_NEURON_PROMISCUOUS);
       EncodeDecodeMsg msg;
       QByteArray datagram = msg.encondeMsg(UPDATE_MODEL_VALUES_PARAMETERS_FROM_MODEL_TO_MOTHER,node->ipmSource,model,QString::number((double)node->parameters->V),QString::number((double)node->parameters->Iexc), QString::number((double)node->parameters->Iinh),QString::number(node->parameters->tau_e),QString::number(node->parameters->tau_i),QString::number(node->parameters->tau_v),QString::number(node->parameters->R),QString::number(node->parameters->Vr),QString::number(node->parameters->Vrh),QString::number(node->parameters->Vth),QString::number(node->parameters->At),QString::number(node->amountOfNeurons),node->label).toStdString().c_str();
       udpSocket4_MotherNeuron.writeDatagram(datagram, groupAddress4_to_MotherNeuron, NEURON_PROMISCUOS_PORT);
    }
}

void ModelAdExLIF::on_pushButton_19_clicked()
{

    QString model= MODEL_ADEXLIF;
    bool ok=false;

    if (!ui->lineEdit_Label->text().length()) {
        QMessageBox::information(this, "Warning","A label is needed for identifying the neuron.\nIt will be provided automatically");
        ui->lineEdit_Label->setText(QString("G"));
        ok=false;
    }

    else {
       node->label= ui->lineEdit_Label->text();
       node->hide();
       node->show();
       QUdpSocket udpSocket4_MotherNeuron; //from mother neuron
       QHostAddress groupAddress4_to_MotherNeuron; //I'm listening to the neuron mother

       groupAddress4_to_MotherNeuron=QHostAddress(IPM_NEURON_PROMISCUOUS);
       EncodeDecodeMsg msg;
       QByteArray datagram = msg.encondeMsg(UPDATE_MODEL_VALUES_PARAMETERS_FROM_MODEL_TO_MOTHER,node->ipmSource,model,QString::number((double)node->parameters->V),QString::number((double)node->parameters->Iexc), QString::number((double)node->parameters->Iinh),QString::number(node->parameters->tau_e),QString::number(node->parameters->tau_i),QString::number(node->parameters->tau_v),QString::number(node->parameters->R),QString::number(node->parameters->Vr),QString::number(node->parameters->Vrh),QString::number(node->parameters->Vth),QString::number(node->parameters->At),QString::number(node->amountOfNeurons),node->label).toStdString().c_str();
       udpSocket4_MotherNeuron.writeDatagram(datagram, groupAddress4_to_MotherNeuron, NEURON_PROMISCUOS_PORT);
    }
}


void ModelAdExLIF::on_pushButton_6_clicked()
{
    QString model= MODEL_ADEXLIF;
    bool ok=false;
    double valueDouble;

    valueDouble=ui->lineEdit_V->text().toDouble(&ok);
    if (!ok)
        QMessageBox::information(this, "Warning","The value of V introduced is not valid.");
    else {
        node->parameters->V=ui->lineEdit_V->text().toDouble();

       QUdpSocket udpSocket4_MotherNeuron; //from mother neuron
       QHostAddress groupAddress4_to_MotherNeuron; //I'm listening to the neuron mother

       groupAddress4_to_MotherNeuron=QHostAddress(IPM_NEURON_PROMISCUOUS);
       EncodeDecodeMsg msg;
       QByteArray datagram = msg.encondeMsg(UPDATE_MODEL_VALUES_PARAMETERS_FROM_MODEL_TO_MOTHER,node->ipmSource,model,QString::number((double)node->parameters->V),QString::number((double)node->parameters->Iexc), QString::number((double)node->parameters->Iinh),QString::number(node->parameters->tau_e),QString::number(node->parameters->tau_i),QString::number(node->parameters->tau_v),QString::number(node->parameters->R),QString::number(node->parameters->Vr),QString::number(node->parameters->Vrh),QString::number(node->parameters->Vth),QString::number(node->parameters->At),QString::number(node->amountOfNeurons),node->label).toStdString().c_str();
       udpSocket4_MotherNeuron.writeDatagram(datagram, groupAddress4_to_MotherNeuron, NEURON_PROMISCUOS_PORT);
    }
}


void ModelAdExLIF::on_pushButton_9_clicked()
{
    QString model= MODEL_ADEXLIF;
    bool ok=false;
    double valueDouble;

    valueDouble=ui->lineEdit_Iexc->text().toDouble(&ok);
    if (!ok) {
        QMessageBox::information(this, "Warning","The value of I Exc introduced is not valid.");
        ok=false;
    }

    else {
       node->parameters->Iexc=ui->lineEdit_Iexc->text().toDouble();

       QUdpSocket udpSocket4_MotherNeuron; //from mother neuron
       QHostAddress groupAddress4_to_MotherNeuron; //I'm listening to the neuron mother

       groupAddress4_to_MotherNeuron=QHostAddress(IPM_NEURON_PROMISCUOUS);
       EncodeDecodeMsg msg;
       QByteArray datagram = msg.encondeMsg(UPDATE_MODEL_VALUES_PARAMETERS_FROM_MODEL_TO_MOTHER,node->ipmSource,model,QString::number((double)node->parameters->V),QString::number((double)node->parameters->Iexc), QString::number((double)node->parameters->Iinh),QString::number(node->parameters->tau_e),QString::number(node->parameters->tau_i),QString::number(node->parameters->tau_v),QString::number(node->parameters->R),QString::number(node->parameters->Vr),QString::number(node->parameters->Vrh),QString::number(node->parameters->Vth),QString::number(node->parameters->At),QString::number(node->amountOfNeurons),node->label).toStdString().c_str();
       udpSocket4_MotherNeuron.writeDatagram(datagram, groupAddress4_to_MotherNeuron, NEURON_PROMISCUOS_PORT);
    }
}

void ModelAdExLIF::on_pushButton_10_clicked()
{

    QString model= MODEL_ADEXLIF;
    bool ok=false;
    double valueDouble;

    valueDouble=ui->lineEdit_Iinh->text().toDouble(&ok);
    if (!ok) {
        QMessageBox::information(this, "Warning","The value of I Inh introduced is not valid.");
        ok=false;
    }

    else {
       node->parameters->Iinh=ui->lineEdit_Iinh->text().toDouble();

       QUdpSocket udpSocket4_MotherNeuron; //from mother neuron
       QHostAddress groupAddress4_to_MotherNeuron; //I'm listening to the neuron mother

       groupAddress4_to_MotherNeuron=QHostAddress(IPM_NEURON_PROMISCUOUS);
       EncodeDecodeMsg msg;
       QByteArray datagram = msg.encondeMsg(UPDATE_MODEL_VALUES_PARAMETERS_FROM_MODEL_TO_MOTHER,node->ipmSource,model,QString::number((double)node->parameters->V),QString::number((double)node->parameters->Iexc), QString::number((double)node->parameters->Iinh),QString::number(node->parameters->tau_e),QString::number(node->parameters->tau_i),QString::number(node->parameters->tau_v),QString::number(node->parameters->R),QString::number(node->parameters->Vr),QString::number(node->parameters->Vrh),QString::number(node->parameters->Vth),QString::number(node->parameters->At),QString::number(node->amountOfNeurons),node->label).toStdString().c_str();
       udpSocket4_MotherNeuron.writeDatagram(datagram, groupAddress4_to_MotherNeuron, NEURON_PROMISCUOS_PORT);
    }
}

void ModelAdExLIF::on_pushButton_11_clicked()
{
    QString model= MODEL_ADEXLIF;
    bool ok=false;
    double valueDouble;

    valueDouble=ui->lineEdit_tauExc->text().toDouble(&ok);
    if (!ok) {
        QMessageBox::information(this, "Warning","The value of Tau Exc introduced is not valid.");
        ok=false;
    }

    else {
       node->parameters->tau_e=ui->lineEdit_tauExc->text().toDouble();

       QUdpSocket udpSocket4_MotherNeuron; //from mother neuron
       QHostAddress groupAddress4_to_MotherNeuron; //I'm listening to the neuron mother

       groupAddress4_to_MotherNeuron=QHostAddress(IPM_NEURON_PROMISCUOUS);
       EncodeDecodeMsg msg;
       QByteArray datagram = msg.encondeMsg(UPDATE_MODEL_VALUES_PARAMETERS_FROM_MODEL_TO_MOTHER,node->ipmSource,model,QString::number((double)node->parameters->V),QString::number((double)node->parameters->Iexc), QString::number((double)node->parameters->Iinh),QString::number(node->parameters->tau_e),QString::number(node->parameters->tau_i),QString::number(node->parameters->tau_v),QString::number(node->parameters->R),QString::number(node->parameters->Vr),QString::number(node->parameters->Vrh),QString::number(node->parameters->Vth),QString::number(node->parameters->At),QString::number(node->amountOfNeurons),node->label).toStdString().c_str();
       udpSocket4_MotherNeuron.writeDatagram(datagram, groupAddress4_to_MotherNeuron, NEURON_PROMISCUOS_PORT);
    }
}

void ModelAdExLIF::on_pushButton_12_clicked()
{
    QString model= MODEL_ADEXLIF;
    bool ok=false;
    double valueDouble;

    valueDouble=ui->lineEdit_tauInh->text().toDouble(&ok);
    if (!ok) {
        QMessageBox::information(this, "Warning","The value of Tau Inh introduced is not valid.");
        ok=false;
    }

    else {
       node->parameters->tau_i=ui->lineEdit_tauInh->text().toDouble();

       QUdpSocket udpSocket4_MotherNeuron; //from mother neuron
       QHostAddress groupAddress4_to_MotherNeuron; //I'm listening to the neuron mother

       groupAddress4_to_MotherNeuron=QHostAddress(IPM_NEURON_PROMISCUOUS);
       EncodeDecodeMsg msg;
       QByteArray datagram = msg.encondeMsg(UPDATE_MODEL_VALUES_PARAMETERS_FROM_MODEL_TO_MOTHER,node->ipmSource,model,QString::number((double)node->parameters->V),QString::number((double)node->parameters->Iexc), QString::number((double)node->parameters->Iinh),QString::number(node->parameters->tau_e),QString::number(node->parameters->tau_i),QString::number(node->parameters->tau_v),QString::number(node->parameters->R),QString::number(node->parameters->Vr),QString::number(node->parameters->Vrh),QString::number(node->parameters->Vth),QString::number(node->parameters->At),QString::number(node->amountOfNeurons),node->label).toStdString().c_str();
       udpSocket4_MotherNeuron.writeDatagram(datagram, groupAddress4_to_MotherNeuron, NEURON_PROMISCUOS_PORT);
    }
}

void ModelAdExLIF::on_pushButton_13_clicked()
{
    QString model= MODEL_ADEXLIF;
    bool ok=false;
    double valueDouble;

    valueDouble=ui->lineEdit_tauV->text().toDouble(&ok);
    if (!ok) {
        QMessageBox::information(this, "Warning","The value of Tau V introduced is not valid.");
        ok=false;
    }
    else {
       node->parameters->tau_v=ui->lineEdit_tauV->text().toDouble();

       QUdpSocket udpSocket4_MotherNeuron; //from mother neuron
       QHostAddress groupAddress4_to_MotherNeuron; //I'm listening to the neuron mother

       groupAddress4_to_MotherNeuron=QHostAddress(IPM_NEURON_PROMISCUOUS);
       EncodeDecodeMsg msg;
       QByteArray datagram = msg.encondeMsg(UPDATE_MODEL_VALUES_PARAMETERS_FROM_MODEL_TO_MOTHER,node->ipmSource,model,QString::number((double)node->parameters->V),QString::number((double)node->parameters->Iexc), QString::number((double)node->parameters->Iinh),QString::number(node->parameters->tau_e),QString::number(node->parameters->tau_i),QString::number(node->parameters->tau_v),QString::number(node->parameters->R),QString::number(node->parameters->Vr),QString::number(node->parameters->Vrh),QString::number(node->parameters->Vth),QString::number(node->parameters->At),QString::number(node->amountOfNeurons),node->label).toStdString().c_str();
       udpSocket4_MotherNeuron.writeDatagram(datagram, groupAddress4_to_MotherNeuron, NEURON_PROMISCUOS_PORT);
    }
}

void ModelAdExLIF::on_pushButton_14_clicked()
{
    QString model= MODEL_ADEXLIF;
    bool ok=false;
    double valueDouble;

    valueDouble=ui->lineEdit_R->text().toDouble(&ok);
    if (!ok) {
        QMessageBox::information(this, "Warning","The value of R introduced is not valid.");
        ok=false;
    }
    else {
       node->parameters->R=ui->lineEdit_R->text().toDouble();

       QUdpSocket udpSocket4_MotherNeuron; //from mother neuron
       QHostAddress groupAddress4_to_MotherNeuron; //I'm listening to the neuron mother

       groupAddress4_to_MotherNeuron=QHostAddress(IPM_NEURON_PROMISCUOUS);
       EncodeDecodeMsg msg;
       QByteArray datagram = msg.encondeMsg(UPDATE_MODEL_VALUES_PARAMETERS_FROM_MODEL_TO_MOTHER,node->ipmSource,model,QString::number((double)node->parameters->V),QString::number((double)node->parameters->Iexc), QString::number((double)node->parameters->Iinh),QString::number(node->parameters->tau_e),QString::number(node->parameters->tau_i),QString::number(node->parameters->tau_v),QString::number(node->parameters->R),QString::number(node->parameters->Vr),QString::number(node->parameters->Vrh),QString::number(node->parameters->Vth),QString::number(node->parameters->At),QString::number(node->amountOfNeurons),node->label).toStdString().c_str();
       udpSocket4_MotherNeuron.writeDatagram(datagram, groupAddress4_to_MotherNeuron, NEURON_PROMISCUOS_PORT);
    }
}

void ModelAdExLIF::on_pushButton_15_clicked()
{
    QString model= MODEL_ADEXLIF;
    bool ok=false;
    double valueDouble;

    valueDouble=ui->lineEdit_Vr->text().toDouble(&ok);
    if (!ok) {
        QMessageBox::information(this, "Warning","The value of Vr introduced is not valid.");
        ok=false;
    }
    else {
       node->parameters->Vr=ui->lineEdit_Vr->text().toDouble();

       QUdpSocket udpSocket4_MotherNeuron; //from mother neuron
       QHostAddress groupAddress4_to_MotherNeuron; //I'm listening to the neuron mother

       groupAddress4_to_MotherNeuron=QHostAddress(IPM_NEURON_PROMISCUOUS);
       EncodeDecodeMsg msg;
       QByteArray datagram = msg.encondeMsg(UPDATE_MODEL_VALUES_PARAMETERS_FROM_MODEL_TO_MOTHER,node->ipmSource,model,QString::number((double)node->parameters->V),QString::number((double)node->parameters->Iexc), QString::number((double)node->parameters->Iinh),QString::number(node->parameters->tau_e),QString::number(node->parameters->tau_i),QString::number(node->parameters->tau_v),QString::number(node->parameters->R),QString::number(node->parameters->Vr),QString::number(node->parameters->Vrh),QString::number(node->parameters->Vth),QString::number(node->parameters->At),QString::number(node->amountOfNeurons),node->label).toStdString().c_str();
       udpSocket4_MotherNeuron.writeDatagram(datagram, groupAddress4_to_MotherNeuron, NEURON_PROMISCUOS_PORT);
    }
}

void ModelAdExLIF::on_pushButton_16_clicked()
{
    QString model= MODEL_ADEXLIF;
    bool ok=false;
    double valueDouble;

    valueDouble=ui->lineEdit_Vrh->text().toDouble(&ok);
    if (!ok) {
        QMessageBox::information(this, "Warning","The value of Vrh introduced is not valid.");
        ok=false;
    }
    else {
       node->parameters->Vrh=ui->lineEdit_Vrh->text().toDouble();

       QUdpSocket udpSocket4_MotherNeuron; //from mother neuron
       QHostAddress groupAddress4_to_MotherNeuron; //I'm listening to the neuron mother

       groupAddress4_to_MotherNeuron=QHostAddress(IPM_NEURON_PROMISCUOUS);
       EncodeDecodeMsg msg;
       QByteArray datagram = msg.encondeMsg(UPDATE_MODEL_VALUES_PARAMETERS_FROM_MODEL_TO_MOTHER,node->ipmSource,model,QString::number((double)node->parameters->V),QString::number((double)node->parameters->Iexc), QString::number((double)node->parameters->Iinh),QString::number(node->parameters->tau_e),QString::number(node->parameters->tau_i),QString::number(node->parameters->tau_v),QString::number(node->parameters->R),QString::number(node->parameters->Vr),QString::number(node->parameters->Vrh),QString::number(node->parameters->Vth),QString::number(node->parameters->At),QString::number(node->amountOfNeurons),node->label).toStdString().c_str();
       udpSocket4_MotherNeuron.writeDatagram(datagram, groupAddress4_to_MotherNeuron, NEURON_PROMISCUOS_PORT);
    }
}

void ModelAdExLIF::on_pushButton_17_clicked()
{
    QString model= MODEL_ADEXLIF;
    bool ok=false;
    double valueDouble;

    valueDouble=ui->lineEdit_Vth->text().toDouble(&ok);
    if (!ok) {
        QMessageBox::information(this, "Warning","The value of Vth introduced is not valid.");
        ok=false;
    }
    else {
       node->parameters->Vth=ui->lineEdit_Vth->text().toDouble();

       QUdpSocket udpSocket4_MotherNeuron; //from mother neuron
       QHostAddress groupAddress4_to_MotherNeuron; //I'm listening to the neuron mother

       groupAddress4_to_MotherNeuron=QHostAddress(IPM_NEURON_PROMISCUOUS);
       EncodeDecodeMsg msg;
       QByteArray datagram = msg.encondeMsg(UPDATE_MODEL_VALUES_PARAMETERS_FROM_MODEL_TO_MOTHER,node->ipmSource,model,QString::number((double)node->parameters->V),QString::number((double)node->parameters->Iexc), QString::number((double)node->parameters->Iinh),QString::number(node->parameters->tau_e),QString::number(node->parameters->tau_i),QString::number(node->parameters->tau_v),QString::number(node->parameters->R),QString::number(node->parameters->Vr),QString::number(node->parameters->Vrh),QString::number(node->parameters->Vth),QString::number(node->parameters->At),QString::number(node->amountOfNeurons),node->label).toStdString().c_str();
       udpSocket4_MotherNeuron.writeDatagram(datagram, groupAddress4_to_MotherNeuron, NEURON_PROMISCUOS_PORT);
    }
}

void ModelAdExLIF::on_pushButton_18_clicked()
{
    QString model= MODEL_ADEXLIF;
    bool ok=false;
    double valueDouble;

    valueDouble=ui->lineEdit_At->text().toDouble(&ok);
    if (!ok) {
        QMessageBox::information(this, "Warning","The value of At introduced is not valid.");
        ok=false;
    }
    else {
       node->parameters->At=ui->lineEdit_At->text().toDouble();

       QUdpSocket udpSocket4_MotherNeuron; //from mother neuron
       QHostAddress groupAddress4_to_MotherNeuron; //I'm listening to the neuron mother

       groupAddress4_to_MotherNeuron=QHostAddress(IPM_NEURON_PROMISCUOUS);
       EncodeDecodeMsg msg;
       QByteArray datagram = msg.encondeMsg(UPDATE_MODEL_VALUES_PARAMETERS_FROM_MODEL_TO_MOTHER,node->ipmSource,model,QString::number((double)node->parameters->V),QString::number((double)node->parameters->Iexc), QString::number((double)node->parameters->Iinh),QString::number(node->parameters->tau_e),QString::number(node->parameters->tau_i),QString::number(node->parameters->tau_v),QString::number(node->parameters->R),QString::number(node->parameters->Vr),QString::number(node->parameters->Vrh),QString::number(node->parameters->Vth),QString::number(node->parameters->At),QString::number(node->amountOfNeurons),node->label).toStdString().c_str();
       udpSocket4_MotherNeuron.writeDatagram(datagram, groupAddress4_to_MotherNeuron, NEURON_PROMISCUOS_PORT);
    }
}

void ModelAdExLIF::on_pushButton_Exit_clicked()
{
    close();
}

void ModelAdExLIF::on_pushButton_applyData_clicked()
{
    QString model= MODEL_ADEXLIF;
    if (parmatersOK()) {
        QMessageBox::StandardButton reply;
        QString msg="Are you sure you want to apply all these changes to the neuron?";
        reply=QMessageBox::question(this, "Atención",msg,QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        if (reply==QMessageBox::Yes) {
            node->label= ui->lineEdit_Label->text();
            //node->ip=ui->lineEdit_Ip->text
            node->parameters->V=ui->lineEdit_V->text().toDouble();
            node->parameters->Iexc=ui->lineEdit_Iexc->text().toDouble();
            node->parameters->Iinh=ui->lineEdit_Iinh->text().toDouble();
            node->parameters->tau_e=ui->lineEdit_tauExc->text().toDouble();
            node->parameters->tau_i=ui->lineEdit_tauInh->text().toDouble();
            node->parameters->tau_v=ui->lineEdit_tauV->text().toDouble();
            node->parameters->R=ui->lineEdit_R->text().toDouble();
            node->parameters->Vr=ui->lineEdit_Vr->text().toDouble();
            node->parameters->Vrh=ui->lineEdit_Vrh->text().toDouble();
            node->parameters->Vth=ui->lineEdit_Vth->text().toDouble();
            node->parameters->At=ui->lineEdit_At->text().toDouble();
            node->amountOfNeurons=ui->lineEdit_Amount->text().toInt();
            node->hide();
            node->show();

            QUdpSocket udpSocket4_MotherNeuron; //from mother neuron
            QHostAddress groupAddress4_to_MotherNeuron; //I'm listening to the neuron mother

            groupAddress4_to_MotherNeuron=QHostAddress(IPM_NEURON_PROMISCUOUS);
            EncodeDecodeMsg msg;
            QByteArray datagram = msg.encondeMsg(UPDATE_MODEL_VALUES_PARAMETERS_FROM_MODEL_TO_MOTHER,node->ipmSource,model,QString::number((double)node->parameters->V),QString::number((double)node->parameters->Iexc), QString::number((double)node->parameters->Iinh),QString::number(node->parameters->tau_e),QString::number(node->parameters->tau_i),QString::number(node->parameters->tau_v),QString::number(node->parameters->R),QString::number(node->parameters->Vr),QString::number(node->parameters->Vrh),QString::number(node->parameters->Vth),QString::number(node->parameters->At),QString::number(node->amountOfNeurons),node->label).toStdString().c_str();
            udpSocket4_MotherNeuron.writeDatagram(datagram, groupAddress4_to_MotherNeuron, NEURON_PROMISCUOS_PORT);
         }
     }
}

void ModelAdExLIF::on_pushButton_ApplySynapse_clicked()
{
    bool ok;
    ui->lineEdit->text().toDouble(&ok);
    QTableWidgetItem *itemid = ui->tableWidget->item(ui->tableWidget->currentRow(), 0);
    QTableWidgetItem *target = ui->tableWidget->item(ui->tableWidget->currentRow(), 1);
    QTableWidgetItem *itemTypeSynapse = ui->tableWidget->item(ui->tableWidget->currentRow(), 2);
    QTableWidgetItem *itemValue = ui->tableWidget->item(ui->tableWidget->currentRow(), 3);
    QTableWidgetItem *itemFx = ui->tableWidget->item(ui->tableWidget->currentRow(), 4);

    if (!ok)
        QMessageBox::information(this, "Warning"," The value introduced for the synapse is not valid.");
    else if (!itemValue)
        QMessageBox::information(this, "Warning","You must select synapse.");
    else {
        ui->tableWidget->setItem(ui->tableWidget->currentRow(), 1, itemTypeSynapse);
        itemTypeSynapse->setText(ui->comboBox_2->currentText());
        ui->tableWidget->setItem(ui->tableWidget->currentRow(), 2, itemValue);
        itemValue->setText(ui->lineEdit->text());
        ui->tableWidget->setItem(ui->tableWidget->currentRow(), 3, itemFx);
        itemFx->setText(ui->comboBox->currentText());

        int id=itemid->text().toInt();
        QString fx_numberTxt=fxI.find(itemFx->text()).value();
        QString fx_unitMeasureTxt=fxI.find(itemFx->text()).key();
        double w=itemValue->text().toDouble();
        QString typeSynapse=itemTypeSynapse->text();
        bool encontrado=true;
        int idx=0;

        if (encontrado) {
            int type=TYPE_SYP_EXCITATION;
            if (typeSynapse=="Inh")
                type=TYPE_SYP_INHIBITION;
// LOS DATOS DE Vsynapse SON ACTUALIZADOS POR UN MENSAJE ENTRANTE DESDE MOTHER A NEURON

            QUdpSocket udpSocket4_MotherNeuron; //from mother neuron
            QHostAddress groupAddress4_to_MotherNeuron; //I'm listening to the neuron mother
            EncodeDecodeMsg msg;

            groupAddress4_to_MotherNeuron=QHostAddress(IPM_MOTHER);

            QByteArray datagram = msg.encondeMsg(UPDATE_VALUES_SYNAPSE_FROM_NEURONWIDGET_TO_MOTHER,node->ipmSource,QString::number(id),QString::number(w),fx_numberTxt,fx_unitMeasureTxt,QString::number(type)).toStdString().c_str();
            udpSocket4_MotherNeuron.writeDatagram(datagram, groupAddress4_to_MotherNeuron, NEURON_PROMISCUOS_PORT);

            int pos=ui->comboBox->findText(fx_unitMeasureTxt,Qt::MatchCaseSensitive);
            ui->comboBox->setCurrentIndex(pos);
        }
    }
}

void ModelAdExLIF::on_pushButton_Refresh_clicked()
{
    showSynapsys();
}
