#include "junctionsDraw.h"
#include "node.h"
#include <qmath.h>
#include <QPainter>
#include <QTextItem>
#include <iostream>
#include <QtMath>
#include <QtGui>
#include <qgraphicsproxywidget.h>
#include "config.h"
#include "typedata.h"
extern typeFxI fxI;

JunctionsDraw::JunctionsDraw(Node *sourceNode, Node *targetNode, int typeTargetNeuron, double w, QString FX, int typeSynapse, QString idGlobalSynapse)
    : arrowSize(10)
{
    setAcceptedMouseButtons(nullptr);
    source = sourceNode;
    target = targetNode;
    source->addJunction(this);
    target->addJunction(this);
    this->w=w;
    this->fx=FX;
    this->idGlobalSynapse=idGlobalSynapse;

    this->typeSynapse=typeSynapse;
    this->typeTargetNeuron=typeTargetNeuron;
    source->self=(source==target);
    target->self=(source==target);

    adjust();
}

Node *JunctionsDraw::sourceNode() const
{
    return source;
}

Node *JunctionsDraw::targetNode() const
{
    return target;
}

void JunctionsDraw::adjust()
{

    if (!source || !target)
        return;

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(target, 0, 0));

    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.)) {
        QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        sourcePoint = line.p1() + edgeOffset;
        targetPoint = line.p2() - edgeOffset;

    }
    else
        sourcePoint = targetPoint = line.p1();
}

QRectF JunctionsDraw::boundingRect() const
{
    if (!source || !target)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0;

    if (source==target) {
        return QRectF(sourcePoint, QSizeF(targetPoint.x() - sourcePoint.x(),
                                          targetPoint.y() - sourcePoint.y()))
            .normalized()
            .adjusted(-extra-400, -extra-400, extra+400, extra+400);
    }

    return QRectF(sourcePoint, QSizeF(targetPoint.x() - sourcePoint.x(),
                                      targetPoint.y() - sourcePoint.y()))
        .normalized()
        .adjusted(-extra-400, -extra-400, extra+400, extra+400);

}

void JunctionsDraw::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{

    if (!source || !target)
        return;

    painter->setPen(QPen(Qt::black,2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    QLineF line(sourcePoint, targetPoint);
    line.setLength(line.length()-10);

    if (this->typeSynapse==TYPE_SYP_INHIBITION)
        painter->setPen(QPen(Qt::red,2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    if (source==target){ //SelfLink
        QRectF rectangle(sourcePoint.rx()-35,sourcePoint.ry()-30,25,25);
        int startAngle = 0;
        int spanAngle = 300 * 16;
        painter->drawArc(rectangle, startAngle, spanAngle);

        painter->drawText(int(sourcePoint.rx()-40),int(sourcePoint.ry()-30),"w="+QString::number(w)+" "+fx);

        if (this->typeSynapse== TYPE_SYP_INHIBITION) {
            painter->setPen(QPen(Qt::red,2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            QPointF p1(rectangle.topRight().rx(),rectangle.topRight().ry()+10);
            painter->drawEllipse(p1, 4,4);
        }
        else {
            painter->setPen(QPen(Qt::black,2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            QPointF p1(rectangle.topRight().rx()-2,rectangle.topRight().ry()+15);
            QPointF p2(rectangle.topRight().rx()+5,rectangle.topRight().ry()+11);
            QPointF p3(rectangle.topRight().rx(),rectangle.topRight().ry()+9);
            painter->drawPoint(p1);
            painter->drawPoint(p2);
            painter->drawPoint(p3);
            painter->drawPolygon(QPolygonF() << p1<<p2<<p3);
        }
    }
    if (qFuzzyCompare(line.length(), qreal(0.))) // Neuron without synapse
       return;

    painter->drawLine(line);


    // Draw the arrows
    double angle = std::atan2(-line.dy(), line.dx());

    QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + M_PI / 3) * arrowSize,
                                                  cos(angle + M_PI / 3) * arrowSize);
    QPointF destArrowP1;
    QPointF destArrowP2;
    painter->setBrush(Qt::black);

    double a=targetPoint.rx()-sourcePoint.rx();
    double b=targetPoint.ry()-sourcePoint.ry();
    double h=qSqrt((a*a)+(b*b));
    double angulo=qRadiansToDegrees(-angle);

    if ((angulo>90) && (angulo<180))
        angulo=angulo+180;
    else if ((angulo>-180) && (angulo<-90))
        angulo=angulo-180;
    if (angulo>90)
        h*=-1;
    else if ((angulo>-360) && (angulo<-270))
        h*=-1;

    painter->begin(painter->device());
    painter->translate(sourceArrowP1);
    painter->rotate(angulo);
    painter->drawText(int(h/2-25),20,"w="+QString::number(w)+" "+fx);
    painter->restore();
    painter->setBrush(Qt::black);


    if (this->typeSynapse==TYPE_SYP_EXCITATION) {
        destArrowP1 = targetPoint + QPointF(sin(angle - M_PI / 3) * arrowSize,
                                                  cos(angle - M_PI / 3) * arrowSize);
        destArrowP2 = targetPoint + QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize,
                                                      cos(angle - M_PI + M_PI / 3) * arrowSize);
        line.setLength(line.length()-5);
        painter->setPen(QPen(Qt::black,2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

        painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
        std::cout<<"angle Antes: " <<angulo<<std::endl;

       std::cout<<"angle Despues: " <<angulo<<std::endl;

    }
    else {
        destArrowP1 = targetPoint + 1.2*QPointF(sin(angle - M_PI / 3) * arrowSize,
                                                          cos(angle - M_PI / 3) * arrowSize);
        destArrowP2 = targetPoint + 1.2*QPointF(sin(angle - M_PI + M_PI / 3) * arrowSize,
                                                          cos(angle - M_PI + M_PI / 3) * arrowSize);
        painter->setPen(QPen(Qt::red,2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        line.setLength(line.length()-3);
        painter->drawEllipse(line.p2(), 4,4);
    }
}
