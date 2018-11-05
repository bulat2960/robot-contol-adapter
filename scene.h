#ifndef SCENE_H
#define SCENE_H

#include <QTcpServer>
#include <QTcpSocket>

class Scene : public QTcpServer
{
    Q_OBJECT
private:
    QTcpSocket* rcaSocket;
public:
    Scene();

public slots:
    void incomingConnection(int socketDescriptor);
    void readyRead();
    void deleteSocket();
};

#endif // SCENE_H
