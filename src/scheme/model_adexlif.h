#ifndef MODELADEXLIF_H
#define MODELADEXLIF_H

#include <QWidget>
#include "scheme/node.h"
#include <QGraphicsDropShadowEffect>

namespace Ui {
class ModelAdExLIF;
}

class ModelAdExLIF : public QWidget
{
    Q_OBJECT

public:
    explicit ModelAdExLIF(QWidget *parent = nullptr, Node *node=nullptr);
    void show(); //overrided
    void showSynapsys();
    void showParameters();
    bool parmatersOK();
    void showEvent(QShowEvent *);

    ~ModelAdExLIF();

private slots:

    void cellSelected(int row, int col);

    void on_pushButton_5_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_lineEdit_Label_textEdited(const QString &arg1);

    void on_lineEdit_Label_editingFinished();

    void on_lineEdit_Amount_textEdited(const QString &arg1);

    void on_lineEdit_Amount_editingFinished();

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

    void on_lineEdit_textEdited(const QString &arg1);

    void on_lineEdit_editingFinished();

    void on_lineEdit_Amount_returnPressed();

    void on_lineEdit_tauExc_returnPressed();

    void on_lineEdit_tauInh_returnPressed();

    void on_lineEdit_tauV_returnPressed();

    void on_lineEdit_R_returnPressed();

    void on_lineEdit_Vr_returnPressed();

    void on_lineEdit_Vrh_returnPressed();

    void on_lineEdit_Vth_returnPressed();

    void on_lineEdit_At_returnPressed();

    void on_lineEdit_Label_returnPressed();

    void on_lineEdit_V_textEdited(const QString &arg1);

    void on_lineEdit_V_editingFinished();

    void on_lineEdit_Iexc_textEdited(const QString &arg1);

    void on_lineEdit_Iexc_editingFinished();

    void on_lineEdit_Iinh_textEdited(const QString &arg1);

    void on_lineEdit_Iinh_editingFinished();

    void on_lineEdit_V_returnPressed();

    void on_lineEdit_Iexc_returnPressed();

    void on_lineEdit_Iinh_returnPressed();

    void on_pushButton_2_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_Exit_clicked();

    void on_pushButton_applyData_clicked();

    void on_pushButton_ApplySynapse_clicked();

    void on_pushButton_Refresh_clicked();

protected:
        void keyPressEvent(QKeyEvent *event);
private:
    Node *node;

    QString mathematicalModel;
    Ui::ModelAdExLIF *ui;
};

#endif // MODELADEXLIF_H
