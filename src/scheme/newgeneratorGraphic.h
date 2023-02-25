#ifndef NEWGENERATORFORM_H
#define NEWGENERATORFORM_H

#include <QWidget>
#include "mainScreen.h"
#include "node.h"
#include "mainGraphics.h"

namespace Ui {
class NewGeneratorForm;
}

class NewGeneratorGraphics : public QWidget
{
    Q_OBJECT

public:
    explicit NewGeneratorGraphics(QWidget *parent = nullptr,MainGraphics *graphWidget=nullptr,float posX=350, float posY=350, int *idGlobalNeuron=nullptr, QList <Node*> *vectorGraphicsNodes=nullptr, QList <Neuron *> *localNeurons=nullptr,bool *sceneBeSaved=nullptr);
    ~NewGeneratorGraphics();
    void showEvent(QShowEvent *);

private slots:

    void on_pushButton_clicked();

    void on_lineEdit_Ip_textEdited(const QString &arg1);

    void on_lineEdit_Ip_editingFinished();

    void on_lineEdit_Label_textEdited(const QString &arg1);

    void on_lineEdit_Label_editingFinished();

    void on_lineEdit_PosX_textEdited(const QString &arg1);

    void on_lineEdit_PosX_editingFinished();

    void on_lineEdit_PosY_textEdited(const QString &arg1);

    void on_lineEdit_PosY_editingFinished();

    void on_lineEdit_Frecuency_textEdited(const QString &arg1);

    void on_lineEdit_Frecuency_editingFinished();

    void on_lineEdit_Amount_textEdited(const QString &arg1);

    void on_lineEdit_Amount_editingFinished();

    void on_lineEdit_Ip_returnPressed();

    void on_lineEdit_Label_returnPressed();

    void on_lineEdit_PosX_returnPressed();

    void on_lineEdit_PosY_returnPressed();

    void on_lineEdit_Frecuency_returnPressed();

    void on_lineEdit_Amount_returnPressed();

    void on_pushButton_2_clicked();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::NewGeneratorForm *ui;
    int *idGlobalNeuron;
    MainGraphics *graphWidget;
    QList <Node*> *vectorGraphicsNodes;
    QVector <Neuron *> *Vneurons;
    QList <Neuron *> *localNeurons;
    bool parametersOK(); //Test if all types of parameters are ok.

    bool *sceneBeSaved;
};

#endif // NEWGENERATORFORM_H
