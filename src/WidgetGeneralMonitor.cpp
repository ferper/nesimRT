#include "WidgetGeneralMonitor.h"
#include "ui_generalmonitor.h"
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
#include <QMessageBox>
#include <QMutex>
#include "publicmsg.h"

WidgetGeneralMonitor::WidgetGeneralMonitor(QWidget *parent, typeDataNeurons *Qneurons):
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    this->fr=10;
    this->Qneurons=Qneurons;
    QString cadena="General Monitor: ";
    setWindowTitle(tr(cadena.toStdString().c_str()));

    //Cambiar aqui por si se quiere añadir más controles
    windowWidth=1200; //window width size
    windowHeight=700; //window height size

    rightMargen=350; //graph right margen
    bottomMargen=25; //graph bottom margen
    posCanvasX=360; //graph posX
    posCanvasY=100; //graph posY

    //Parameters to paint
    this->Iexc=0;
    this->Iinh=0;
    this->V1=0;
    this->V2=0;

    resize(windowWidth, windowHeight); //Window Size
    ui->spinSizeX->setValue(-250);
    ui->spinSizeY->setValue(100);

    QGridLayout * layout = new QGridLayout(this);
    graph = new TickerGraph(this);

    //graph Dimensions
    graph->setFixedWidth(windowWidth-rightMargen-posCanvasX);
    graph->setFixedHeight(windowHeight-bottomMargen-posCanvasY);
    graph->setRange(-250,100);
    graph->setTimespan(4);
    graph->setTickSeparation(0.5);
    graph->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    graph->setBackgroundBrush(QBrush(QColor("#ffffff")));
    graph->setBorderColor(QColor("#002d5b"));
    graph->setGridPen(QPen(QColor("#22002d5b")));
    layout->addWidget(graph);
    initializeColorSeries();
    timer.setInterval(10);
    timer.stop();

    QGraphicsScene *sceneTmp = new QGraphicsScene();
    QPixmap p0(":graphics/neuron.png");
    sceneTmp->addPixmap(p0);
    ui->graphicsView->setScene(sceneTmp);

    sceneTmp = new QGraphicsScene();
    QPixmap p1(":graphics/equation.png");
    sceneTmp->addPixmap(p1);
    ui->graphicsView_2->setScene(sceneTmp);

    QPixmap pixmap_Exit(":graphics/exit.png");
    QIcon ButtonIconExit(pixmap_Exit);
    ui->pushButton_Exit->setIcon(ButtonIconExit);
    QSize sExit(ui->pushButton_Exit->rect().size().width()-10,ui->pushButton_Exit->rect().size().height()-10);
    ui->pushButton_Exit->setIconSize(sExit);
    ui->pushButton_Exit->setFixedSize(ui->pushButton_Exit->rect().size());
    ui->pushButton_Exit->setFlat(true);

    QPixmap pixmap_Start(":graphics/play.png");
    QIcon ButtonIconStart(pixmap_Start);
    ui->pushButton_Start->setIcon(ButtonIconStart);
    QSize s6(ui->pushButton_Start->rect().size().width()-10,ui->pushButton_Start->rect().size().height()-10);
    ui->pushButton_Start->setIconSize(s6);
    ui->pushButton_Start->setFixedSize(ui->pushButton_Start->rect().size());
    ui->pushButton_Start->setFlat(true);

    QPixmap pixmap_Capture(":graphics/capture.png");
    QIcon ButtonIconCapture(pixmap_Capture);
    ui->pushButton_Capture->setIcon(ButtonIconCapture);
    QSize s7(ui->pushButton_Capture->rect().size().width()-10,ui->pushButton_Capture->rect().size().height()-10);
    ui->pushButton_Capture->setIconSize(s7);
    ui->pushButton_Capture->setFixedSize(ui->pushButton_Capture->rect().size());
    ui->pushButton_Capture->setFlat(true);

    QPixmap pixmap_autoRange(":graphics/signal.png");
    QIcon ButtonIcon_autoRange(pixmap_autoRange);
    ui->pushButton_autoRange->setIcon(ButtonIcon_autoRange);
    QSize sAuto(ui->pushButton_autoRange->rect().size().width()-10,ui->pushButton_autoRange->rect().size().height()-10);
    ui->pushButton_autoRange->setIconSize(sAuto);
    ui->pushButton_autoRange->setFixedSize(ui->pushButton_autoRange->rect().size());
    ui->pushButton_autoRange->setFlat(true);

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

    ui->pushButton_Exit->setGraphicsEffect(effect0);
    ui->pushButton_Start->setGraphicsEffect(effect1);
    ui->pushButton_autoRange->setGraphicsEffect(effect2);
    ui->pushButton_Capture->setGraphicsEffect(effect3);

    connect(&timer, SIGNAL(timeout()), this, SLOT(paintGraphic()));
    connect(ui->tableWidget, SIGNAL(cellDoubleClicked (int, int) ),this, SLOT( cellSelected( int, int ) ) );

    //-- Para recibir todas las señales
    udpSocket4_GeneralMonitor = new QUdpSocket();
    groupAddress4_GeneralMonitor = new QHostAddress(IPM_NEURON_PROMISCUOUS);

    //Escuchamos por un unico puerto todas las señales
    udpSocket4_GeneralMonitor->bind(QHostAddress::AnyIPv4,  MONITOR_PORT, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    udpSocket4_GeneralMonitor->joinMulticastGroup(*groupAddress4_GeneralMonitor);
    connect(udpSocket4_GeneralMonitor, SIGNAL(readyRead()),this, SLOT(processDataGeneralMonitor()));

    QPalette p = ui->tableWidget->palette();
    p.setColor(QPalette::Base, QColor("#f5c1ec"));
    ui->tableWidget->setPalette(p);
    ui->tableWidget->setStyleSheet("QTableWidget::item {""border: 1px solid black;""}");
    ui->label_25->setText(MODEL_ADEXLIF);

    this->move(QApplication::desktop()->availableGeometry().center() - this->rect().center());
}

