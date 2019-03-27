#include "scene.h"

Scene::Scene(quint16 port)
{
    // Start listening
    if (this->listen(QHostAddress::Any, port))
    {
        qDebug() << "Listening Scene";
    }
    else
    {
        qDebug() << "Not listening Scene";
    }
}

void Scene::incomingConnection(qintptr socketDescriptor)
{
    // Create socket, set descritor
    rcaSocket = new QTcpSocket(this);
    rcaSocket->setSocketDescriptor(socketDescriptor);

    qDebug() << "RCA connected to scene";

    // Connect signals and slots
    connect(rcaSocket, &QTcpSocket::readyRead, this, &Scene::readyRead);
}

void Scene::readyRead()
{
    // Read data
    QByteArray data = rcaSocket->readAll();
    qDebug().noquote() << "Scene received message" << data << "from RCA";
}
