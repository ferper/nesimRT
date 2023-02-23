#include "newneuronGraphic.h"
#include "ui_newneuronform.h"
#include <QDialog>
#include <QMessageBox>
#include <iostream>

NewNeuronGraphic::NewNeuronGraphic(QWidget *parent,MainGraphics *graphWidget, QGraphicsScene *scene, int *idGlobalNeuron, QList <Neuron *> *localNeurons, QList <Node*> *vectorGraphicsNodes, QString mathematicalModel,bool *sceneBeSaved):
    QWidget(parent), ui(new Ui::NewNeuronForm) {

    ui->setupUi(this);
    this->idGlobalNeuron=idGlobalNeuron;
    this->mathematicalModel=mathematicalModel;
    this->vectorGraphicsNodes=vectorGraphicsNodes;
    this->localNeurons=localNeurons;
    this->graphWidget=graphWidget;
    this->scene=scene;
    this->sceneBeSaved=sceneBeSaved;
    ui->label_17->setText(mathematicalModel);
    ui->lineEdit_Label->setText(QString("G")+QString::number(graphWidget->getNumberItemOf(TYPENEURON_NORMAL)));

    float tmpX= QRandomGenerator::global()->bounded(800)+250;
    float tmpY= QRandomGenerator::global()->bounded(450)+100;


    ui->lineEdit_PosX->setText(QString::number(tmpX));
    ui->lineEdit_PosY->setText(QString::number(tmpY));
    ui->lineEdit_Ip->setText(graphWidget->generateIPSimulated(QString::number(TYPENEURON_NORMAL)));
    ui->lineEdit_V->setText("0");
    ui->lineEdit_Iexc->setText("0");
    ui->lineEdit_Iinh->setText("0");
    ui->lineEdit_tauExc->setText("20");
    ui->lineEdit_tauInh->setText("40");
    ui->lineEdit_tauV->setText("50");
    ui->lineEdit_R->setText("32000000");
    ui->lineEdit_Vr->setText("0");
    ui->lineEdit_Vrh->setText("0.005");
    ui->lineEdit_Vth->setText("0.015");
    ui->lineEdit_At->setText("0.0015");
    ui->lineEdit_Amount->setText("1");
    ui->lineEdit_a->setText("0.02");
    ui->lineEdit_b->setText("0.2");
    ui->lineEdit_d->setText("8");

    ui->comboBox->addItem("Adexlif");
    ui->comboBox->addItem("Cubalif");
    ui->comboBox->addItem("Izhikevich");
    ui->label_a->hide();
    ui->label_b->hide();
    ui->label_d->hide();
    ui->lineEdit_a->hide();
    ui->lineEdit_b->hide();
    ui->lineEdit_d->hide();


    QGraphicsScene *sceneTmp = new QGraphicsScene();
    QPixmap p0(":graphics/neuron.png");

    sceneTmp->addPixmap(p0);
    ui->graphicsView->setScene(sceneTmp);
    sceneTmp = new QGraphicsScene();
    QPixmap p1(":graphics/equation_ADEXLIF.png");
    sceneTmp->addPixmap(p1);

    ui->graphicsView_2->setScene(sceneTmp);

    this->setWindowTitle("Create Neuron");
    ui->lineEdit_Label->setFocus();

    QPixmap pixmap_ok(":graphics/createNeuron.png");
    QIcon ButtonIcon(pixmap_ok);
    ui->pushButton->setIcon(ButtonIcon);
    QSize s0(ui->pushButton->rect().size().width()-10,ui->pushButton->rect().size().height()-10);
    ui->pushButton->setIconSize(s0);
    ui->pushButton->setFixedSize(ui->pushButton->rect().size());
    ui->pushButton->setFlat(true);

    QPixmap pixmap_cancel(":graphics/cancel.png");
    QIcon ButtonIcon1(pixmap_cancel);
    ui->pushButton_2->setIcon(ButtonIcon1);
    QSize s1(ui->pushButton_2->rect().size().width()-10,ui->pushButton_2->rect().size().height()-10);
    ui->pushButton_2->setIconSize(s1);
    ui->pushButton_2->setFixedSize(ui->pushButton_2->rect().size());
    ui->pushButton_2->setFlat(true);

    QGraphicsDropShadowEffect* effect0 = new QGraphicsDropShadowEffect();
    QGraphicsDropShadowEffect* effect1 = new QGraphicsDropShadowEffect();

    effect0->setBlurRadius(100);
    effect0->setOffset(3,3);
    effect1->setBlurRadius(100);
    effect1->setOffset(3,3);

    ui->pushButton->setGraphicsEffect(effect0);
    ui->pushButton_2->setGraphicsEffect(effect1);
    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(cambioModeloSlot()));
}


