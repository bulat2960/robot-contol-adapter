#include "scene.h"

Scene::Scene()
{
    // Start listening
    if (this->listen(QHostAddress("localhost"), 1111))
    {
        qDebug() << "Listening Scene";
    }
    else
    {
        qDebug() << "Not listening Scene";
    }
}

void Scene::incomingConnection(int socketDescriptor)
{
    // Create socket, set descritor
    rcaSocket = new QTcpSocket(this);
    rcaSocket->setSocketDescriptor(socketDescriptor);

    qDebug() << "Scene: new incoming connection(RCA)";

    // Connect signals and slots
    connect(rcaSocket, &QTcpSocket::readyRead, this, &Scene::readyRead);
}

void Scene::readyRead()
{
    // Read data
    QByteArray data = rcaSocket->readAll();
    qDebug().noquote() << "Scene received message" << data << "from RCA";
}