WidgetGeneralMonitor::~WidgetGeneralMonitor() {
    delete ui;
}

void WidgetGeneralMonitor::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
       case Qt::Key_Escape: // if ESCAPE key is pressed, the window is closed
          close();
          break;
    }
}

void WidgetGeneralMonitor::setupGraphColor(){
    QMap <QString,QPair<QColor,QColor>>::Iterator itColor=colorsSerie.begin();
    for (;itColor!=colorsSerie.end();++itColor) {
       graph->setDataLinePen(itColor.key(),itColor.value().first);
       graph->setFillColor(itColor.key(), itColor.value().second);
    }
}

void WidgetGeneralMonitor::initializeColorSeries(){
    QString color1="#0000ff"; QString color2="#ff0000"; QString color3="#00ffff";
    QString color4="#00ff00"; QString color5="#002d5b"; QString color6="#ff00ff";

      colorsSerie.insert("serie1",qMakePair(QColor(color1),QColor("#00000000")));
      colorsSerie.insert("serie2",qMakePair(QColor(color2),QColor("#00000000")));
      colorsSerie.insert("serie3",qMakePair(QColor(color3),QColor("#00000000")));
      colorsSerie.insert("serie4",qMakePair(QColor(color4),QColor("#00000000")));
      colorsSerie.insert("serie5",qMakePair(QColor(color5),QColor("#00000000")));
      colorsSerie.insert("serie6",qMakePair(QColor(color6),QColor("#00000000")));
      ui->radioButton_s1->setStyleSheet("background-color: "+color1);
      ui->radioButton_s2->setStyleSheet("background-color: "+color2);
      ui->radioButton_s3->setStyleSheet("background-color: "+color3);
      ui->radioButton_s4->setStyleSheet("background-color: "+color4);
      ui->radioButton_s5->setStyleSheet("background-color: "+color5);
      ui->radioButton_s6->setStyleSheet("background-color: "+color6);
      setupGraphColor();
}

