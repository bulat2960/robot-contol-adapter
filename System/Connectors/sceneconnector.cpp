#include "sceneconnector.h"

SceneConnector::SceneConnector(QString ip, quint16 port, int untilReconnectDuration, int reconnectTimes)
{
    this->ip = ip;
    this->port = port;

    this->untilReconnectDuration = untilReconnectDuration;
    this->reconnectTimes = reconnectTimes;

    this->attemptsToReconnect = 0;

    socket = new QTcpSocket(this);
    socket->connectToHost(ip, port);

    reconnectTimer.setDuration(untilReconnectDuration);
    connect(&reconnectTimer, &QTimeLine::finished, this, &SceneConnector::slotSendAgain);
}

void SceneConnector::slotSend(QByteArray msg)
{
    QTime timer;
    timer.restart();

    if (isConnectedWithScene())
    {
        attemptsToReconnect = 0;
        socket->write(msg);
        socket->waitForBytesWritten();
    }
    else
    {
        attemptsToReconnect++;

        if (reconnectTimer.state() == QTimeLine::Running)
        {
            reconnectTimer.stop();
        }
        reconnectTimer.start();

        if (attemptsToReconnect < reconnectTimes)
        {
            unsentMessages.push_back(msg);
        }
    }

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}

bool SceneConnector::isConnectedWithScene()
{
    if (socket->state() == QAbstractSocket::UnconnectedState)
    {
        socket->connectToHost(ip, port);
        if (!socket->waitForConnected())
        {
            return false;
        }
    }

    return true;
}

void SceneConnector::slotSendAgain()
{
    if (isConnectedWithScene())
    {
        while (!unsentMessages.empty())
        {
            QByteArray s = unsentMessages.front();
            unsentMessages.pop_front();
            socket->write(s);
            socket->waitForBytesWritten();
        }
    }
    else
    {
        reconnectTimer.start();
    }
}

SceneConnector::~SceneConnector()
{
    QTime timer;
    timer.restart();

    socket->disconnectFromHost();
    socket->deleteLater();

    qDebug() << "Elapsed" << timer.elapsed() << "ms";
}
