#ifndef WIDGET_H
#define WIDGET_H

#include <QTimer>
#include <QWidget>
#include "TickerGraph.h"
#include "parameters.h"
#include "synapse.h"
#include "typedata.h"
#include <QKeyEvent>

namespace Ui {
    class Widget;
}

class Widget : public QWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr, QString *ipmSource=nullptr, QString *label=nullptr, bool *dataIsAvailable=nullptr, long long int *muestra=nullptr, double *iexcCurrent=nullptr, double *iinhCurrent=nullptr, double *vCurrent=nullptr, Parameters *p=nullptr, QVector <Synapse *> *Vsynapse=nullptr, int *NumberNeuronsGroup=nullptr);
    ~Widget() override;

    void showParameters();
    void showEvent(QShowEvent *) override;

    double *Iexc;
    double *Iinh;
    double *V;
    long long int *muestra;
    bool *dataIsAvailable;
    QString *ipmSource;
    Parameters *p;
    QVector <Synapse *> *Vsynapse;
    typeFxI fxI;
    void show(); //overrided
    void showSynapsys();
    int *NumberNeuronsGroup;
    QString *label;

protected:
    bool event(QEvent *event) override; //To process onShow event window
    void keyPressEvent(QKeyEvent *event) override;

public slots:
    void paintGraphic();

private slots:
    void on_pushButton_clicked();
    void on_spinBox_valueChanged(int arg1);
    void cellSelected(int row, int col);
    bool parametersOK();
    void on_spinBox_3_valueChanged(int arg1);
    void on_spinSizeX_valueChanged(int arg1);
    void on_spinSizeY_valueChanged(int arg1);
    void on_spinSampler_valueChanged(int arg1);
    void on_pushButton_2_clicked();
    void on_SpinLabelTime_valueChanged(int arg1);
    void on_pushButton_3_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_16_clicked();
    void on_pushButton_15_clicked();
    void on_pushButton_14_clicked();
    void on_pushButton_10_clicked();
    void on_pushButton_11_clicked();
    void on_pushButton_12_clicked();
    void on_pushButton_13_clicked();
    void on_pushButton_17_clicked();
    void on_lineEdit_Label_returnPressed();
    void on_pushButton_18_clicked();
    void on_lineEdit_Group_returnPressed();
    void on_lineEdit_tauExc_returnPressed();
    void on_lineEdit_tauInh_returnPressed();
    void on_lineEdit_tauV_returnPressed();
    void on_lineEdit_R_returnPressed();
    void on_lineEdit_Vr_returnPressed();
    void on_lineEdit_Vrh_returnPressed();
    void on_lineEdit_Vth_returnPressed();
    void on_lineEdit_At_returnPressed();
    void on_pushButton_okChanges_clicked();
    void on_lineEdit_Label_editingFinished();
    void on_lineEdit_Label_textEdited(const QString &arg1);
    void on_lineEdit_Group_textEdited(const QString &arg1);
    void on_lineEdit_Group_editingFinished();
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
    void on_pushButton_19_clicked();
    void on_pushButton_20_clicked();
    void on_lineEdit_V_returnPressed();
    void on_lineEdit_Iexc_returnPressed();
    void on_lineEdit_Iinh_returnPressed();
    void on_lineEdit_V_textEdited(const QString &arg1);
    void on_lineEdit_V_editingFinished();
    void on_lineEdit_Iexc_textEdited(const QString &arg1);
    void on_lineEdit_Iexc_editingFinished();
    void on_lineEdit_Iinh_textEdited(const QString &arg1);
    void on_lineEdit_Iinh_editingFinished();
    void on_pushButton_7_clicked();
    void on_pushButton_21_clicked();
    void on_pushButton_22_clicked();

private:
    int fr; // Sampling rate 
    Ui::Widget *ui;
    TickerGraph * graph;
    QTimer timer;
    bool isDrawing;
    void onShow();  //Function to be called from event function
    int rightMargen;
    int bottomMargen;
    int windowWidth;
    int windowHeight;
    int posCanvasX;
    int posCanvasY;
};

#endif // WIDGET_H
