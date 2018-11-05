#include "scene.h"

Scene::Scene()
{
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
    // Создаем новый сокет - канал связи между одним из ControlUnit
    rcaSocket = new QTcpSocket(this);
    rcaSocket->setSocketDescriptor(socketDescriptor);

    if (rcaSocket->isValid())
    {
        qDebug() << "RCA connected to Scene";
    }

    // Необходимые соединения слотов и сигналов
    connect(rcaSocket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(rcaSocket, SIGNAL(disconnected()), this, SLOT(deleteSocket()));
}

void Scene::readyRead()
{
    // Читаем данные
    QByteArray data = rcaSocket->readAll();
    qDebug().noquote() << "Scene received message" << data << "from RCA";
}

void Scene::deleteSocket()
{
    qDebug() << "Scene disconnect";
    rcaSocket->deleteLater();
}