void WidgetGeneralMonitor::showLegend(){
    ui->tableWidget_Legend->clearContents();
    ui->tableWidget_Legend->setColumnCount(3);
    ui->tableWidget_Legend->setRowCount(0);
    QStringList labels;
    labels << tr("Neuron") << tr("Signal") << tr("Serie");
    ui->tableWidget_Legend->setColumnWidth(0,70);
    ui->tableWidget_Legend->setColumnWidth(1,30);
    ui->tableWidget_Legend->setColumnWidth(2,30);

    ui->tableWidget_Legend->setHorizontalHeaderLabels(labels);
    ui->tableWidget_Legend->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget_Legend->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget_Legend->setShowGrid(true);
    ui->tableWidget_Legend->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QMap <QString,QPair<QString,int>>::Iterator itlegend=signalsSelected.begin();

    for (;itlegend!=signalsSelected.end();++itlegend){
       QTableWidgetItem *tneuron = new QTableWidgetItem();
       QTableWidgetItem *tsignal = new QTableWidgetItem;
       QTableWidgetItem *tserie = new QTableWidgetItem;
       tneuron->setText(itlegend.value().first);
       tsignal->setText(paramsMonitor[itlegend.value().second]);
       tserie->setText(itlegend.key());
       int row = ui->tableWidget_Legend->rowCount();

       ui->tableWidget_Legend->insertRow(row);
       ui->tableWidget_Legend->setItem(row, 0,tneuron);
       ui->tableWidget_Legend->setItem(row, 1,tsignal);
       ui->tableWidget_Legend->setItem(row, 2,tserie);
       ui->tableWidget_Legend->item(row,2)->setBackground(colorsSerie.find(itlegend.key()).value().first);
    }
}

void WidgetGeneralMonitor::paintGraphic(){
   QMap<QString, double *>::const_iterator i;
   for (i = signalsMonitor.begin(); i != signalsMonitor.end(); ++i) {
       graph->appendPoint(i.key(),*(i.value()));
      std::cout<<"Paint: "<<i.key().toStdString()<<" "<<*i.value()<<std::endl;
   }
}

bool WidgetGeneralMonitor::event(QEvent *event) {
    if (event->spontaneous())
        onShow();
     return QWidget::event(event);
}

void WidgetGeneralMonitor::onShow(){
    graph->move(posCanvasX,posCanvasY); //graph position
    typeDataNeurons::const_iterator it1=Qneurons->cbegin();
    int n=0; //Amount of TYPENEURON_NORMAL
    while (it1!=Qneurons->cend()) {
        if (it1.value().typeNeuron==TYPENEURON_NORMAL)
            n++;
        ++it1;
    }
    ui->tableWidget->setRowCount(n);// Vneurons->size());
    ui->tableWidget->setColumnCount(4);
    QStringList labels;
    labels << tr("Sender") << tr("Monitor") << tr("Id")<<tr("Address");
    ui->tableWidget->setHorizontalHeaderLabels(labels);
    ui->tableWidget->setColumnWidth(0,80);
    ui->tableWidget->setColumnWidth(1,80);
    ui->tableWidget->setColumnWidth(2,50);
    ui->tableWidget->setColumnWidth(3,100);

    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    typeDataNeurons::const_iterator it=Qneurons->cbegin();
    for (int pos=0;it!=Qneurons->cend();++it) {
        if (it.value().typeNeuron==TYPENEURON_NORMAL) { //Only show a kind of NEURON
            QString id=it.value().id;
            QString ipm=it.key();
            QString sending="";

            if (it.value().sending)
                sending="*";
            ui->tableWidget->setItem(pos, 0, new QTableWidgetItem(sending));
            ui->tableWidget->setItem(pos, 2, new QTableWidgetItem(id));
            ui->tableWidget->setItem(pos, 3, new QTableWidgetItem(ipm));
            ui->tableWidget->verticalHeader()->setVisible(false);
            pos++;
        }
    }
    ui->graphicsView->fitInView(ui->graphicsView->sceneRect(),Qt::KeepAspectRatio);
    ui->graphicsView_2->fitInView(ui->graphicsView_2->sceneRect(),Qt::KeepAspectRatio);
}

