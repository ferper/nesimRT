#include "newgeneratorGraphic.h"
#include "spikeGenerator.h"
#include "ui_newgeneratorform.h"
#include <QGraphicsDropShadowEffect>
#include <QRandomGenerator>

NewGeneratorGraphics::NewGeneratorGraphics(QWidget *parent, NeuronScheme *graphWidget,float posX, float posY, int *idGlobalNeuron, QList <Node*> *vectorGraphicsNodes, QList <Neuron *> *localNeurons,bool *sceneBeSaved) :
    QWidget(parent),
    ui(new Ui::NewGeneratorForm)
{

    ui->setupUi(this);
    this->idGlobalNeuron=idGlobalNeuron;
    this->setWindowTitle("Create Generator");
    this->vectorGraphicsNodes=vectorGraphicsNodes;
    this->graphWidget=graphWidget;
    this->sceneBeSaved=sceneBeSaved;
    this->localNeurons=localNeurons;

    ui->lineEdit_Label->setText(QString("GEN")+QString::number(graphWidget->getNumberItemOf(TYPENEURON_GENERATOR)));

    ui->lineEdit_PosX->setText(QString::number(posX));
    ui->lineEdit_PosY->setText(QString::number(posY));
    ui->lineEdit_Frecuency->setText("1000");
    ui->lineEdit_Amount->setText("1");
    ui->lineEdit_Ip->setText(graphWidget->generateIPSimulated(QString::number(TYPENEURON_GENERATOR)));

    ui->lineEdit_Ip->setFocus();


    QGraphicsScene *sceneTmp = new QGraphicsScene();
    QPixmap p0(":graphics/carefullycra.jpg");

    sceneTmp->addPixmap(p0);
    ui->graphicsView->setStyleSheet("background: transparent");
    ui->graphicsView->setScene(sceneTmp);

    QPixmap pixmap_ok(":graphics/ok.png");
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

}
// To strecht the images into the container
void NewGeneratorGraphics::showEvent(QShowEvent *) {

    ui->graphicsView->fitInView(ui->graphicsView->sceneRect(), Qt::KeepAspectRatio);
}

void NewGeneratorGraphics::keyPressEvent(QKeyEvent *event){
    switch(event->key()) {
       case Qt::Key_Escape: // Pressing ESCAPE closes the window
          graphWidget->restaurateIPSimulated(QString::number(TYPENEURON_GENERATOR));
          close();
          break;
    }
}

NewGeneratorGraphics::~NewGeneratorGraphics()
{
    delete ui;
}

bool NewGeneratorGraphics::parametersOK() {
    bool ok=true;
    bool error=false;
    int valueInt;

    if (ui->lineEdit_Ip->text().length()){
        if (graphWidget->existIp(ui->lineEdit_Ip->text())) {
            QMessageBox::information(this, "Warning","A generator with that IP already exists.");
            error=true;
        }
    }
    else {
        QMessageBox::information(this, "Warning","A valid IP is needed for the generator.");
        error=true;
    }

    if (!error) {
        valueInt=ui->lineEdit_PosX->text().toInt(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value introduced for the X coordinate of the generator is not valid.");
            error=true;
        }
    }
    if (!error) {
        valueInt=ui->lineEdit_PosY->text().toInt(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value introduced for the Y coordinate of the generator is not valid.");
            error=true;
        }
    }

    if (!error) {
        valueInt=ui->lineEdit_Frecuency->text().toInt(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value introduced for the firing rate of the generator is not valid.");
            error=true;
        }
    }
    if (!error) {
        valueInt=ui->lineEdit_Amount->text().toInt(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value introduced for the number of generators is not valid.");
            error=true;
        }
    }
    return !error;
}

