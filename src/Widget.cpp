#include "Widget.h"
#include "ui_Widget.h"
#include <QVBoxLayout>
#include <QtMath>
#include <QProcess>
#include <iostream>
#include <QDateTime>
#include <QMainWindow>
#include <QApplication>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QThread>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <QtMath>
#include <QDebug>
#include "config.h"
#include <QHash>
#include <QMessageBox>
#include "messages.h"
#include "encodeDecodeMsg.h"
#include <QFileDialog>
#include <QWindow>
#include <QScreen>
#include <QImageWriter>
#include <QApplication>
#include "neuron.h"
#include <QDesktopWidget>
#include <QGraphicsDropShadowEffect>


Widget::Widget(QWidget *parent,QString *ipmSource, QString *label, bool *dataIsAvailable, long long int *muestra, double *iexcCurrent, double *iinhCurrent, double *vCurrent, Parameters *p, QVector <Synapse *> *Vsynapse, int * NumberNeuronsGroup) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->ipmSource=ipmSource;
    this->fr=10;

    QString cadena="Neuron: " + *ipmSource;
    setWindowTitle(tr(cadena.toStdString().c_str()));

    windowWidth=1180; //window width size
    windowHeight=710; //window height size

    rightMargen=320; //graph right margen
    bottomMargen=20; //graph bottom margen
    posCanvasX=220; //graph posX
    posCanvasY=300; //graph posY

    ui->label_25->setText(MODEL_ADEXLIF);
    ui->spinSampler->setValue(10);
    ui->spinBox_3->setValue(4);
    ui->spinSizeX->setValue(windowWidth);
    ui->spinSizeY->setValue(windowHeight);

    ui->spinSizeX->setValue(-250);
    ui->spinSizeY->setValue(100);

    ui->lineEdit_Group->setText(QString::number(*NumberNeuronsGroup));
    ui->SpinLabelTime->setValue(5);
    ui->pushButton->setText("s&top");
    isDrawing=true;

    //Parameters to paint
    this->Iexc=iexcCurrent;
    this->Iinh=iinhCurrent;
    this->V=vCurrent;
    this->dataIsAvailable=dataIsAvailable;
    this->muestra=muestra;
    this->p=p; //can change the neuron value
    this->Vsynapse=Vsynapse;
    resize(windowWidth, windowHeight); //Window Size
    this->NumberNeuronsGroup=NumberNeuronsGroup;
    this->label=label;
    showParameters();
    ui->lineEdit_Ip->setText(ipmSource->toStdString().c_str());


    QGridLayout * layout = new QGridLayout(this);
    graph = new TickerGraph(this);

    //graph Dimensions
    graph->setFixedWidth(windowWidth-rightMargen-posCanvasX);
    graph->setFixedHeight(windowHeight-bottomMargen-posCanvasY);
    graph->setRange(-250,100);

    graph->setTimespan(ui->spinBox_3->value());
    graph->setTickSeparation(ui->SpinLabelTime->value()/10.0);

    graph->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    graph->setBackgroundBrush(QBrush(QColor("#ffffff")));

    graph->setBorderColor(QColor("#002d5b"));
    graph->setGridPen(QPen(QColor("#22002d5b")));

    graph->setDataLinePen("Iinh", QPen(QColor("#ff0000"),2));
    graph->setFillColor("Iinh", QColor("#00000000"));

    graph->setDataLinePen("Iexc", QPen(QColor("#00ff00"),2));
    graph->setFillColor("Iexc", QColor("#00000000"));

    graph->setDataLinePen("V", QPen(QColor("#0000ff"),2));
    graph->setFillColor("V", QColor("#00000000"));

    layout->addWidget(graph);
    timer.setInterval(ui->spinSampler->value());
    timer.start();
    connect(&timer, SIGNAL(timeout()), this, SLOT(paintGraphic()));

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

   QGraphicsScene *sceneTmp = new QGraphicsScene();
   QPixmap p0(":graphics/neuron.png");
   sceneTmp->addPixmap(p0);
   ui->graphicsView->setScene(sceneTmp);
   sceneTmp = new QGraphicsScene();
   QPixmap p1(":graphics/equation.png");
   sceneTmp->addPixmap(p1);

   QPixmap pixmap_ApplyChanges(":graphics/applyChangeNeuron.gif");
   QIcon ButtonIcon4(pixmap_ApplyChanges);
   ui->pushButton_okChanges->setIcon(ButtonIcon4);
   QSize s2(ui->pushButton_okChanges->rect().size().width()-10,ui->pushButton_okChanges->rect().size().height()-10);
   ui->pushButton_okChanges->setIconSize(s2);
   ui->pushButton_okChanges->setFixedSize(ui->pushButton_okChanges->rect().size());
   ui->pushButton_okChanges->setFlat(true);

   QPixmap pixmap_ApplySynapse(":graphics/applyChangeSynapse.png");
   QIcon ButtonIcon5(pixmap_ApplySynapse);
   ui->pushButton_5->setIcon(ButtonIcon5);
   QSize s3(ui->pushButton_5->rect().size().width()-10,ui->pushButton_5->rect().size().height()-10);
   ui->pushButton_5->setIconSize(s3);
   ui->pushButton_5->setFixedSize(ui->pushButton_5->rect().size());
   ui->pushButton_5->setFlat(true);


   QPixmap pixmap_Refresh(":graphics/refresh.png");
   QIcon ButtonIcon6(pixmap_Refresh);
   ui->pushButton_3->setIcon(ButtonIcon6);
   QSize s4(ui->pushButton_3->rect().size().width()-10,ui->pushButton_3->rect().size().height()-10);
   ui->pushButton_3->setIconSize(s4);
   ui->pushButton_3->setFixedSize(ui->pushButton_3->rect().size());
   ui->pushButton_3->setFlat(true);

   QPixmap pixmap_Remove(":graphics/removeSynapse.png");
   QIcon ButtonIcon7(pixmap_Remove);
   ui->pushButton_4->setIcon(ButtonIcon7);
   QSize s5(ui->pushButton_4->rect().size().width()-10,ui->pushButton_4->rect().size().height()-10);
   ui->pushButton_4->setIconSize(s2);
   ui->pushButton_4->setFixedSize(ui->pushButton_4->rect().size());
   ui->pushButton_4->setFlat(true);


   QPixmap pixmap_Stop(":graphics/stop.png");
   QIcon ButtonIcon8(pixmap_Stop);
   ui->pushButton->setIcon(ButtonIcon8);
   QSize s6(ui->pushButton->rect().size().width()-10,ui->pushButton->rect().size().height()-10);
   ui->pushButton->setIconSize(s6);
   ui->pushButton->setFixedSize(ui->pushButton->rect().size());
   ui->pushButton->setFlat(true);

   QPixmap pixmap_Capture(":graphics/capture.png");
   QIcon ButtonIcon9(pixmap_Capture);
   ui->pushButton_6->setIcon(ButtonIcon9);
   QSize s7(ui->pushButton_6->rect().size().width()-10,ui->pushButton_6->rect().size().height()-10);
   ui->pushButton_6->setIconSize(s7);
   ui->pushButton_6->setFixedSize(ui->pushButton_6->rect().size());
   ui->pushButton_6->setFlat(true);

   QPixmap pixmap_Auto(":graphics/signal.png");
   QIcon ButtonIconAuto(pixmap_Auto);
   ui->pushButton_7->setIcon(ButtonIconAuto);
   QSize sAuto(ui->pushButton_7->rect().size().width()-10,ui->pushButton_7->rect().size().height()-10);
   ui->pushButton_7->setIconSize(sAuto);
   ui->pushButton_7->setFixedSize(ui->pushButton_7->rect().size());
   ui->pushButton_7->setFlat(true);

   QPixmap pixmap_Exit(":graphics/exit.png");
   QIcon ButtonIcon_exit(pixmap_Exit);
   ui->pushButton_8->setIcon(ButtonIcon_exit);
   QSize s_exit(ui->pushButton_8->rect().size().width()-10,ui->pushButton_8->rect().size().height()-10);
   ui->pushButton_8->setIconSize(s_exit);
   ui->pushButton_8->setFixedSize(ui->pushButton_8->rect().size());
   ui->pushButton_8->setFlat(true);

   QGraphicsDropShadowEffect* effect0 = new QGraphicsDropShadowEffect();
   QGraphicsDropShadowEffect* effect1 = new QGraphicsDropShadowEffect();
   QGraphicsDropShadowEffect* effect2 = new QGraphicsDropShadowEffect();
   QGraphicsDropShadowEffect* effect3 = new QGraphicsDropShadowEffect();
   QGraphicsDropShadowEffect* effect4 = new QGraphicsDropShadowEffect();
   QGraphicsDropShadowEffect* effect5 = new QGraphicsDropShadowEffect();
   QGraphicsDropShadowEffect* effect6 = new QGraphicsDropShadowEffect();
   QGraphicsDropShadowEffect* effect7 = new QGraphicsDropShadowEffect();

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
   effect5->setBlurRadius(100);
   effect5->setOffset(3,3);
   effect6->setBlurRadius(100);
   effect6->setOffset(3,3);
   effect7->setBlurRadius(100);
   effect7->setOffset(3,3);

   ui->pushButton_okChanges->setGraphicsEffect(effect0);
   ui->pushButton_5->setGraphicsEffect(effect1);
   ui->pushButton_3->setGraphicsEffect(effect2);
   ui->pushButton_4->setGraphicsEffect(effect3);
   ui->pushButton->setGraphicsEffect(effect4);
   ui->pushButton_6->setGraphicsEffect(effect5);
   ui->pushButton_8->setGraphicsEffect(effect6);
   ui->pushButton_7->setGraphicsEffect(effect7);

   this->move(QApplication::desktop()->availableGeometry().center() - this->rect().center());

   ui->graphicsView_2->setScene(sceneTmp);

   QPalette pal = ui->tableWidget->palette();
   pal.setColor(QPalette::Base, QColor("#bbc9b6"));
   ui->tableWidget->setPalette(pal);
   ui->tableWidget->setStyleSheet("QTableWidget::item {""border: 1px solid black;""}");
   ui->label_26->setStyleSheet("background-color: rgba(0,0,0,0); color: black;");
   ui->label_27->setStyleSheet("background-color: rgba(0,0,0,0); color: black;");
   ui->lineEdit_Label->setFocus();
}