void WidgetGeneralMonitor::processDataGeneralMonitor()
{
    QByteArray datagram;
    QString data="";
    int pos;

    while (udpSocket4_GeneralMonitor->hasPendingDatagrams()) {
        datagram.resize(int(udpSocket4_GeneralMonitor->pendingDatagramSize()));
        udpSocket4_GeneralMonitor->readDatagram(datagram.data(), datagram.size());
        data=datagram.constData();
        pos=data.indexOf(";");
        ipmSource=data.left(pos);
        data.remove(0,pos+1);
        pos=data.indexOf(";");
        itData=signalsReceived.begin();
        bool encontrado=false;
        while ((!encontrado) && (itData != signalsReceived.end())) {
            if (itData.key()==ipmSource)
               encontrado=true;
            else
               ++itData;
        }

        if (encontrado) {
           int idx=0;
           while (pos>=0) { //Se puede proteger el vector con idx<=paramsMonitor.size Por si se reciben más parametros que elmentos en el vector
               QString p=data.left(pos);
               *(itData.value()+idx)=p.toDouble();
               data.remove(0,pos+1);
               pos=data.indexOf(";");
               idx++;
           }
       }
    }
}

void WidgetGeneralMonitor::on_pushButton_4_clicked()
{
    QTableWidgetItem *item = ui->tableWidget->item(ui->tableWidget->currentRow(),3); //ip multicast
    if (item) { // Hay neuronas en el sistema
       QString NeuronSelected=item->text();
       std::cout<<"Row Selected: "<<ui->tableWidget->currentRow()<<" Neuron: "<<NeuronSelected.toStdString()<<std::endl;
       int pos=0; //Spike
       if (ui->radioButton_v2->isChecked())
          pos=1; // Iexe
       else if (ui->radioButton_v3->isChecked())
          pos=2; // Iinh
       else if (ui->radioButton_v4->isChecked())
          pos=3; // V
       QString serie="serie1";
       if (ui->radioButton_s2->isChecked())
          serie="serie2";
       else if (ui->radioButton_s3->isChecked())
          serie="serie3";
       else if (ui->radioButton_s4->isChecked())
          serie="serie4";
       else if (ui->radioButton_s5->isChecked())
          serie="serie5";
       else if (ui->radioButton_s6->isChecked())
          serie="serie6";

       QMap <QString, QPair <QString,int>>::iterator itSS=signalsSelected.begin();
       itSS=signalsSelected.find(serie.toStdString().c_str());
       if (itSS!=signalsSelected.end())
          std::cout<<"EXISTS key= "<<itSS.key().toStdString()<<" ip: "<<itSS.value().first.toStdString()<<" pos: "<<itSS.value().second<<std::endl;
       else
           std::cout<<"Serie does not exist "<<std::endl;
       bool serieFound=true;
       bool pairFound=false;
       if (itSS==signalsSelected.end())
          serieFound=false;
        //IMPORTANTE: NO OPTIMIZAR, NO CAMBIAR - EL ORDEN ES IMPORTANTE
        // Tenemos que buscar en la KEY y luegos en todos los values el QPair
        itSS=signalsSelected.begin();
        while ((!pairFound) && (itSS!=signalsSelected.end())) { //No existe Pair
           if ((itSS.value().first==NeuronSelected) && (itSS.value().second==pos)) {
              pairFound=true;
              std::cout<<"Found"<<std::endl;
           }
           else ++itSS;
        }
        //Añadir registro nuevo en SS,SR y SM
        if (!pairFound && !serieFound){ //NO existe serie
            std::cout<<"CASE 0 - ADDING"<<std::endl;
            double *v; //= new double[4];
            QMap <QString, double *>::Iterator it = signalsReceived.begin();
            it=signalsReceived.find(NeuronSelected);
            if (it==signalsReceived.end()) {//La ip de la neurona no está
                v= new double[4];
                signalsReceived.insert(NeuronSelected,v);
            }
            else // Ya existe - Guardamos el vector de parametros a recibir
                v=it.value();
            signalsMonitor.insert(serie,v+pos);
            signalsSelected.insert(serie,qMakePair(NeuronSelected,pos));
        }
        else if (serieFound){ //La serie existe
            QMessageBox::StandardButton reply;
            itSS=signalsSelected.find(serie);
            QString msg="1 - Serie '"+serie+"'is assigned to parameter '"+paramsMonitor[itSS.value().second]+"' of neuron '"+itSS.value().first+"' \nWould you like to assign serie '"+serie+"' to parameter '"+paramsMonitor[pos]+"' of  neuron '"+NeuronSelected+"'?";
            reply=QMessageBox::question(this, "Warning",msg,QMessageBox::Yes|QMessageBox::No);
            if (reply==QMessageBox::Yes) {
               QString serie_Anterior=itSS.key();
               QString ip_anterior=itSS.value().first;
               //int pos_anterior=itSS.value().second;

               signalsSelected.insert(serie,qMakePair(NeuronSelected,pos));
               QMap <QString, double*>::Iterator ittmp_SR;
               ittmp_SR=signalsReceived.find(NeuronSelected);
               if (ittmp_SR==signalsReceived.end()){ //No existe ip en SR
                  double *v= new double[4];
                  signalsReceived.insert(NeuronSelected,v);
                  itSS=signalsSelected.begin();
                  bool exist=false;
                  while ((!exist) && (itSS!=signalsSelected.end())){
                     if (itSS.value().first.toStdString()==ip_anterior.toStdString())
                        exist=true;
                     else
                        ++itSS;
                  }
                  if (!exist)
                     signalsReceived.remove(ip_anterior);
               }
               ittmp_SR=signalsReceived.find(NeuronSelected);
               QMap <QString, double *>::Iterator ittmp_SM;
               signalsMonitor.find(serie).value()=signalsReceived.find(NeuronSelected).value()+pos;
            }
        }
        else if (!serieFound) { //La serie no existe, pero existe un QPair
            bool encontrado=false;
            itSS=signalsSelected.begin();
            //Localizamos el QPair
            while ((!encontrado) && itSS!=signalsSelected.end()) {
                 if ((itSS.value().first==NeuronSelected) && (itSS.value().second==pos))
                  encontrado=true;
               else ++itSS;
            }
            if (encontrado) {
                QMessageBox::StandardButton reply;
                QString msg="2 - Parameter '"+paramsMonitor[pos]+"' is associated to serie '"+itSS.key()+"' of neuron '"+ NeuronSelected+"\nWould you like to update parameter '"+paramsMonitor[pos]+"' to serie '"+serie+"'?";
                reply=QMessageBox::question(this, "Warning",msg,QMessageBox::Yes|QMessageBox::No);
                if (reply==QMessageBox::Yes) {
                    QString serie_Anterior=itSS.key();
                    QPair<QString,int> tmp = signalsSelected.take(serie_Anterior);
                    signalsSelected.insert(serie,tmp);

                    double *v=signalsMonitor.take(serie_Anterior);
                    signalsMonitor.insert(serie,v);
                    graph->removeSeries(serie_Anterior);
                }
            }
        }
        ui->tableWidget->setItem(ui->tableWidget->currentRow(),1 , new QTableWidgetItem("*"));
        setupGraphColor();
        showLegend();
    } else
       QMessageBox::information(this, "Warning","A neuron has not been selected.");
}

