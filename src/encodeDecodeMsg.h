#ifndef ENCODEDECODEMSG_H
#define ENCODEDECODEMSG_H

#include <QObject>
#include "messages.h"

class EncodeDecodeMsg : public QObject
{
    Q_OBJECT
public:
    explicit EncodeDecodeMsg(QObject *parent = nullptr);
    void decodeMsg(QString input="");
    QString encondeMsg(QString operation,QString f1="",QString f2="",QString f3="", QString f4="",QString f5="", QString f6="", QString f7="", QString f8="",
                       QString f9="",QString f10="",QString f11="", QString f12="",QString f13="", QString f14="", QString f15="", QString f16="",
                       QString f17="",QString f18="",QString f19="", QString f20="",QString f21="", QString f22="", QString f23="", QString f24="",
                       QString f25="",QString f26="",QString f27="", QString f28="",QString f29="", QString f30="", QString f31="", QString f32="");
    QString operation;
    QString field1;
    QString field2;
    QString field3;
    QString field4;
    QString field5;
    QString field6;
    QString field7;
    QString field8;

    QString field9;
    QString field10;
    QString field11;
    QString field12;
    QString field13;
    QString field14;
    QString field15;
    QString field16;
    QString field17;
    QString field18;
    QString field19;
    QString field20;
    QString field21;
    QString field22;
    QString field23;
    QString field24;
    QString field25;
    QString field26;
    QString field27;
    QString field28;
    QString field29;
    QString field30;
    QString field31;
    QString field32;

private:

signals:

public slots:

};

#endif // DECODEMSG_H
