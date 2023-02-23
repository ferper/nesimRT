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
    motherNeuron = new MotherNeuron(nullptr, schemeNeuron);

    QPixmap pixmap_General(":graphics/generalMonitorMain.png");
    QIcon ButtonIconGeneral(pixmap_General);
    ui->bt_general_monitor->setIcon(ButtonIconGeneral);
    QSize s1(ui->bt_general_monitor->rect().size().width()-10,ui->bt_general_monitor->rect().size().height()-10);
    ui->bt_general_monitor->setIconSize(s1);
    ui->bt_general_monitor->setFixedSize(ui->bt_general_monitor->rect().size());

    QPixmap pixmap_Scheme(":graphics/schemeNeurons.jpg");
    QIcon ButtonIconScheme(pixmap_Scheme);
    ui->bt_neural_scheme->setIcon(ButtonIconScheme);
    QSize s2(ui->bt_neural_scheme->rect().size().width()-10,ui->bt_neural_scheme->rect().size().height()-10);
    ui->bt_neural_scheme->setIconSize(s2);
    ui->bt_neural_scheme->setFixedSize(ui->bt_neural_scheme->rect().size());

    QPixmap pixmap_About(":graphics/about3.jpg");
    QIcon ButtonIconAbout(pixmap_About);
    ui->bt_about->setIcon(ButtonIconAbout);
    QSize s3(ui->bt_about->rect().size().width()-10,ui->bt_about->rect().size().height()-10);
    ui->bt_about->setIconSize(s3);
    ui->bt_about->setFixedSize(ui->bt_about->rect().size());

    QPixmap pixmap_Exit(":graphics/exit.png");
    QIcon ButtonIconExit(pixmap_Exit);
    ui->bt_exit->setIcon(ButtonIconExit);
    QSize s4(ui->bt_exit->rect().size().width()-10,ui->bt_exit->rect().size().height()-10);
    ui->bt_exit->setIconSize(s4);
    ui->bt_exit->setFixedSize(ui->bt_exit->rect().size());

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


    ui->bt_neural_scheme->setGraphicsEffect(effect0);
    ui->bt_general_monitor->setGraphicsEffect(effect1);
    ui->bt_about->setGraphicsEffect(effect2);
    ui->bt_exit->setGraphicsEffect(effect3);

    QGraphicsDropShadowEffect* effect4 = new QGraphicsDropShadowEffect();
    effect4->setBlurRadius(100);
    effect4->setOffset(3,3);
    this->setGraphicsEffect(effect4);

    ui->lb_name->setStyleSheet("QLabel {background-color: transparent; color: white;}");
    ui->lb_title->setStyleSheet("QLabel {background-color: transparent; color: white;}");

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

    QGraphicsScene *sceneTmp1 = new QGraphicsScene();
    QPixmap p1(":graphics/logoUCA1.png");
    sceneTmp1->addPixmap(p1);

    ui->gv_logo->setStyleSheet("background: transparent");
    ui->gv_logo->setScene(sceneTmp1);
    ui->gv_logo->fitInView(sceneTmp1->itemsBoundingRect() ,Qt::IgnoreAspectRatio);

    this->move(QApplication::desktop()->availableGeometry().center() - this->rect().center());
    setFixedSize(width(),height());
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
       delete motherNeuron;
       QApplication::closeAllWindows();
    }
    else
       event->ignore();
}

MainScreen::~MainScreen()
{
    delete ui;
}

void MainScreen::on_bt_neural_scheme_clicked()
{
    schemeNeuron->show();
    schemeNeuron->activateWindow();
}

void MainScreen::on_bt_general_monitor_clicked()
{
    WidgetGeneralMonitor *generalMonitor = new WidgetGeneralMonitor(nullptr,&motherNeuron->dataNeurons);
    generalMonitor->show();
}


void MainScreen::on_bt_exit_clicked()
{
  close();
}


void MainScreen::on_bt_about_clicked()
{
    About *about = new About();
    about->setWindowModality(Qt::ApplicationModal);
    about->show();
}
