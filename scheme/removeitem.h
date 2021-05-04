#ifndef REMOVEITEM_H
#define REMOVEITEM_H

#include <QWidget>
#include "node.h"
#include "mainGraphics.h"
#include <QTimer>
namespace Ui {
class Removeitem;
}

class Removeitem : public QWidget
{
    Q_OBJECT

public:

    explicit Removeitem(QWidget *parent = nullptr, QList <Node*> *vectorGraphicsNodes=nullptr);
    ~Removeitem();

    QList <Node*> *vectorGraphicsNodes;

    void fillAllNeurons(QString Key="", QString value="");
    void fill_LocalSynapsys(QString ipNodeTarget="", QString key="", QString value="");


    void fill_GlobalSynapsys(QString Key="", QString value="");
    void showEvent(QShowEvent *) override;


private slots:
    void on_pushButton_clicked();

    void on_tableWidget_Neurons_cellClicked(int row, int column);

    void on_pushButton_3_clicked();

    void on_pushButton_8_clicked();

    void neuronFilterSearch();
    void LocalSynapsysFilterSearch();
    void GlobalSynapsysFilterSearch();


    void on_pushButton_2_clicked();

private:
    Ui::Removeitem *ui;
    Node* findNode(QString key, QString value);

    bool NodeHasSynapsys(Node * node);
    void sendMsg(QString msg, quint16 port);

    QTimer timer_NeuronsRefresh;        //For refresh the Neuron's list into screen
    QTimer timer_LocalSynapsysRefresh;  //For refresh the Neuron's local synapsys  list into screen
    QTimer timer_GlobalSynapsysRefresh; //For refresh the Neuron's global synapsys list into screen

protected:
    void keyPressEvent(QKeyEvent *event);

};

#endif // REMOVEITEM_H
