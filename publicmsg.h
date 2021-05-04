#ifndef PUBLICMSG_H
#define PUBLICMSG_H
#include <QThread>
#include <QHostAddress>
#include <QUdpSocket>
class PublicMSG : public QThread
{
    Q_OBJECT
public:
    explicit PublicMSG(QObject *parent = nullptr,  QPixmap *originalPixmap=nullptr);
     void run() override;
    QPixmap *originalPixmap;
private:
    QString msg;
    quint16 port;
    QHostAddress groupAddress;

signals:

public slots:

};

#endif // PUBLICMSG_H