//To strecht the images into the container
void Widget::showEvent(QShowEvent *) {
    ui->graphicsView->fitInView(ui->graphicsView->sceneRect(),Qt::KeepAspectRatio);
    ui->graphicsView_2->fitInView(ui->graphicsView_2->sceneRect(),Qt::KeepAspectRatio);
    ui->lineEdit_Label->setFocus();
}

void Widget::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
       case Qt::Key_Escape: // if ESCAPE key is pressed, the window is closed
          close();
          break;
    }
}

void Widget::showParameters(){

    ui->lineEdit_Group->setText(QString::number(*NumberNeuronsGroup));
    ui->lineEdit_Label->setText(label->toStdString().c_str());
    ui->lineEdit_V->setText(QString::number((double)p->V));
    ui->lineEdit_Iexc->setText(QString::number((double)p->Iexc));
    ui->lineEdit_Iinh->setText(QString::number((double)p->Iinh));
    ui->lineEdit_tauExc->setText(QString::number(p->tau_e));
    ui->lineEdit_tauInh->setText(QString::number(p->tau_i));
    ui->lineEdit_tauV->setText(QString::number(p->tau_v));
    ui->lineEdit_R->setText(QString::number(p->R));
    ui->lineEdit_Vr->setText(QString::number(p->Vr));
    ui->lineEdit_Vrh->setText(QString::number(p->Vrh));
    ui->lineEdit_Vth->setText(QString::number(p->Vth));
    ui->lineEdit_At->setText(QString::number(p->At));
}

