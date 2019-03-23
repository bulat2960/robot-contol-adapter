#include "sceneconnector.h"

SceneConnector::SceneConnector(QString ip, quint16 port)
{
    socket = new QTcpSocket(this);
    socket->connectToHost(ip, port);
}

void SceneConnector::slotSend(QByteArray msg)
{
    QTime timer;
    timer.restart();

    socket->write(msg);

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

SceneConnector::~SceneConnector()
{
    QTime timer;
    timer.restart();

    socket->disconnectFromHost();
    socket->deleteLater();

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}
