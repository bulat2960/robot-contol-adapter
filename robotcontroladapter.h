#ifndef ROBOTCONTROLADAPTER_H
#define ROBOTCONTROLADAPTER_H

#include <QTcpServer>
#include <QTcpSocket>

class RobotControlAdapter : public QTcpServer
{
    Q_OBJECT
private:
    QList<QTcpSocket*> waitSockets;
    QMap<QString, QTcpSocket*> clients;
public:
    RobotControlAdapter();
public slots:
    void incomingConnection(int socketDescriptor);
    void readyRead();
    void deleteSocket();
};

#endif // ROBOTCONTROLADAPTER_H
