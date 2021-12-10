#include "mainScreen.h"
#include "ui_mainwindow.h"
#include "Widget.h"

#include "motherNeuron.h"
#include "synapse.h"
#include <iostream>
#include "Widget.h"
#include "encodeDecodeMsg.h"
#include "config.h"
#include "about.h"

using namespace std;

// Container for storage the current Units
typeFxI fxI;

MainScreen::MainScreen(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    fxI.insert("MA","1E6");
    fxI.insert("kA","1E3");
    fxI.insert("A","1");
    fxI.insert("mA","1E-3");
    fxI.insert("uA","1E-6");
    fxI.insert("nA","1E-9");
    fxI.insert("pA","1E-12");


    spikeGenerator=nullptr;
    schemeNeuron = new MainGraphics(nullptr);
    motherNeuron = new MotherNeuron(nullptr,IPM_MOTHER,schemeNeuron);

    ui->label_4->setText("Neural Scheme");
    ui->label_5->setText("General Monitor");

    QPixmap pixmap_General(":graphics/generalMonitorMain.png");
    QIcon ButtonIconGeneral(pixmap_General);
    ui->pushButton_12->setIcon(ButtonIconGeneral);
    QSize s1(ui->pushButton_12->rect().size().width()-10,ui->pushButton_12->rect().size().height()-10);
    ui->pushButton_12->setIconSize(s1);
    ui->pushButton_12->setFixedSize(ui->pushButton_12->rect().size());


    QPixmap pixmap_Scheme(":graphics/schemeNeurons.jpg");
    QIcon ButtonIconScheme(pixmap_Scheme);
    ui->pushButton_5->setIcon(ButtonIconScheme);
    QSize s2(ui->pushButton_5->rect().size().width()-10,ui->pushButton_5->rect().size().height()-10);
    ui->pushButton_5->setIconSize(s2);
    ui->pushButton_5->setFixedSize(ui->pushButton_5->rect().size());

    QPixmap pixmap_About(":graphics/about3.jpg");
    QIcon ButtonIconAbout(pixmap_About);
    ui->pushButton_14->setIcon(ButtonIconAbout);
    QSize s3(ui->pushButton_14->rect().size().width()-10,ui->pushButton_14->rect().size().height()-10);
    ui->pushButton_14->setIconSize(s3);
    ui->pushButton_14->setFixedSize(ui->pushButton_14->rect().size());

    QPixmap pixmap_Exit(":graphics/exit.png");
    QIcon ButtonIconExit(pixmap_Exit);
    ui->pushButton_13->setIcon(ButtonIconExit);
    QSize s4(ui->pushButton_13->rect().size().width()-10,ui->pushButton_13->rect().size().height()-10);
    ui->pushButton_13->setIconSize(s4);
    ui->pushButton_13->setFixedSize(ui->pushButton_13->rect().size());
    //ui->pushButton_5->setFlat(true);


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


    ui->pushButton_5->setGraphicsEffect(effect0);
    ui->pushButton_12->setGraphicsEffect(effect1);
    ui->pushButton_14->setGraphicsEffect(effect2);
    ui->pushButton_13->setGraphicsEffect(effect3);

    QGraphicsDropShadowEffect* effect4 = new QGraphicsDropShadowEffect();
    effect4->setBlurRadius(100);
    effect4->setOffset(3,3);
    this->setGraphicsEffect(effect4);
    ui->label_2->setStyleSheet("QLabel {background-color: transparent; color: white;}");
    ui->label_3->setStyleSheet("QLabel {background-color: transparent; color: white;}");
    ui->label->setStyleSheet("QLabel {background-color: transparent; color: white;}");
    //setWindowFlags(Qt::FramelessWindowHint);
}

void MainScreen::keyPressEvent(QKeyEvent *event){
    switch(event->key()) {
       case Qt::Key_Escape: // if ESCAPE key is pressed, the window is closed
          close();
          break;
    }
}
void MainScreen::showEvent(QShowEvent *) {

      QGraphicsScene *sceneTmp = new QGraphicsScene();
      QPixmap p0(":graphics/neuron_background.jpg");
      sceneTmp->addPixmap(p0);

      ui->graphicsView->setStyleSheet("background: transparent");
      ui->graphicsView->setScene(sceneTmp);
      ui->graphicsView->fitInView(sceneTmp->itemsBoundingRect() ,Qt::IgnoreAspectRatio);

     //Para que la ventana no se maximice
     setFixedSize(width(),height());


     QGraphicsScene *sceneTmp1 = new QGraphicsScene();
     QPixmap p1(":graphics/logoUCA1.png");
     sceneTmp1->addPixmap(p1);
     ui->graphicsView_2->setStyleSheet("background: transparent");
     ui->graphicsView_2->setScene(sceneTmp1);
     ui->graphicsView_2->fitInView(sceneTmp1->itemsBoundingRect() ,Qt::IgnoreAspectRatio);

     this->move(QApplication::desktop()->availableGeometry().center() - this->rect().center());
}

void MainScreen::closeEvent(QCloseEvent *event){

    QMessageBox::StandardButton reply;
    QString msg="Are you sure you want to quit?";

    reply=QMessageBox::question(this, "Warning",msg,QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    if (reply==QMessageBox::Yes) {
       event->accept();
       schemeNeuron->removeALLScenary(); //Matamos a todas la neuronas remotas y locales
       schemeNeuron->close();
       delete schemeNeuron;
       QApplication::closeAllWindows();
    }
    else
       event->ignore();
}

MainScreen::~MainScreen()
{
    delete ui;
}

void MainScreen::on_pushButton_5_clicked()
{
    schemeNeuron->show();
    schemeNeuron->activateWindow();
}

void MainScreen::on_pushButton_12_clicked()
{
    WidgetGeneralMonitor *generalMonitor = new WidgetGeneralMonitor(nullptr,&motherNeuron->dataNeurons);
    generalMonitor->show();
}


void MainScreen::on_pushButton_13_clicked()
{
  close();
}


void MainScreen::on_pushButton_14_clicked()
{
    About *about = new About();
    about->setWindowModality(Qt::ApplicationModal);
    about->show();
}
