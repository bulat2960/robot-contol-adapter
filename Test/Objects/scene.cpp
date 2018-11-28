#include "scene.h"

Scene::Scene()
{
    timer.restart();

    // Start listening
    if (this->listen(QHostAddress("localhost"), 1111))
    {
        qDebug() << "Listening Scene";
    }
    else
    {
        qDebug() << "Not listening Scene";
    }

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

void Scene::incomingConnection(int socketDescriptor)
{
    timer.restart();

    // Create socket, set descritor
    rcaSocket = new QTcpSocket(this);
    rcaSocket->setSocketDescriptor(socketDescriptor);

    qDebug() << "RCA connected to scene";

    // Connect signals and slots
    connect(rcaSocket, &QTcpSocket::readyRead, this, &Scene::readyRead);

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

void Scene::readyRead()
{
    timer.restart();

    // Read data
    QByteArray data = rcaSocket->readAll();
    qDebug().noquote() << "Scene received message" << data << "from RCA";

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}
