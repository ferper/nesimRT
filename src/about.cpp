#include "mainScreen.h"
#include "about.h"
#include "ui_Formabout.h"

About::About(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

    setWindowTitle(tr("About us..."));
    ui->label_version->setText(QString::number(VERSION));

    QGraphicsScene *sceneTmp1 = new QGraphicsScene();
    QPixmap p1("scheme/graphics/logoUCA1.png");
    sceneTmp1->addPixmap(p1);
    ui->graphicsView_2->setStyleSheet("background: transparent");
    ui->graphicsView_2->setScene(sceneTmp1);
    ui->graphicsView_2->fitInView(sceneTmp1->itemsBoundingRect() ,Qt::IgnoreAspectRatio);



    QPixmap pixmap_ok("scheme/graphics/ok.png");
    QIcon ButtonIcon(pixmap_ok);
    ui->pushButton->setIcon(ButtonIcon);
    QSize s0(ui->pushButton->rect().size().width()-10,ui->pushButton->rect().size().height()-10);
    ui->pushButton->setIconSize(s0);
    ui->pushButton->setFixedSize(ui->pushButton->rect().size());
    ui->pushButton->setFlat(true);

    QGraphicsScene *sceneTmp = new QGraphicsScene();
    QPixmap p0("scheme/graphics/about3.jpg");
    sceneTmp->addPixmap(p0);

    ui->graphicsView->setStyleSheet("background: transparent");
    ui->graphicsView->setScene(sceneTmp);
    ui->graphicsView->fitInView(sceneTmp->itemsBoundingRect() ,Qt::IgnoreAspectRatio);


    //Para que la ventana no se maximice
   setFixedSize(width(),height());


    QGraphicsDropShadowEffect* effect0 = new QGraphicsDropShadowEffect();
    effect0->setBlurRadius(100);
    effect0->setOffset(3,3);
    ui->pushButton->setGraphicsEffect(effect0);




    //Para que la ventana no se maximice
    setFixedSize(width(),height());
    //Posicionamos la ventana en el centro
    this->move(QApplication::desktop()->availableGeometry().center() - this->rect().center());

}

About::~About()
{
    delete ui;
}

void About::on_pushButton_clicked()
{
    close();
}