//To strecht the images into the container
void NewNeuronGraphic::showEvent(QShowEvent *) {
    ui->graphicsView->fitInView(ui->graphicsView->sceneRect(),Qt::KeepAspectRatio);
    ui->graphicsView_2->fitInView(ui->graphicsView_2->sceneRect(),Qt::KeepAspectRatio);
}

void NewNeuronGraphic::keyPressEvent(QKeyEvent *event){
    switch(event->key()) {
       case Qt::Key_Escape: // Pressing ESCAPE closes the window
          graphWidget->restaurateIPSimulated(QString::number(TYPENEURON_NORMAL));
          close();
          break;
    }
}

NewNeuronGraphic::~NewNeuronGraphic()
{
    delete ui;
}

bool NewNeuronGraphic::parametersOK() {
    bool ok=true;
    bool error=false;
    int valueInt;
    double valueDouble;

    if (ui->lineEdit_Ip->text().length()){
        if (graphWidget->existIp(ui->lineEdit_Ip->text())) {
            QMessageBox::information(this, "Warning","A neuron with that IP already exists.");
            error=true;
        }
    }
    else {
        QMessageBox::information(this, "Warning","A valid IP is needed for the neuron.");
        error=true;
    }
    if (!ui->lineEdit_Label->text().length()) {
        QMessageBox::information(this, "Warning","A label is needed to identify the neuron.\nIt will be provided automatically.");
        ui->lineEdit_Label->setText(QString("G")+QString::number(vectorGraphicsNodes->size()+1));
        error=true;
    }
    if (!error) {
        valueInt=ui->lineEdit_PosX->text().toInt(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value of the X coordinate of the neuron is not valid.");
            error=true;
        }
    }
    if (!error){
         valueInt=ui->lineEdit_PosY->text().toInt(&ok);
         if (!ok) {
             QMessageBox::information(this, "Warning","The value of the Y coordinate of the neuron is not valid.");
             error=true;
         }
    }
    if (!error) {
        valueDouble=ui->lineEdit_Amount->text().toInt(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value of the Amount parameter of the neuron is not valid.");
            error=true;
        }
    }

    if (!error) {
        valueDouble=ui->lineEdit_V->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value of the V parameter of the neuron is not valid.");
            error=true;
        }
    }
    if (!error) {
        valueDouble=ui->lineEdit_Iexc->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value of the Iexc parameter of the neuron is not valid.");
            error=true;
        }
    }
    if (!error) {
        valueDouble=ui->lineEdit_Iinh->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value of the Iinh parameter of the neuron is not valid.");
            error=true;
        }
    }


    if (!error) {
        valueDouble=ui->lineEdit_Iexc->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value of the Iexc parameter of the neuron is not valid.");
            error=true;
        }
    }
    if (!error) {
        valueDouble=ui->lineEdit_tauExc->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value of the tauExc parameter of the neuron is not valid.");
            error=true;
        }
    }
    if (!error) {
        valueDouble=ui->lineEdit_tauInh->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value of the tauInh parameter of the neuron is not valid.");
            error=true;
        }
    }
    if (!error) {
        valueDouble=ui->lineEdit_tauV->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value of the tauV parameter of the neuron is not valid.");
            error=true;
        }
    }
    if (!error) {
        valueDouble=ui->lineEdit_R->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value of the R parameter of the neuron is not valid.");
            error=true;
        }
    }
    if (!error) {
        valueDouble=ui->lineEdit_Vr->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value of the Vr parameter of the neuron is not valid.");
            error=true;
        }
    }
    if (!error) {
        valueDouble=ui->lineEdit_Vrh->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value of the Vrh parameter of the neuron is not valid.");
            error=true;
        }
    }
    if (!error) {
        valueDouble=ui->lineEdit_Vth->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value of the Vth parameter of the neuron is not valid.");
            error=true;
        }
    }
    if (!error) {
        valueDouble=ui->lineEdit_At->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value of the At parameter of the neuron is not valid.");
            error=true;
        }
    }

    return !error;
}

