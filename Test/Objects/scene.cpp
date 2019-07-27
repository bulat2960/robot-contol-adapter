#include "scene.h"

Scene::Scene(quint16 port)
{
    this->port = port;
    this->socket = nullptr;
}

void Scene::startServer()
{
    if (this->listen(QHostAddress::Any, port))
    {
        qDebug() << "Listening Scene";
    }
    else
    {
        qDebug() << "Not listening Scene";
    }
}

void Scene::closeServer()
{
    if (socket != nullptr)
    {
        socket->close();
        socket->deleteLater();
        socket = nullptr;
    }
    qDebug() << "Now scene is not connected with RCA";
}

void Scene::incomingConnection(qintptr socketDescriptor)
{
    // Create socket, set descritor
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    qDebug() << "RCA connected to scene";

    // Connect signals and slots
    connect(socket, &QTcpSocket::readyRead, this, &Scene::readyRead);
}

void Scene::readyRead()
{
    // Read data
    QByteArray data = socket->readAll();
    qDebug().noquote() << "Scene received message" << data << "from RCA";
    receivedMessages.push_back(QString(data));
}

bool Scene::isRcaConnected() const
{
    if (socket == nullptr)
    {
        return false;
    }
    return socket->state() == QAbstractSocket::ConnectedState;
}

bool Scene::isRcaDisconnected() const
{
    if (socket == nullptr)
    {
        return true;
    }
    return socket->state() == QAbstractSocket::UnconnectedState;
}

QString Scene::getLastMessage() const
{
    return receivedMessages.size() > 0 ? receivedMessages.back() : "";
}

int Scene::messagesCount() const
{
    return receivedMessages.size();
}
