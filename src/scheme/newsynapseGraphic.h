#ifndef NEWSYNAPSEGRAPHIC_H
#define NEWSYNAPSEGRAPHIC_H

#include <QWidget>
#include "junctionsDraw.h"
#include "node.h"
#include <QGraphicsItem>
#include <QGraphicsView>
#include "motherNeuron.h"
#include "config.h"
#include "encodeDecodeMsg.h"
#include <QGraphicsDropShadowEffect>
#include <QPushButton>
namespace Ui {
class NewSynapseForm;
}

class NewSynapseGraphic : public QWidget
{
    Q_OBJECT

public:

    explicit NewSynapseGraphic(QWidget *parent = nullptr, int *idGlobalSynapse=nullptr, QGraphicsScene *scene=nullptr, QList <Node*> *vectorGraphicsNeuron=nullptr, QVector <JunctionsDraw*> *vectorGraphicsEdge=nullptr, bool *sceneBeSaved=nullptr);

    ~NewSynapseGraphic();

    int generate_Globalidsynapse();
    QMutex mutexIdSynapse;

    bool *sceneBeSaved;
private slots:

    void neuronFilterSearchSource();
    void neuronFilterSearchTarget();
    void on_comboBox_Source_currentIndexChanged(int index);

    void on_comboBox_Target_currentIndexChanged(int index);

    void on_pushButton_Create_clicked();

    void on_SALIR_clicked();

    void on_pushButton_Exit_clicked();

private:
    void fullTableWidgetdataNeurons(QTableWidget *table);
    void filterTableWidgetdataNeurons(QTableWidget table, int field, QString filter);
    void neuronFilterSearch(int entity);
    void sendMsg(QString msg, quint16 port);


    QGraphicsScene *scene;
    Ui::NewSynapseForm *ui;
    QList <Node*> *vectorGraphicsNeuron;
    QVector <JunctionsDraw*> *vectorGraphicsEdge;
    QWidget *parent;
    int *idGlobalSynapse;

protected:
    void keyPressEvent(QKeyEvent *event) override;

};

#endif // NEWSYNAPSEGRAPHIC_H