void NewGeneratorGraphics::on_pushButton_clicked()
{
    if (parametersOK()) {
        int posX= QRandomGenerator::global()->bounded(800)+250;
        int posY= QRandomGenerator::global()->bounded(450)+100;
        // Actual and simulated DHCP settings are updated
        graphWidget->generateIPReal(QString::number(TYPENEURON_GENERATOR));

        (*idGlobalNeuron)++;
        Parameters *p1 = new Parameters();//ui->lineEdit_V->text().toDouble(),ui->lineEdit_Iexc->text().toDouble(), ui->lineEdit_Iinh->text().toDouble(),ui->lineEdit_tauExc->text().toDouble(), ui->lineEdit_tauInh->text().toDouble(),ui->lineEdit_tauV->text().toDouble(),ui->lineEdit_R->text().toDouble(), ui->lineEdit_Vr->text().toDouble(),ui->lineEdit_Vrh->text().toDouble(),ui->lineEdit_Vth->text().toDouble(), ui->lineEdit_At->text().toDouble());
        Neuron *g = new Neuron(nullptr,1,ui->lineEdit_Label->text(),posX,posY, ui->lineEdit_Ip->text(),*idGlobalNeuron, TYPENEURON_GENERATOR,LOCAL_NEURON, p1,1,"1E-9");
        SpikeGenerator *spikeGenerator = new SpikeGenerator(nullptr,ui->lineEdit_Frecuency->text().toInt(),ui->lineEdit_Ip->text());
        g->FormDialog=spikeGenerator;
        localNeurons->append(g);

        *sceneBeSaved=true;
        close();
        QMessageBox::information(this, "Warning","The generator has been created correctly.");
     }
}

void NewGeneratorGraphics::on_lineEdit_Ip_textEdited(const QString &arg1)
{
    ui->lineEdit_Ip->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void NewGeneratorGraphics::on_lineEdit_Ip_editingFinished()
{
    ui->lineEdit_Ip->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void NewGeneratorGraphics::on_lineEdit_Label_textEdited(const QString &arg1)
{
    ui->lineEdit_Label->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void NewGeneratorGraphics::on_lineEdit_Label_editingFinished()
{
    ui->lineEdit_Label->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void NewGeneratorGraphics::on_lineEdit_PosX_textEdited(const QString &arg1)
{
    ui->lineEdit_PosX->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void NewGeneratorGraphics::on_lineEdit_PosX_editingFinished()
{
    ui->lineEdit_PosX->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void NewGeneratorGraphics::on_lineEdit_PosY_textEdited(const QString &arg1)
{
    ui->lineEdit_PosY->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void NewGeneratorGraphics::on_lineEdit_PosY_editingFinished()
{
    ui->lineEdit_PosY->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void NewGeneratorGraphics::on_lineEdit_Frecuency_textEdited(const QString &arg1)
{
    ui->lineEdit_Frecuency->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void NewGeneratorGraphics::on_lineEdit_Frecuency_editingFinished()
{
    ui->lineEdit_Frecuency->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void NewGeneratorGraphics::on_lineEdit_Amount_textEdited(const QString &arg1)
{
    ui->lineEdit_Amount->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void NewGeneratorGraphics::on_lineEdit_Amount_editingFinished()
{
    ui->lineEdit_Amount->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void NewGeneratorGraphics::on_lineEdit_Ip_returnPressed()
{
    ui->lineEdit_Label->setFocus();
}

void NewGeneratorGraphics::on_lineEdit_Label_returnPressed()
{
    ui->lineEdit_PosX->setFocus();
}

void NewGeneratorGraphics::on_lineEdit_PosX_returnPressed()
{
    ui->lineEdit_PosY->setFocus();
}

void NewGeneratorGraphics::on_lineEdit_PosY_returnPressed()
{
    ui->lineEdit_Frecuency->setFocus();
}

void NewGeneratorGraphics::on_lineEdit_Frecuency_returnPressed()
{
    ui->lineEdit_Amount->setFocus();
}

void NewGeneratorGraphics::on_lineEdit_Amount_returnPressed()
{
    ui->pushButton->setFocus();
}

void NewGeneratorGraphics::on_pushButton_2_clicked()
{
    graphWidget->restaurateIPSimulated(QString::number(TYPENEURON_GENERATOR));
    close();
}
