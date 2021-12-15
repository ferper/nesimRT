#include "encodeDecodeMsg.h"
#include <iostream>

EncodeDecodeMsg::EncodeDecodeMsg(QObject *parent):  QObject(parent){

}

void EncodeDecodeMsg::decodeMsg(QString input)
{
    int idx=input.indexOf(sep_operation,0);
    int idx1;

    if (input.toStdString().substr(0,idx)==header.toStdString()) {
       //std::cout<<" IS A CONFIGURATION MESSAGE"<<std::endl;
       //std::cout<<input.toStdString()<<"\n";
       idx++;
       idx1= input.indexOf(sep_operation,idx);  //Searching the operation type
       operation = input.toStdString().substr(idx,idx1-idx).c_str(); //Assinging the operation type
       idx1++;

       idx= input.indexOf(separator,idx1);
       if (idx==-1)
          return;
       field1=input.toStdString().substr(idx1, idx-idx1).c_str();
       idx++;

       idx1= input.indexOf(separator,idx);
       if (idx1==-1)
          return;
       field2 = input.toStdString().substr(idx, idx1-idx).c_str();
       idx1++;

       idx= input.indexOf(separator,idx1);
       if (idx==-1)
          return;
       field3 = input.toStdString().substr(idx1, idx-idx1).c_str();
       idx++;

       idx1= input.indexOf(separator,idx);
       if (idx1==-1)
          return;
       field4 = input.toStdString().substr(idx, idx1-idx).c_str();
       idx1++;

       idx= input.indexOf(separator,idx1);
       if (idx==-1)
          return;
       field5 = input.toStdString().substr(idx1, idx-idx1).c_str();
       idx++;

       idx1= input.indexOf(separator,idx);
       if (idx==-1)
          return;
       field6 = input.toStdString().substr(idx, idx1-idx).c_str();
       idx1++;

       idx= input.indexOf(separator,idx1);
       if (idx==-1)
          return;
       field7 = input.toStdString().substr(idx1, idx-idx1).c_str();
       idx++;

       idx1= input.indexOf(separator,idx);
       if (idx1==-1)
          return;
       field8= input.toStdString().substr(idx, idx1-idx).c_str();
       idx1++;

       idx= input.indexOf(separator,idx1);
       if (idx==-1)
          return;
       field9 = input.toStdString().substr(idx1, idx-idx1).c_str();
       idx++;

       idx1= input.indexOf(separator,idx);
       if (idx1==-1)
          return;
       field10= input.toStdString().substr(idx, idx1-idx).c_str();
       idx1++;

       idx= input.indexOf(separator,idx1);
       if (idx==-1)
          return;
       field11 = input.toStdString().substr(idx1, idx-idx1).c_str();
       idx++;

       idx1= input.indexOf(separator,idx);
       if (idx1==-1)
          return;
       field12= input.toStdString().substr(idx, idx1-idx).c_str();
       idx1++;

       idx= input.indexOf(separator,idx1);
       if (idx==-1)
          return;
       field13 = input.toStdString().substr(idx1, idx-idx1).c_str();
       idx++;

       idx1= input.indexOf(separator,idx);
       if (idx1==-1)
          return;
       field14= input.toStdString().substr(idx, idx1-idx).c_str();
       idx1++;

       idx= input.indexOf(separator,idx1);
       if (idx==-1)
          return;
       field15 = input.toStdString().substr(idx1, idx-idx1).c_str();
       idx++;

       idx1= input.indexOf(separator,idx);
       if (idx1==-1)
          return;
       field16= input.toStdString().substr(idx, idx1-idx).c_str();
       idx1++;

       idx= input.indexOf(separator,idx1);
       if (idx==-1)
          return;
       field17 = input.toStdString().substr(idx1, idx-idx1).c_str();
       idx++;

       idx1= input.indexOf(separator,idx);
       if (idx1==-1)
          return;
       field18= input.toStdString().substr(idx, idx1-idx).c_str();
       idx1++;

       idx= input.indexOf(separator,idx1);
       if (idx==-1)
          return;
       field19 = input.toStdString().substr(idx1, idx-idx1).c_str();
       idx++;

       idx1= input.indexOf(separator,idx);
       if (idx1==-1)
          return;
       field20= input.toStdString().substr(idx, idx1-idx).c_str();
       idx1++;

       idx= input.indexOf(separator,idx1);
       if (idx==-1)
          return;
       field21 = input.toStdString().substr(idx1, idx-idx1).c_str();
       idx++;

       idx1= input.indexOf(separator,idx);
       if (idx1==-1)
          return;
       field22= input.toStdString().substr(idx, idx1-idx).c_str();
       idx1++;

       idx= input.indexOf(separator,idx1);
       if (idx==-1)
          return;
       field23 = input.toStdString().substr(idx1, idx-idx1).c_str();
       idx++;

       idx1= input.indexOf(separator,idx);
       if (idx1==-1)
          return;
       field24= input.toStdString().substr(idx, idx1-idx).c_str();
       idx1++;

       idx= input.indexOf(separator,idx1);
       if (idx==-1)
          return;
       field25 = input.toStdString().substr(idx1, idx-idx1).c_str();
       idx++;

       idx1= input.indexOf(separator,idx);
       if (idx1==-1)
          return;
       field26= input.toStdString().substr(idx, idx1-idx).c_str();
       idx1++;
       idx= input.indexOf(separator,idx1);
       if (idx==-1)
          return;
       field27 = input.toStdString().substr(idx1, idx-idx1).c_str();
       idx++;

       idx1= input.indexOf(separator,idx);
       if (idx1==-1)
          return;
       field28= input.toStdString().substr(idx, idx1-idx).c_str();
       idx1++;

       idx= input.indexOf(separator,idx1);
       if (idx==-1)
          return;
       field29 = input.toStdString().substr(idx1, idx-idx1).c_str();
       idx++;

       idx1= input.indexOf(separator,idx);
       if (idx1==-1)
          return;
       field30= input.toStdString().substr(idx, idx1-idx).c_str();
       idx1++;

       idx= input.indexOf(separator,idx1);
       if (idx==-1)
          return;
       field31 = input.toStdString().substr(idx1, idx-idx1).c_str();
       idx++;

       idx1= input.indexOf(separator,idx);
       if (idx1==-1)
          return;
       field32= input.toStdString().substr(idx, idx1-idx).c_str();
       idx1++;

    }
}

QString EncodeDecodeMsg::encondeMsg(QString operation,QString f1,QString f2,QString f3, QString f4, QString f5, QString f6, QString f7, QString f8,
                       QString f9,QString f10,QString f11, QString f12,QString f13, QString f14, QString f15, QString f16,
                       QString f17,QString f18,QString f19, QString f20,QString f21, QString f22, QString f23, QString f24,
                       QString f25,QString f26,QString f27, QString f28,QString f29, QString f30, QString f31, QString f32) {
    QString tmp= "#"+sep_operation+operation+sep_operation+f1+separator+f2+separator+f3+separator+f4+separator+f5+separator+f6+separator+f7+separator+f8+separator;
    tmp+=f9+separator+f10+separator+f11+separator+f12+separator+f13+separator+f14+separator+f15+separator+f16+separator;
    tmp+=f17+separator+f18+separator+f19+separator+f20+separator+f21+separator+f22+separator+f23+separator+f24+separator;
    tmp+=f25+separator+f26+separator+f27+separator+f28+separator+f29+separator+f30+separator+f31+separator+f32+separator;

    return tmp;
}
