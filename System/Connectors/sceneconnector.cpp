#include "sceneconnector.h"

SceneConnector::SceneConnector(QString ip, quint16 port)
{
    socket = new QTcpSocket(this);
    socket->connectToHost(ip, port);
}
