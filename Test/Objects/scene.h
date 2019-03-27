#ifndef SCENE_H
#define SCENE_H

#include <QTcpServer>
#include <QTcpSocket>

/**
 * This is a mock-object of 3DScene
 * It can only wait for connecting RCA-module and
 * reading its messages.
**/

class Scene : public QTcpServer // Simple tcp server
{
    Q_OBJECT
private:
    quint16 port;
    // Socket for connect to RCA
    QTcpSocket* rcaSocket;
public:
    // Basic constructor
    Scene(quint16 port);
public slots:
    // Slots for necessary actions
    void readyRead();
protected:
    void incomingConnection(qintptr socketDescriptor) override;
};

#endif // SCENE_H