Widget::~Widget() {
    delete ui;
}

void Widget::cellSelected(int row, int col) {

   QTableWidgetItem *itemValue = ui->tableWidget->item(row,3);
   QTableWidgetItem *itemTypeSynapse = ui->tableWidget->item(row,2);
   QTableWidgetItem *itemUnits = ui->tableWidget->item(row,4);
   ui->lineEdit->setText(itemValue->text());
   QString unit= itemUnits->text();
   QString typeSynapse=itemTypeSynapse->text();
   ui->comboBox_2->setCurrentIndex(ui->comboBox_2->findData(typeSynapse, Qt::DisplayRole));
   ui->comboBox->setCurrentIndex(ui->comboBox->findText(unit,Qt::MatchCaseSensitive));//, Qt::DisplayRole));
}

void Widget::paintGraphic(){
    if (*dataIsAvailable) {
       graph->appendPoint("Iexc", *Iexc*1e11);
       graph->appendPoint("V", *V*50000);
       graph->appendPoint("Iinh", *Iinh*1e11);
    }
}

void Widget::show() {
   showSynapsys();
   ui->comboBox->setCurrentIndex(-1);
   ui->comboBox_2->addItem("Exc");
   ui->comboBox_2->addItem("Inh");
   ui->comboBox_2->setCurrentIndex(-1);

   QWidget::show();
}
bool Widget::event(QEvent *event) {

    if (event->spontaneous())
        onShow();

    return QWidget::event(event);
}

void Widget::onShow(){
    graph->move(posCanvasX,posCanvasY); //graph position
}

void Widget::on_spinBox_valueChanged(int arg1)
{
   if (this->isVisible()){
      timer.stop();
      timer.start(arg1);
   }
}

void Widget::on_spinBox_3_valueChanged(int arg1) {
   if (this->isVisible())
      graph->setTimespan(arg1);
}


void Widget::on_spinSizeX_valueChanged(int arg1) {
   if (this->isVisible())
      graph->setRange(ui->spinSizeX->value(),ui->spinSizeY->value());
}

void Widget::on_spinSizeY_valueChanged(int arg1) {
    if (this->isVisible())
       graph->setRange(ui->spinSizeX->value(),ui->spinSizeY->value());
}