void NewNeuronGraphic::on_pushButton_clicked()
{
    if (mathematicalModel==MODEL) {
       if (parametersOK()) {

           QString ip=ui->lineEdit_Ip->text();

           graphWidget->generateIPReal(QString::number(TYPENEURON_NORMAL));

           (*idGlobalNeuron)++;

           if( ui->comboBox->currentText() == "Adexlif" ){
               Parameters *p1 = new Parameters(0,ui->lineEdit_V->text().toDouble(),ui->lineEdit_Iexc->text().toDouble(), ui->lineEdit_Iinh->text().toDouble(),ui->lineEdit_tauExc->text().toDouble(), ui->lineEdit_tauInh->text().toDouble(),ui->lineEdit_tauV->text().toDouble(),ui->lineEdit_R->text().toDouble(), ui->lineEdit_Vr->text().toDouble(),ui->lineEdit_Vrh->text().toDouble(),ui->lineEdit_Vth->text().toDouble(), ui->lineEdit_At->text().toDouble());

               neuron_adexlif *n = new neuron_adexlif(nullptr,ui->lineEdit_Amount->text().toInt() ,ui->lineEdit_Label->text(),ui->lineEdit_PosX->text().toFloat(),ui->lineEdit_PosY->text().toFloat(), ui->lineEdit_Ip->text(),*idGlobalNeuron, TYPENEURON_NORMAL,LOCAL_NEURON, p1,1,"1E-9",ui->lineEdit_V->text().toDouble(),ui->lineEdit_Iexc->text().toDouble(),ui->lineEdit_Iinh->text().toDouble(),ui->lineEdit_V->text().toDouble()); //V_prior,IexcCurrent,IinhCurrent,VCurrent);

               localNeurons->append(dynamic_cast<Neuron*>(n));
               *sceneBeSaved=true;
               close();
               QMessageBox::information(this, "Warning","The neuron has been succesfully created.");

           }else if(ui->comboBox->currentText() == "Cubalif"){
               Parameters *p1 = new Parameters(1,ui->lineEdit_V->text().toDouble(),ui->lineEdit_Iexc->text().toDouble(), ui->lineEdit_Iinh->text().toDouble(),ui->lineEdit_tauExc->text().toDouble(), ui->lineEdit_tauInh->text().toDouble(),ui->lineEdit_tauV->text().toDouble(),ui->lineEdit_R->text().toDouble(), ui->lineEdit_Vr->text().toDouble(),ui->lineEdit_Vrh->text().toDouble(),ui->lineEdit_Vth->text().toDouble(), ui->lineEdit_At->text().toDouble());

               neuron_cubalif *n = new neuron_cubalif(nullptr,ui->lineEdit_Amount->text().toInt() ,ui->lineEdit_Label->text(),ui->lineEdit_PosX->text().toFloat(),ui->lineEdit_PosY->text().toFloat(), ui->lineEdit_Ip->text(),*idGlobalNeuron, TYPENEURON_NORMAL,LOCAL_NEURON, p1,1,"1E-9",ui->lineEdit_V->text().toDouble(),ui->lineEdit_Iexc->text().toDouble(),ui->lineEdit_Iinh->text().toDouble(),ui->lineEdit_V->text().toDouble()); //V_prior,IexcCurrent,IinhCurrent,VCurrent);

               localNeurons->append(dynamic_cast<Neuron*>(n));
               *sceneBeSaved=true;
               close();
               QMessageBox::information(this, "Warning","The neuron has been succesfully created.");
           }else if(ui->comboBox->currentText() == "Izhikevich"){
               Parameters *p1 = new Parameters(2,ui->lineEdit_V->text().toDouble(),ui->lineEdit_Iexc->text().toDouble(), ui->lineEdit_Iinh->text().toDouble(),ui->lineEdit_tauExc->text().toDouble(), ui->lineEdit_tauInh->text().toDouble(),ui->lineEdit_tauV->text().toDouble(),ui->lineEdit_R->text().toDouble(), ui->lineEdit_Vr->text().toDouble(),ui->lineEdit_Vrh->text().toDouble(),ui->lineEdit_Vth->text().toDouble(), ui->lineEdit_At->text().toDouble());

               neuron_izhikevich *n = new neuron_izhikevich(nullptr,ui->lineEdit_Amount->text().toInt() ,ui->lineEdit_Label->text(),ui->lineEdit_PosX->text().toFloat(),ui->lineEdit_PosY->text().toFloat(), ui->lineEdit_Ip->text(),*idGlobalNeuron, TYPENEURON_NORMAL,LOCAL_NEURON, p1,1,"1E-9",ui->lineEdit_V->text().toDouble(),ui->lineEdit_Iexc->text().toDouble(),ui->lineEdit_Iinh->text().toDouble(),ui->lineEdit_V->text().toDouble()); //V_prior,IexcCurrent,IinhCurrent,VCurrent);

               localNeurons->append(dynamic_cast<Neuron*>(n));
               *sceneBeSaved=true;
               close();
               QMessageBox::information(this, "Warning","The neuron has been succesfully created.");
           }

       }
   }
}
void NewNeuronGraphic::on_pushButton_2_clicked()
{
    graphWidget->restaurateIPSimulated(QString::number(TYPENEURON_NORMAL));
    close();
}