void WidgetGeneralMonitor::on_pushButton_3_clicked() {

    QTableWidgetItem *item = ui->tableWidget_Legend->item(ui->tableWidget_Legend->currentRow(),0);
    if (item) {
        QMessageBox::StandardButton reply;
        reply=QMessageBox::question(this, "Warning","Are you sure you want to delete the monitoring of the selected signal?",QMessageBox::Yes|QMessageBox::No);
        if (reply==QMessageBox::Yes) {
            QString ip=ui->tableWidget_Legend->item(ui->tableWidget_Legend->currentRow(),0)->text();
            QString signal=ui->tableWidget_Legend->item(ui->tableWidget_Legend->currentRow(),1)->text();
            QString serie=ui->tableWidget_Legend->item(ui->tableWidget_Legend->currentRow(),2)->text();
            std::cout<<"ip: "<<ip.toStdString()<<" signal: "<<signal.toStdString()<<" Serie: "<<serie.toStdString()<<std::endl;

            signalsSelected.remove(serie);
            //Comprobamos si existe otro registro que tenga la misma ip en signalsSelected,
            //si no lo hay, entonces hay que eliminar el registro de signalsReceived para que no quede huérfano
            QMap  <QString,QPair<QString,int>>::Iterator it;
            it=signalsSelected.begin();
            bool encontrado=false;
            while ((!encontrado) && (it!=signalsSelected.end())) {
               if (it.value().first.toStdString()==ip.toStdString()) //Existe otro registro con la misma ip
                   encontrado=true;
               else
                   ++it;
            }
            signalsMonitor.remove(serie);
            if (!encontrado) //Registro en signalsReceived huérfano.
               signalsReceived.remove(ip);
            graph->removeSeries(serie);
            if (!signalsMonitor.size())
                graph->clear();
            showLegend();
        }
    }
    else
       QMessageBox::information(this, "Warning","No signal has been selected.");
}

