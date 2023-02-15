#ifndef NEWSPINNAKER_H
#define NEWSPINNAKER_H

#include <QWidget>
#include <QGraphicsDropShadowEffect>
#include <QKeyEvent>
#include "junctionsDraw.h"
#include "node.h"

namespace Ui {
class NewSpiNNaker;
}

class NewSpiNNaker : public QWidget
{
    Q_OBJECT

public:
    explicit NewSpiNNaker(QWidget *parent = nullptr,QList <Node*> *vectorGraphicsNodes=nullptr,QVector <JunctionsDraw*> *vectorGraphicsEdge=nullptr);
    void showEvent(QShowEvent *) override;
    void exportToSpiNNaker();
    void exportToBrian2();
    ~NewSpiNNaker();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void neuronFilterSearch();


    void on_pushButton_5_clicked();

    void on_bt_exportBrian2_clicked();

private:
    Ui::NewSpiNNaker *ui;
    QString fileName;
    bool ParametersOk();
    float timeStep;
    int delay;
    int timeSimulation;

    void fillAllNeurons(QString Key="", QString value="");

    Node* findNode(QString key, QString value);

    QList <Node*> *vectorGraphicsNodes;

};

#endif // NEWSPINNAKER_H