void Widget::on_pushButton_clicked() {
    QString image="play.png";
    if (isDrawing) {
       ui->pushButton->setText("s&tart");
       isDrawing=false;
       timer.stop();
   }
   else {
       image="stop.png";
        ui->pushButton->setText("s&top");
       isDrawing=true;
       timer.start(ui->spinSampler->value());
   }

   QPixmap pixmap_Stop(":graphics/"+image);
   QIcon ButtonIcon8(pixmap_Stop);
   ui->pushButton->setIcon(ButtonIcon8);
   QSize s6(ui->pushButton->rect().size().width()-10,ui->pushButton->rect().size().height()-10);
   ui->pushButton->setIconSize(s6);
   ui->pushButton->setFixedSize(ui->pushButton->rect().size());
   ui->pushButton->setFlat(true);
}

void Widget::on_spinSampler_valueChanged(int arg1) {
    if (this->isVisible()){
       timer.stop();
       timer.start(arg1);
    }
}

void Widget::on_pushButton_2_clicked() {
    bool ok;
    ui->lineEdit_Group->text().toInt(&ok);
    if (!ok)
        QMessageBox::information(this, "Warning","The value introduced for the group of neurons is not valid.");
    else {

        QUdpSocket udpSocket4_MotherNeuron; //from mother neuron
        QHostAddress groupAddress4_to_MotherNeuron; //I'm listening to the neuron mother


        int N=ui->lineEdit_Group->text().toInt();
        *this->NumberNeuronsGroup=N;

        EncodeDecodeMsg msg;
        groupAddress4_to_MotherNeuron=QHostAddress(IPM_MOTHER);

        QByteArray datagram = msg.encondeMsg(UPDATE_NUMBER_GROUP_NEURON,*ipmSource,QString::number(N)).toStdString().c_str();
        udpSocket4_MotherNeuron.writeDatagram(datagram, groupAddress4_to_MotherNeuron, NEURON_PROMISCUOS_PORT);
    }
}

void Widget::on_SpinLabelTime_valueChanged(int arg1) {
   if (this->isVisible())
      graph->setTickSeparation(qreal(arg1/10.0));
}

void Widget::showSynapsys(){

    ui->tableWidget->clearContents();
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setRowCount(0);
    QStringList labels;
    labels << tr("id") << tr("Source") << tr("Type") << tr("Value") << tr("Unit");
    ui->tableWidget->setColumnWidth(0,30);
    ui->tableWidget->setColumnWidth(1,80);
    ui->tableWidget->setColumnWidth(2,30);
    ui->tableWidget->setColumnWidth(3,30);
    ui->tableWidget->setColumnWidth(4,30);

    ui->tableWidget->setHorizontalHeaderLabels(labels);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    for (int i=0; i<Vsynapse->size();i++) {
        if (Vsynapse->at(i)->ipmT==*ipmSource) {
            QTableWidgetItem *id = new QTableWidgetItem();
            QTableWidgetItem *source = new QTableWidgetItem();
            QTableWidgetItem *type = new QTableWidgetItem;
            QTableWidgetItem *value = new QTableWidgetItem;
            QTableWidgetItem *fx_unit = new QTableWidgetItem;
            id->setText(QString::number(Vsynapse->at(i)->idGlobalSynapse));
            source->setText(Vsynapse->at(i)->ipmS);
            if (Vsynapse->at(i)->type==TYPE_SYP_EXCITATION)
                type->setText("Exc");
            else {
                type->setText("Inh");
                id->setForeground(QColor::fromRgb(255,0,0));
                source->setForeground(id->foreground());
                type->setForeground(id->foreground());
                value->setForeground(id->foreground());
                fx_unit->setForeground(id->foreground());
            }
            value->setText(QString::number(Vsynapse->at(i)->w/Vsynapse->at(i)->fx_numberTxt.toDouble()));
            fx_unit->setText(Vsynapse->at(i)->fx_unitMeasureTxt);
            int row = ui->tableWidget->rowCount();

            ui->tableWidget->insertRow(row);
            ui->tableWidget->setItem(row, 0,id);
            ui->tableWidget->setItem(row, 1,source);
            ui->tableWidget->setItem(row, 2,type);
            ui->tableWidget->setItem(row, 3,value);
            ui->tableWidget->setItem(row, 4,fx_unit);
        }
    }
}

void Widget::on_pushButton_3_clicked() {
    showSynapsys();
}

