#ifndef ROBOTCONTROLADAPTER_H
#define ROBOTCONTROLADAPTER_H

#include <QTcpServer>
#include <QTcpSocket>

/**
 * It's the main object - a server, who can connect to 3DScene,
 * contain and support connections of other clients and planner,
 * can receive their commands and retranslate them to 3DScene.
**/

class RobotControlAdapter : public QTcpServer // Simple tcp-server
{
    Q_OBJECT
private:
    // Sockets without name
    QList<QTcpSocket*> waitSockets;

    // Clients(units) with name
    QMap<QString, QTcpSocket*> clients;

    // Planner socket
    QTcpSocket* planner;

    // 3DScene socket
    QTcpSocket* sceneSocket;
public:
    // Basic c-tor
    RobotControlAdapter();
public slots:
    // Slots for necessary actions
    void incomingConnection(int socketDescriptor);
    void readyRead();
    void showAll();
};

#endif // ROBOTCONTROLADAPTER_H
