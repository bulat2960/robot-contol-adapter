#ifndef SCENE_H
#define SCENE_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>

/**
 * This is a mock-object of 3DScene
 * It can only wait for connecting RCA-module and
 * reading its messages.
**/

class Scene : public QTcpServer // Simple tcp server
{
    Q_OBJECT
private:
    // Socket for connect to RCA
    QTcpSocket* rcaSocket;

    // Function execution time
    QTime timer;
public:
    // Basic c-tor
    Scene();
public slots:
    // Slots for necessary actions
    void incomingConnection(int socketDescriptor);
    void readyRead();
};

#endif // SCENE_H