void WidgetGeneralMonitor::on_pushButton_21_clicked() {
    if (ui->spinSizeX->isEnabled())
        ui->spinSizeX->setValue(-250);
}

void WidgetGeneralMonitor::on_pushButton_22_clicked() {
    if (ui->spinSizeY->isEnabled())
        ui->spinSizeY->setValue(100);
}

void WidgetGeneralMonitor::on_spinSizeX_valueChanged(int arg1) {
    if (this->isVisible())
       graph->setRange(ui->spinSizeX->value(),ui->spinSizeY->value());
}

void WidgetGeneralMonitor::on_spinSizeY_valueChanged(int arg1) {
    if (this->isVisible())
       graph->setRange(ui->spinSizeX->value(),ui->spinSizeY->value());
}

void WidgetGeneralMonitor::on_pushButton_Exit_clicked() {
    close();
}

void WidgetGeneralMonitor::on_pushButton_autoRange_clicked() {
    if (graph->autoRange()) {
      ui->pushButton_autoRange->setText("&Fixed\n Range");
      graph->setRange(ui->spinSizeX->value(),ui->spinSizeY->value());
      ui->spinSizeX->setEnabled(true);
      ui->spinSizeY->setEnabled(true);
      graph->setAutoRange(false);
    }
    else {
       ui->spinSizeX->setEnabled(false);
       ui->spinSizeY->setEnabled(false);
       ui->pushButton_autoRange->setText("Au&to\nRange");
       graph->setAutoRange(true);
    }
}

void WidgetGeneralMonitor::on_pushButton_Start_clicked() {
    QString image="stop.png";
    if (isDrawing) {
        ui->pushButton_Start->setText("s&top");
        isDrawing=false;
        timer.stop();
   }
   else {
       image="play.png";
       ui->pushButton_Start->setText("s&tart");
       isDrawing=true;
       timer.start(timer.interval());
   }

   QPixmap pixmap_Stop(":graphics/"+image);
   QIcon ButtonIcon1(pixmap_Stop);
   ui->pushButton_Start->setIcon(ButtonIcon1);
   QSize s6(ui->pushButton_Start->rect().size().width()-10,ui->pushButton_Start->rect().size().height()-10);
   ui->pushButton_Start->setIconSize(s6);
   ui->pushButton_Start->setFixedSize(ui->pushButton_Start->rect().size());
   ui->pushButton_Start->setFlat(true);

   if (timer.isActive()) {
        ui->pushButton_Start->setText("&Start");
        timer.stop();
   }
   else {
        ui->pushButton_Start->setText("&Stop");
        timer.start();
    }
}

void WidgetGeneralMonitor::on_pushButton_Capture_clicked()
{

    QPixmap originalPixmap;
    QScreen *screen = QGuiApplication::primaryScreen();

    if (const QWindow *window = windowHandle()) {
       screen = window->screen();
    }
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
    fileDialog.activateWindow();
    fileDialog.setOption(QFileDialog::DontUseNativeDialog,true); //Very important for refresh
    if (fileDialog.exec() != QDialog::Accepted) {
        return;
    }

    const QString fileName = fileDialog.selectedFiles().first();
    if (!originalPixmap.save(fileName)) {
           QMessageBox::warning(this, tr("Save Error"), tr("The image could not be saved to \"%1\".")
                                .arg(QDir::toNativeSeparators(fileName)));
    }
}