void NewNeuronGraphic::cambioModeloSlot()
{

    QGraphicsScene *sceneTmp = new QGraphicsScene();
    sceneTmp = new QGraphicsScene();

    ui->graphicsView_2->setScene(sceneTmp);

    if(ui->comboBox->currentText() == "Adexlif"){
        ui->label_17->setText("Adexlif");
        ui->label_R->show();
        ui->label_a->hide();
        ui->label_b->hide();
        ui->label_d->hide();
        ui->lineEdit_a->hide();
        ui->lineEdit_b->hide();
        ui->lineEdit_d->hide();

        ui->lineEdit_At->show();
        ui->label_At->show();
        ui->lineEdit_Vrh->show();
        ui->label_Vrh->show();
        ui->lineEdit_tauV->show();
        ui->label_tauV->show();
        ui->lineEdit_R->show();

        QPixmap p1(":graphics/equation_ADEXLIF.png");
        sceneTmp->addPixmap(p1);
    }else if(ui->comboBox->currentText() == "Cubalif"){
        ui->label_17->setText("Cubalif");

        ui->lineEdit_At->hide();
        ui->label_At->hide();
        ui->lineEdit_Vrh->hide();
        ui->label_Vrh->hide();
        ui->label_a->hide();
        ui->label_b->hide();
        ui->label_d->hide();
        ui->lineEdit_a->hide();
        ui->lineEdit_b->hide();
        ui->lineEdit_d->hide();

        ui->lineEdit_tauV->show();
        ui->label_tauV->show();
        ui->lineEdit_R->show();
        ui->label_R->show();

        QPixmap p1(":graphics/equation_CUBALIF.png");
        sceneTmp->addPixmap(p1);

    }else if(ui->comboBox->currentText() == "Izhikevich"){
        ui->label_17->setText("Izhikevich");

        ui->lineEdit_At->hide();
        ui->label_At->hide();
        ui->lineEdit_Vrh->hide();
        ui->label_Vrh->hide();
        ui->lineEdit_tauV->hide();
        ui->label_tauV->hide();
        ui->lineEdit_R->hide();
        ui->label_R->hide();

        ui->label_a->show();
        ui->label_b->show();
        ui->label_d->show();
        ui->lineEdit_a->show();
        ui->lineEdit_b->show();
        ui->lineEdit_d->show();

        QPixmap p1(":graphics/equation_IZHIKEVICH.png");
        sceneTmp->addPixmap(p1);
    }
    //QMessageBox::information(this, "Info","The model has been changed",ui->comboBox->currentText());
}

void NewNeuronGraphic::on_lineEdit_Label_returnPressed()
{
   ui->lineEdit_Ip->setFocus();
}

void NewNeuronGraphic::on_lineEdit_PosX_returnPressed()
{
    ui->lineEdit_PosY->setFocus();
}

void NewNeuronGraphic::on_lineEdit_Ip_returnPressed()
{
    ui->lineEdit_Amount->setFocus();
}

void NewNeuronGraphic::on_lineEdit_Amount_returnPressed()
{
    ui->lineEdit_PosX->setFocus();
}


