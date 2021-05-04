#include "junctionsDraw.h"
#include "node.h"
#include "mainGraphics.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <iostream>
#include "scheme/model_adexlif.h"
#include "scheme/newgeneratorGraphic.h"

Node::Node(MainGraphics *graphWidget,float posX, float posY, QList <Neuron*> *localNeurons) : graph(graphWidget)

{
    this->localNeurons=localNeurons;
    this->graphWidget=graphWidget;

    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);

    scale=1.8;      // default scale
    ip="233.0.0.1"; // default ip
    amountOfNeurons=1;
    typeNode=TYPENEURON_NORMAL; //Normal Neuron
    this->posX=posX;
    this->posY=posY;
    posGraph_X=posX;
    posGraph_Y=posY;
    frecuency=1000; //Only if the Node is a Generator

    w=1; // Default value for synapse
        this->fx_numberTxt="1e-9"; //Units of measure is nA
}

void Node::setLabel(QString label)  {
   this->label=label;
}

void Node::addJunction(JunctionsDraw *junction)
{
    edgeList << junction;
    junction->adjust();
}

QRectF Node::boundingRect() const
{
    qreal adjust = 2;
    //Ancho de etiqueta de Neurona
    return QRectF( -10 - adjust, -10 - adjust, 100 + adjust, 23 + adjust);
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(-7, -7, 20, 20); //shadow


    QRadialGradient gradient(-3, -3, 10);
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, QColor(Qt::red).light(120));
        gradient.setColorAt(0, QColor(Qt::darkRed).light(120));
    } else {
        gradient.setColorAt(0, Qt::green);
        gradient.setColorAt(1, Qt::darkGreen);
    }
    painter->setBrush(gradient);

    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-10, -10, 20, 20); //Outline

    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(5);

    painter->setFont(font);

    QPoint p;
    p.rx()=-5;
    p.ry()=0;
    painter->drawText(p, label);
}
QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (JunctionsDraw *edge, edgeList)
            edge->adjust();

        graph->itemMoved();

        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}
void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event){

    bool found=false;
    QList <Neuron *>::iterator it=localNeurons->begin();
    while ((!found) && (it!=localNeurons->end())) {
        if ((*it)->ipmSource.toStdString() ==this->ipmSource.toStdString())
            found=true;
        else
            ++it;
    }
    if (found) {
        ((QDialog*)(*it)->FormDialog)->show();
        ((QDialog*)(*it)->FormDialog)->activateWindow();

    }

    if ((!found) && (this->typeNode==TYPENEURON_NORMAL)) {
        if (FormDialog) {
            ((ModelAdExLIF*)FormDialog)->show();
            ((ModelAdExLIF*)FormDialog)->activateWindow();
        }
        else {
            //Solo creamos un ModelAdExLIF form cuando se hace un doble click sobre el Node
            ModelAdExLIF *m1 = new  ModelAdExLIF(nullptr,this);
            FormDialog=m1; //Para que se pueda refrescar los cambios realizados desde fuera en tiempo real.
            m1->show();
        }
    }
    else if ((!found) && (this->typeNode==TYPENEURON_GENERATOR)) {
    }

    QGraphicsItem::mouseDoubleClickEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    this->posGraph_X=event->scenePos().x();
    this->posGraph_Y=event->scenePos().y();
    std::cout<<"X: "<<posGraph_X<<" Y:"<<posGraph_Y<<std::endl;

    QGraphicsItem::mouseReleaseEvent(event);
}
