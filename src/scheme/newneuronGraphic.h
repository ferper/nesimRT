#ifndef NEWNEURONFORM_H
#define NEWNEURONFORM_H

#include <QWidget>
#include <QDialog>
#include "node.h"
#include <QGraphicsItem>
#include <QGraphicsView>
#include "mainScreen.h"

namespace Ui { //form
class NewNeuronForm;
}

class NewNeuronGraphic : public QWidget
{
    Q_OBJECT

public:
       explicit NewNeuronGraphic(QWidget *parent = nullptr,MainGraphics *graphWidget=nullptr, QGraphicsScene *scene=nullptr, int *idGlobalNeuron=nullptr,  QList <Neuron *> *localNeurons=nullptr, QList <Node*> *vectorGraphicsNodes=nullptr, QString mathematicalModel="", bool *sceneBeSaved=nullptr);
    ~NewNeuronGraphic();
    void showEvent(QShowEvent *) override;

    bool existIpNeuron(QString ip);
    bool *sceneBeSaved;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_lineEdit_Label_returnPressed();

    void on_lineEdit_PosX_returnPressed();

    void on_lineEdit_Ip_returnPressed();

    void on_lineEdit_Amount_returnPressed();

    void on_lineEdit_Label_textEdited(const QString &arg1);

    void on_lineEdit_At_returnPressed();

    void on_lineEdit_PosY_returnPressed();

    void on_lineEdit_V_returnPressed();

    void on_lineEdit_Iexc_returnPressed();

    void on_lineEdit_Iinh_returnPressed();

    void on_lineEdit_tauExc_returnPressed();

    void on_lineEdit_tauInh_returnPressed();

    void on_lineEdit_tauV_returnPressed();

    void on_lineEdit_R_returnPressed();

    void on_lineEdit_Vr_returnPressed();

    void on_lineEdit_Vrh_returnPressed();

    void on_lineEdit_Vth_returnPressed();

    void on_lineEdit_Label_editingFinished();

    void on_lineEdit_Ip_textEdited(const QString &arg1);

    void on_lineEdit_Ip_editingFinished();

    void on_lineEdit_Amount_textEdited(const QString &arg1);

    void on_lineEdit_Amount_editingFinished();

    void on_lineEdit_PosX_textEdited(const QString &arg1);

    void on_lineEdit_PosX_editingFinished();

    void on_lineEdit_PosY_textEdited(const QString &arg1);

    void on_lineEdit_PosY_editingFinished();

    void on_lineEdit_V_textEdited(const QString &arg1);

    void on_lineEdit_V_editingFinished();

    void on_lineEdit_Iexc_textEdited(const QString &arg1);

    void on_lineEdit_Iexc_editingFinished();

    void on_lineEdit_Iinh_textEdited(const QString &arg1);

    void on_lineEdit_Iinh_editingFinished();

    void on_lineEdit_tauExc_textEdited(const QString &arg1);

    void on_lineEdit_tauExc_editingFinished();

    void on_lineEdit_tauInh_textEdited(const QString &arg1);

    void on_lineEdit_tauInh_editingFinished();

    void on_lineEdit_tauV_textEdited(const QString &arg1);

    void on_lineEdit_tauV_editingFinished();

    void on_lineEdit_R_textEdited(const QString &arg1);

    void on_lineEdit_R_editingFinished();

    void on_lineEdit_Vr_textEdited(const QString &arg1);

    void on_lineEdit_Vr_editingFinished();

    void on_lineEdit_Vrh_textEdited(const QString &arg1);

    void on_lineEdit_Vrh_editingFinished();

    void on_lineEdit_Vth_textEdited(const QString &arg1);

    void on_lineEdit_Vth_editingFinished();

    void on_lineEdit_At_textEdited(const QString &arg1);

    void on_lineEdit_At_editingFinished();
protected:
    void keyPressEvent(QKeyEvent *event);


private:
    bool parametersOK(); //Test if all types of parameters are ok.

    QList <Neuron*> *localNeurons;
    QList <Node*> *vectorGraphicsNodes;
    int *idGlobalNeuron;
    QString mathematicalModel;

    MainGraphics *graphWidget;
    QGraphicsScene *scene;
    Ui::NewNeuronForm *ui;
};

#endif // NEWNEURONFORM_H