void Widget::on_pushButton_5_clicked() {

    bool ok;
    ui->lineEdit->text().toDouble(&ok);
    QTableWidgetItem *itemid = ui->tableWidget->item(ui->tableWidget->currentRow(), 0);
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

        int id=itemid->text().toInt();
        QString fx_numberTxt=fxI.find(itemFx->text()).value();
        QString fx_unitMeasureTxt=fxI.find(itemFx->text()).key();
        double w=itemValue->text().toDouble();
        QString typeSynapse=itemTypeSynapse->text();
        bool encontrado=false;
        int idx=0;
        while ((!encontrado) && (idx<Vsynapse->size())) {
           if (Vsynapse->at(idx)->idGlobalSynapse == id)
               encontrado=true;
           else
               idx++;
        }

        if (encontrado) {
            int type=TYPE_SYP_EXCITATION;
            if (typeSynapse=="Inh")
                type=TYPE_SYP_INHIBITION;

            QUdpSocket udpSocket4_MotherNeuron; //from mother neuron
            QHostAddress groupAddress4_to_MotherNeuron; //I'm listening to the neuron mother
            EncodeDecodeMsg msg;

            groupAddress4_to_MotherNeuron=QHostAddress(IPM_NEURON_PROMISCUOUS);
            QByteArray datagram = msg.encondeMsg(UPDATE_VALUES_SYNAPSE_FROM_NEURONWIDGET_TO_MOTHER,*ipmSource,QString::number(id),QString::number(w),fx_numberTxt,fx_unitMeasureTxt,QString::number(type)).toStdString().c_str();
            udpSocket4_MotherNeuron.writeDatagram(datagram, groupAddress4_to_MotherNeuron, NEURON_PROMISCUOS_PORT);

            int pos=ui->comboBox->findText(fx_unitMeasureTxt,Qt::MatchCaseSensitive);
            ui->comboBox->setCurrentIndex(pos);
        }
    }
}

void Widget::on_pushButton_6_clicked()
{
    QPixmap originalPixmap;
    QScreen *screen = QGuiApplication::primaryScreen();

    if (const QWindow *window = windowHandle())
       screen = window->screen();
    if (!screen)
       return;
    originalPixmap = screen->grabWindow(graph->winId());// ->graph->layout()->widget()->winId());

    const QString format = "png";
    QString initialPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    if (initialPath.isEmpty())
        initialPath = QDir::currentPath();
    initialPath += tr("/untitled.") + format;

    QFileDialog fileDialog(this, tr("Save As"), initialPath);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setDirectory(initialPath);
    QStringList mimeTypes;
    foreach (const QByteArray &bf, QImageWriter::supportedMimeTypes())
           mimeTypes.append(QLatin1String(bf));
    fileDialog.setMimeTypeFilters(mimeTypes);
    fileDialog.selectMimeTypeFilter("image/" + format);
    fileDialog.setDefaultSuffix(format);
    fileDialog.setOption(QFileDialog::DontUseNativeDialog,true); //Very important for refresh

    if (fileDialog.exec() != QDialog::Accepted)
        return;

    const QString fileName = fileDialog.selectedFiles().first();

    if (!originalPixmap.save(fileName))
           QMessageBox::warning(this, tr("Save Error"), tr("The image could not be saved to \"%1\".")
                                .arg(QDir::toNativeSeparators(fileName)));
}

void Widget::on_pushButton_4_clicked()
{
    //Enviamos mensaje de eliminacion de synapse
    QTableWidgetItem *item = ui->tableWidget->item(ui->tableWidget->currentRow(),0);
    if (item) {
        int id=item->text().toInt();
        bool encontrado=false;
        int idx=0;
        ui->tableWidget->removeRow(ui->tableWidget->currentRow());

        while ((!encontrado) && (idx<Vsynapse->size())) {
           if (Vsynapse->at(idx)->idGlobalSynapse==id)
              encontrado=true;
           else
               idx++;
        }
        if (encontrado) {

            EncodeDecodeMsg msg;
            QString idGlobalSynapse= QString::number(Vsynapse->at(idx)->idGlobalSynapse);

            QString ipSource=Vsynapse->at(idx)->ipmS;
            QString ipTarget=Vsynapse->at(idx)->ipmT;
            QString m=msg.encondeMsg(REMOVE_SYNAPSE_FROM_GRAPHIC_TO_MOTHER,idGlobalSynapse,ipSource,ipTarget).toStdString().c_str();
            QUdpSocket udpSocket;
            QHostAddress groupAddress;

            groupAddress=QHostAddress(IPM_NEURON_PROMISCUOUS);
            udpSocket.setSocketOption(QAbstractSocket::MulticastTtlOption, TTL);
            QByteArray datagram = m.toStdString().c_str();
            udpSocket.writeDatagram(datagram, groupAddress, NEURON_PROMISCUOS_PORT);
        }
    }
}

void Widget::on_pushButton_8_clicked() {
    close();
}

void Widget::on_pushButton_9_clicked() {
    bool ok;
    ui->lineEdit_R->text().toDouble(&ok);
    if (!ok)
        QMessageBox::information(this, "Warning","The value introduced for the resistance of the neuron is not valid.");
    else {
        p->R=ui->lineEdit_R->text().toDouble();
    }
}

void Widget::on_pushButton_16_clicked() {
    bool ok;
    ui->lineEdit_tauExc->text().toDouble(&ok);
    if (!ok)
        QMessageBox::information(this, "Warning","The value introduced for the tau Exc parameter of the neuron is not valid.");
    else {
        p->tau_e=ui->lineEdit_tauExc->text().toDouble();
    }
}

