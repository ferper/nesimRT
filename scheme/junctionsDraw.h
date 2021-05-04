#ifndef JUNCTIONSDRAW_H
#define JUNCTIONSDRAW_H

#include <QGraphicsItem>
#include "config.h"

class Node;



class JunctionsDraw : public QGraphicsItem
{
public:
    JunctionsDraw(Node *sourceNode=nullptr, Node *targetNode=nullptr, int typeTargetNeuron=TYPENEURON_NORMAL, double w=0.0, QString fx="nA",int typeSynapse=TYPE_SYP_EXCITATION, QString idGlobalSynapse="");

    Node *sourceNode() const;
    Node *targetNode() const;

    void adjust();

    enum { Type = UserType + 2 };
    int type() const override { return Type; }
    double w;
    QString fx;
    QString idGlobalSynapse;

    int typeTargetNeuron=typeTargetNeuron; //0 = none, 1=mother, 2=generator, 3=neuron

    bool self;
    int typeSynapse;

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    Node *source, *target;

    QPointF sourcePoint;
    QPointF targetPoint;
    qreal arrowSize;
};


#endif // JUNCTIONSDRAW_H
