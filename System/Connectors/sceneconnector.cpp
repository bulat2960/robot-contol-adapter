#include "sceneconnector.h"

SceneConnector::SceneConnector(QString ip, quint16 port)
{
    this->ip = ip;

    connectionTimes = 0;
    this->port = port;

    socket = new QTcpSocket(this);
    socket->connectToHost(ip, port);

    reconnectTimer.setDuration(200); // Magic constant?
    connect(&reconnectTimer, &QTimeLine::finished, this, &SceneConnector::slotSendAgain);
}

void SceneConnector::slotSend(QByteArray msg)
{
    QTime timer;
    timer.restart();

    if (socket->state() == QAbstractSocket::UnconnectedState)
    {
        socket->connectToHost(ip, port);

        if (connectionTimes < 5) // Magic constant?
        {
            connectionTimes++;
            reconnectTimer.start();
            unsentMessages.push_back(msg);
        }
    }
    else if (socket->waitForConnected())
    {
        connectionTimes = 0;
        socket->write(msg);
        socket->waitForBytesWritten();
    }

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

void SceneConnector::slotSendAgain()
{
    QByteArray s = unsentMessages.front();
    unsentMessages.pop_front();
    slotSend(s);
}

SceneConnector::~SceneConnector()
{
    QTime timer;
    timer.restart();

    socket->disconnectFromHost();
    socket->deleteLater();

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}