void Widget::on_pushButton_15_clicked() {
    bool ok;
    ui->lineEdit_tauInh->text().toDouble(&ok);
    if (!ok)
        QMessageBox::information(this, "Warning","The value introduced for the tau Inh parameter of the neuron is not valid.");
    else {
        p->tau_i=ui->lineEdit_tauInh->text().toDouble();
    }
}

void Widget::on_pushButton_14_clicked() {
    bool ok;
    ui->lineEdit_tauV->text().toDouble(&ok);
    if (!ok)
        QMessageBox::information(this, "Warning","The value introduced for the tau V parameter of the neuron is not valid.");
    else {
        p->tau_v=ui->lineEdit_tauV->text().toDouble();
    }
}

void Widget::on_pushButton_10_clicked() {
    bool ok;
    ui->lineEdit_Vr->text().toDouble(&ok);
    if (!ok)
        QMessageBox::information(this, "Warning","The value introduced for the Vr parameter of the neuron is not valid.");
    else {
        p->Vr=ui->lineEdit_Vr->text().toDouble();
    }
}

void Widget::on_pushButton_11_clicked() {
    bool ok;
    ui->lineEdit_Vrh->text().toDouble(&ok);
    if (!ok)
        QMessageBox::information(this, "Warning","The value introduced for the Vrh parameter of the neuron is not valid.");
    else {
        p->Vrh=ui->lineEdit_Vrh->text().toDouble();
    }
}

void Widget::on_pushButton_12_clicked() {
    bool ok;
    ui->lineEdit_Vth->text().toDouble(&ok);
    if (!ok)
        QMessageBox::information(this, "Warning","The value introduced for the Vth parameter of the neuron is not valid.");
    else {
        p->Vth=ui->lineEdit_Vth->text().toDouble();
    }
}

void Widget::on_pushButton_13_clicked() {
    bool ok;
    ui->lineEdit_At->text().toDouble(&ok);
    if (!ok)
        QMessageBox::information(this, "Warning","The value introduced for the At parameter of the neuron is not valid.");
    else {
        p->At=ui->lineEdit_At->text().toDouble();
    }
}

bool Widget::parametersOK() {
    bool ok=true;
    bool error=false;
    int valueInt;
    double valueDouble;

    if (!ui->lineEdit_Label->text().length()) {
        QMessageBox::information(this, "Warning","A label is needed in order to identify the neuron.\nIt will be automatically providad.");
        ui->lineEdit_Label->setText(QString("G"));
        error=true;
    }
    if (!error) {
        valueInt=ui->lineEdit_Group->text().toInt(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning"," The value introduced for the group of neurons is not valid.");
            error=true;
        }
    }
    if (!error) {
        valueInt=ui->lineEdit_Iexc->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value introduced for I Exc is not valid.");
            error=true;
        }
    }
    if (!error) {
        valueInt=ui->lineEdit_Iinh->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value introduced for I Inh is not valid.");
            error=true;
        }
    }
    if (!error) {
        valueInt=ui->lineEdit_V->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value introduced for V is not valid.");
            error=true;
        }
    }
    if (!error) {
        valueDouble=ui->lineEdit_tauExc->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value introduced for Tau Exc is not valid.");
            error=true;
        }
    }
    if (!error) {
        valueDouble=ui->lineEdit_tauInh->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value introduced for Tau Inh is not valid.");
            error=true;
        }
    }
    if (!error) {
        valueDouble=ui->lineEdit_tauV->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value introduced for Tau V is not valid.");
            error=true;
        }
    }
    if (!error) {
        valueDouble=ui->lineEdit_R->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value introduced for R is not valid.");
            error=true;
        }
    }
    if (!error) {
        valueDouble=ui->lineEdit_Vr->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value introduced for Vr is not valid.");
            error=true;
        }
    }
    if (!error) {
        valueDouble=ui->lineEdit_Vrh->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value introduced for Vrh is not valid.");
            error=true;
        }
    }
    if (!error) {
        valueDouble=ui->lineEdit_Vth->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value introduced for Vth is not valid.");
            error=true;
        }
    }
    if (!error) {
        valueDouble=ui->lineEdit_At->text().toDouble(&ok);
        if (!ok) {
            QMessageBox::information(this, "Warning","The value introduced for At is not valid.");
            error=true;
        }
    }
    return !error;
}


void Widget::on_pushButton_17_clicked() {
    bool ok;
    ui->lineEdit_V->text().toDouble(&ok);
    if (!ok)
        QMessageBox::information(this, "Warning","The value introduced for V is not valid.");
    else {
        p->V=ui->lineEdit_V->text().toDouble();
    }
}

void Widget::on_lineEdit_Label_returnPressed() {
   ui->lineEdit_Group->setFocus();
}

