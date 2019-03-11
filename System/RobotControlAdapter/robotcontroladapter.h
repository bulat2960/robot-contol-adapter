#ifndef ROBOTCONTROLADAPTER_H
#define ROBOTCONTROLADAPTER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QEventLoop>

#include <QTime>

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
private:
    // Check if socket has a connected state, or unconnected state
    bool isConnectedState(QTcpSocket* socket) const;
    bool isUnconnectedState(QTcpSocket* socket) const;

    // Functions for processing new objects
    void processSingleCharCmd(QTcpSocket* socket, QByteArray name);
    void processPlannerCmd(QByteArray cmd);
    void processUnitCmd(QByteArray cmd);
public:
    // Basic constructor
    RobotControlAdapter(quint16 rcaPort, QString sceneIp, quint16 scenePort);
private slots:
    // Slots for necessary actions
    void incomingConnection(int socketDescriptor) override;
    void readyRead();
signals:
    void closeAll();
};

#endif // ROBOTCONTROLADAPTER_H