void NewNeuronGraphic::on_lineEdit_Label_textEdited(const QString &arg1)
{
    ui->lineEdit_Label->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void NewNeuronGraphic::on_lineEdit_At_returnPressed()
{
    ui->pushButton->setFocus();
}

void NewNeuronGraphic::on_lineEdit_PosY_returnPressed()
{
    ui->lineEdit_V->setFocus();
}

void NewNeuronGraphic::on_lineEdit_V_returnPressed()
{
    ui->lineEdit_Iexc->setFocus();
}

void NewNeuronGraphic::on_lineEdit_Iexc_returnPressed()
{
    ui->lineEdit_Iinh->setFocus();
}

void NewNeuronGraphic::on_lineEdit_Iinh_returnPressed()
{
    ui->lineEdit_tauExc->setFocus();
}

void NewNeuronGraphic::on_lineEdit_tauExc_returnPressed()
{
    ui->lineEdit_tauInh->setFocus();
}

void NewNeuronGraphic::on_lineEdit_tauInh_returnPressed()
{
    ui->lineEdit_tauV->setFocus();
}

void NewNeuronGraphic::on_lineEdit_tauV_returnPressed()
{
    ui->lineEdit_R->setFocus();
}

void NewNeuronGraphic::on_lineEdit_R_returnPressed()
{
    ui->lineEdit_Vr->setFocus();
}

void NewNeuronGraphic::on_lineEdit_Vr_returnPressed()
{
    ui->lineEdit_Vrh->setFocus();
}

void NewNeuronGraphic::on_lineEdit_Vrh_returnPressed()
{
    ui->lineEdit_Vth->setFocus();
}

void NewNeuronGraphic::on_lineEdit_Vth_returnPressed()
{
    ui->lineEdit_At->setFocus();
}

void NewNeuronGraphic::on_lineEdit_Label_editingFinished()
{
    ui->lineEdit_Label->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void NewNeuronGraphic::on_lineEdit_Ip_textEdited(const QString &arg1)
{
    ui->lineEdit_Ip->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void NewNeuronGraphic::on_lineEdit_Ip_editingFinished()
{
    ui->lineEdit_Ip->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void NewNeuronGraphic::on_lineEdit_Amount_textEdited(const QString &arg1)
{
    ui->lineEdit_Amount->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void NewNeuronGraphic::on_lineEdit_Amount_editingFinished()
{
    ui->lineEdit_Amount->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void NewNeuronGraphic::on_lineEdit_PosX_textEdited(const QString &arg1)
{
    ui->lineEdit_PosX->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void NewNeuronGraphic::on_lineEdit_PosX_editingFinished()
{
    ui->lineEdit_PosX->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void NewNeuronGraphic::on_lineEdit_PosY_textEdited(const QString &arg1)
{
    ui->lineEdit_PosY->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void NewNeuronGraphic::on_lineEdit_PosY_editingFinished()
{
    ui->lineEdit_PosY->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void NewNeuronGraphic::on_lineEdit_V_textEdited(const QString &arg1)
{
    ui->lineEdit_V->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void NewNeuronGraphic::on_lineEdit_V_editingFinished()
{
    ui->lineEdit_V->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void NewNeuronGraphic::on_lineEdit_Iexc_textEdited(const QString &arg1)
{
    ui->lineEdit_Iexc->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void NewNeuronGraphic::on_lineEdit_Iexc_editingFinished()
{
    ui->lineEdit_Iexc->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void NewNeuronGraphic::on_lineEdit_Iinh_textEdited(const QString &arg1)
{
    ui->lineEdit_Iinh->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void NewNeuronGraphic::on_lineEdit_Iinh_editingFinished()
{
    ui->lineEdit_Iinh->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void NewNeuronGraphic::on_lineEdit_tauExc_textEdited(const QString &arg1)
{
    ui->lineEdit_tauExc->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void NewNeuronGraphic::on_lineEdit_tauExc_editingFinished()
{
    ui->lineEdit_tauExc->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void NewNeuronGraphic::on_lineEdit_tauInh_textEdited(const QString &arg1)
{
    ui->lineEdit_tauInh->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void NewNeuronGraphic::on_lineEdit_tauInh_editingFinished()
{
    ui->lineEdit_tauInh->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void NewNeuronGraphic::on_lineEdit_tauV_textEdited(const QString &arg1)
{
    ui->lineEdit_tauV->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void NewNeuronGraphic::on_lineEdit_tauV_editingFinished()
{
    ui->lineEdit_tauV->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void NewNeuronGraphic::on_lineEdit_R_textEdited(const QString &arg1)
{
    ui->lineEdit_R->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void NewNeuronGraphic::on_lineEdit_R_editingFinished()
{
    ui->lineEdit_R->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void NewNeuronGraphic::on_lineEdit_Vr_textEdited(const QString &arg1)
{
    ui->lineEdit_Vr->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void NewNeuronGraphic::on_lineEdit_Vr_editingFinished()
{
    ui->lineEdit_Vr->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void NewNeuronGraphic::on_lineEdit_Vrh_textEdited(const QString &arg1)
{
    ui->lineEdit_Vrh->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void NewNeuronGraphic::on_lineEdit_Vrh_editingFinished()
{
    ui->lineEdit_Vrh->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void NewNeuronGraphic::on_lineEdit_Vth_textEdited(const QString &arg1)
{
    ui->lineEdit_Vth->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void NewNeuronGraphic::on_lineEdit_Vth_editingFinished()
{
    ui->lineEdit_Vth->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void NewNeuronGraphic::on_lineEdit_At_textEdited(const QString &arg1)
{
    ui->lineEdit_At->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void NewNeuronGraphic::on_lineEdit_At_editingFinished()
{
    ui->lineEdit_At->setStyleSheet(EDIT_STYLE_DEFAULT);
}