void Widget::on_pushButton_18_clicked() {
    bool ok=false;;
    if (ui->lineEdit_Label->text().length()>0)
        ok=true;
    if (!ok)
        QMessageBox::information(this, "Warning","The value introduced for the label is not valid.");
    else {
        QString model= MODEL_ADEXLIF;
        *this->label=ui->lineEdit_Label->text();
        QUdpSocket udpSocket4_MotherNeuron; //from mother neuron
        QHostAddress groupAddress4_to_MotherNeuron; //I'm listening to the neuron mother
        groupAddress4_to_MotherNeuron=QHostAddress(IPM_NEURON_PROMISCUOUS);
        EncodeDecodeMsg msg;
        QByteArray datagram = msg.encondeMsg(UPDATE_MODEL_VALUES_PARAMETERS_FROM_WIDGET_TO_MOTHER,*ipmSource,model,QString::number((double)p->V),QString::number((double)p->Iexc), QString::number((double)p->Iinh),QString::number(p->tau_e),QString::number(p->tau_i),QString::number(p->tau_v),QString::number(p->R),QString::number(p->Vr),QString::number(p->Vrh),QString::number(p->Vth),QString::number(p->At),QString::number(*NumberNeuronsGroup),*label).toStdString().c_str();
        udpSocket4_MotherNeuron.writeDatagram(datagram, groupAddress4_to_MotherNeuron, NEURON_PROMISCUOS_PORT);
    }
}

void Widget::on_lineEdit_Group_returnPressed() {
    ui->lineEdit_V->setFocus();
}

void Widget::on_lineEdit_tauExc_returnPressed() {
    ui->lineEdit_tauInh->setFocus();
}

void Widget::on_lineEdit_tauInh_returnPressed() {
    ui->lineEdit_tauV->setFocus();
}

void Widget::on_lineEdit_tauV_returnPressed() {
    ui->lineEdit_R->setFocus();
}

void Widget::on_lineEdit_R_returnPressed() {
    ui->lineEdit_Vr->setFocus();
}

void Widget::on_lineEdit_Vr_returnPressed() {
    ui->lineEdit_Vrh->setFocus();
}

void Widget::on_lineEdit_Vrh_returnPressed() {
    ui->lineEdit_Vth->setFocus();
}

void Widget::on_lineEdit_Vth_returnPressed() {
    ui->lineEdit_At->setFocus();
}

void Widget::on_lineEdit_At_returnPressed() {
   ui->pushButton_okChanges->setFocus();
}

