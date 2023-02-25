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
    schemeNeuron = new NeuronScheme(nullptr);
    motherNeuron = new MotherNeuron(nullptr, schemeNeuron);

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
