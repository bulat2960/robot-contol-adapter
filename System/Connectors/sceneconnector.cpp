#include "sceneconnector.h"

SceneConnector::SceneConnector(QString ip, quint16 port)
{
    socket = new QTcpSocket(this);
    socket->connectToHost(ip, port);
}

void SceneConnector::slotSend(QByteArray msg)
{
    socket->write(msg);
}

SceneConnector::~SceneConnector()
{
    socket->disconnectFromHost();
    socket->deleteLater();
}