void Widget::on_pushButton_okChanges_clicked() {
    QMessageBox::StandardButton reply;
    QString msg="Are you sure you want to apply all these changes to the neuron?";
    reply=QMessageBox::question(this, "Atención",msg,QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    if (reply==QMessageBox::Yes) {
        QString model= MODEL_ADEXLIF;
        if (parametersOK()) {
            p->V=ui->lineEdit_V->text().toDouble();
            p->Iexc=ui->lineEdit_Iexc->text().toDouble();
            p->Iinh=ui->lineEdit_Iinh->text().toDouble();
            p->tau_e=ui->lineEdit_tauExc->text().toDouble();
            p->tau_i=ui->lineEdit_tauInh->text().toDouble();
            p->tau_v=ui->lineEdit_tauV->text().toDouble();
            p->R=ui->lineEdit_R->text().toDouble();
            p->Vr=ui->lineEdit_Vr->text().toDouble();
            p->Vrh=ui->lineEdit_Vrh->text().toDouble();
            p->Vth=ui->lineEdit_Vth->text().toDouble();
            p->At=ui->lineEdit_At->text().toDouble();
            *this->NumberNeuronsGroup=ui->lineEdit_Group->text().toInt();
            *this->label=ui->lineEdit_Label->text();
            QUdpSocket udpSocket4_MotherNeuron; //from mother neuron
            QHostAddress groupAddress4_to_MotherNeuron; //I'm listening to the neuron mother

            groupAddress4_to_MotherNeuron=QHostAddress(IPM_NEURON_PROMISCUOUS);
            EncodeDecodeMsg msg;
            QByteArray datagram = msg.encondeMsg(UPDATE_MODEL_VALUES_PARAMETERS_FROM_WIDGET_TO_MOTHER,*ipmSource,model,QString::number((double)p->V),QString::number((double)p->Iexc), QString::number((double)p->Iinh),QString::number(p->tau_e),QString::number(p->tau_i),QString::number(p->tau_v),QString::number(p->R),QString::number(p->Vr),QString::number(p->Vrh),QString::number(p->Vth),QString::number(p->At),QString::number(*NumberNeuronsGroup),*label).toStdString().c_str();
            udpSocket4_MotherNeuron.writeDatagram(datagram, groupAddress4_to_MotherNeuron, NEURON_PROMISCUOS_PORT);
        }
   }
   ui->pushButton_8->setFocus();
}

void Widget::on_lineEdit_Label_editingFinished() {
    ui->lineEdit_Label->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void Widget::on_lineEdit_Label_textEdited(const QString &arg1) {
     ui->lineEdit_Label->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void Widget::on_lineEdit_Group_textEdited(const QString &arg1) {
    ui->lineEdit_Group->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void Widget::on_lineEdit_Group_editingFinished() {
    ui->lineEdit_Group->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void Widget::on_lineEdit_tauExc_textEdited(const QString &arg1) {
    ui->lineEdit_tauExc->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void Widget::on_lineEdit_tauExc_editingFinished() {
    ui->lineEdit_tauExc->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void Widget::on_lineEdit_tauInh_textEdited(const QString &arg1) {
    ui->lineEdit_tauInh->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void Widget::on_lineEdit_tauInh_editingFinished() {
    ui->lineEdit_tauInh->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void Widget::on_lineEdit_tauV_textEdited(const QString &arg1) {
    ui->lineEdit_tauV->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void Widget::on_lineEdit_tauV_editingFinished() {
    ui->lineEdit_tauV->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void Widget::on_lineEdit_R_textEdited(const QString &arg1) {
    ui->lineEdit_R->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void Widget::on_lineEdit_R_editingFinished() {
    ui->lineEdit_R->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void Widget::on_lineEdit_Vr_textEdited(const QString &arg1) {
    ui->lineEdit_Vr->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void Widget::on_lineEdit_Vr_editingFinished() {
    ui->lineEdit_Vr->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void Widget::on_lineEdit_Vrh_textEdited(const QString &arg1) {
    ui->lineEdit_Vrh->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void Widget::on_lineEdit_Vrh_editingFinished() {
    ui->lineEdit_Vrh->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void Widget::on_lineEdit_Vth_textEdited(const QString &arg1) {
    ui->lineEdit_Vth->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void Widget::on_lineEdit_Vth_editingFinished() {
    ui->lineEdit_Vth->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void Widget::on_lineEdit_At_textEdited(const QString &arg1) {
    ui->lineEdit_At->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void Widget::on_lineEdit_At_editingFinished() {
    ui->lineEdit_At->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void Widget::on_lineEdit_textEdited(const QString &arg1) {
    ui->lineEdit->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void Widget::on_lineEdit_editingFinished() {
    ui->lineEdit->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void Widget::on_pushButton_19_clicked() {
    bool ok;
    ui->lineEdit_Iexc->text().toDouble(&ok);
    if (!ok)
        QMessageBox::information(this, "Warning","The value introduced for I Exc is not valid.");
    else {
        p->Iexc=ui->lineEdit_Iexc->text().toDouble();
    }
}

void Widget::on_pushButton_20_clicked() {
    bool ok;
    ui->lineEdit_Iinh->text().toDouble(&ok);
    if (!ok)
        QMessageBox::information(this, "Warning","The value introduced for I Inh is not valid.");
    else {
        p->Iinh=ui->lineEdit_Iinh->text().toDouble();
    }
}

void Widget::on_lineEdit_V_returnPressed() {
    ui->lineEdit_Iexc->setFocus();
}

void Widget::on_lineEdit_Iexc_returnPressed() {
    ui->lineEdit_Iinh->setFocus();
}

void Widget::on_lineEdit_Iinh_returnPressed() {
    ui->lineEdit_tauExc->setFocus();
}

void Widget::on_lineEdit_V_textEdited(const QString &arg1) {
    ui->lineEdit_V->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void Widget::on_lineEdit_V_editingFinished() {
    ui->lineEdit_V->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void Widget::on_lineEdit_Iexc_textEdited(const QString &arg1) {
    ui->lineEdit_Iexc->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void Widget::on_lineEdit_Iexc_editingFinished() {
    ui->lineEdit_Iexc->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void Widget::on_lineEdit_Iinh_textEdited(const QString &arg1) {
    ui->lineEdit_Iinh->setStyleSheet(EDIT_STYLE_MODIFIED);
}

void Widget::on_lineEdit_Iinh_editingFinished() {
    ui->lineEdit_Iinh->setStyleSheet(EDIT_STYLE_DEFAULT);
}

void Widget::on_pushButton_7_clicked() {

    if (graph->autoRange()) {
      ui->pushButton_7->setText("&Fixed\n Range");
      ui->spinSizeX->setEnabled(true);
      ui->spinSizeY->setEnabled(true);
      graph->setRange(ui->spinSizeX->value(),ui->spinSizeY->value());
      graph->setAutoRange(false);

    }
    else {
       ui->spinSizeX->setEnabled(false);
       ui->spinSizeY->setEnabled(false);
       ui->pushButton_7->setText("Au&to\nRange");
       graph->setAutoRange(true);
    }
}

void Widget::on_pushButton_21_clicked() {
   if (ui->spinSizeX->isEnabled())
      ui->spinSizeX->setValue(-250);
}

void Widget::on_pushButton_22_clicked() {
    if (ui->spinSizeY->isEnabled())
        ui->spinSizeY->setValue(100);
}
