#ifndef NODE_H
#define NODE_H

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QList>
#include "neuron.h"
#include "models/neuron_cubalif.h"
#include "models/neuron_izhikevich.h"
#include "models/neuron_adexlif.h"

class JunctionsDraw;
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class Node : public QGraphicsItem
{

public:

    struct DataSynapsys {
        QString idGlobal;
        int type;
        QString ipmTarget;
        QString ipmSource;
        quint16 port;
        double w;
        QString fx_numberTxt;       //fx's value Text
        QString fx_unitMeasureTxt;  //fx's unit's name

    }typedef TypeDataSynapsys;

    Node(QGraphicsView *graphWidget,float posX=350, float posY=350, QList <Neuron*> *localNeurons=nullptr);

    void addJunction(JunctionsDraw *junction);

    enum { Type = UserType + 1 };
    int type() const override { return Type; }

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void setLabel(QString label);
    QString ip;
    QString id;
    QString label;
    float posX;
    float posY;


    float posGraph_X;
    float posGraph_Y;
    double scale;
    int typeNode;   // 0=none; 1=MotherNeuron; 2=Generator; 3=Normal
    int frecuency;  // Only for the Generator Node
    //Mathematical model Variables
    QString model;
    double w; //Default value for synapsys
    QString fx_numberTxt; //fx's value Text
    int amountOfNeurons; //If N==1 the neuron es alone. N>1 it's a group

    bool dataIsAvailable;

    QString ipmSource; // Ip multicast de la neurona. Para que otras puedan subscribirse a ella.
    bool enableDataGeneralMonitor;
    void * FormDialog;
    Parameters *parameters;

    QVector <DataSynapsys> synapsys;
    int localRemote;

    bool self;
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QGraphicsView *graphWidget;

    QList<JunctionsDraw *> edgeList;
    QList<Neuron*> *localNeurons;
    QPointF newPos;
    QGraphicsView *graph;
};

#endif // NODE_H
